// MSS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <vector>
#include <iomanip>
#include <Timeapi.h>
#include "Miles.h"
#include "MSS.h"
#include "hooks.h"
#include "Recorder.h"

Recorder* recorder = 0;
byte* buffer_addr;
int write_size;
DWORD timeLastRecvFrame; // Timestamp of the last received audio buffer frame that contains audio (ie not full of 0s)

__int64 hook_GET_AUDIO_BUFFER_AND_SET_SIZE(__int64* a1, byte** BUFFER, int size) {
	write_size = size;
	auto return_val = hook1(a1, BUFFER, size);
	buffer_addr = *BUFFER;
	return return_val;
}

bool valid_data(byte* buffer, int size) {
	for (int i = 0; i < size; i++) {
		if (buffer[i] != 0) { return true; }
	}

	return false;
}

__int64 hook_TRANSFER_MIXED_AUDIO_TO_SOUND_BUFFER(__int64* a1) {
	if (recorder->Active()) {
		recorder->Append(buffer_addr, write_size);

		if (valid_data(buffer_addr, write_size))
		{
			timeLastRecvFrame = timeGetTime();
		}
		else if (timeGetTime() - timeLastRecvFrame > 250) // After 250ms of silence, the sound is probably done.
		{
			std::cout << "Saving " << recorder->GetName() << std::endl;
			recorder->Save();
		}
	}

	return hook2(a1);
}

void WINAPI logM(int number, char* message)
{
	std::cout << "Message received: " << message << "\r\n";
}

int main()
{

	int i;
	int startup_parameters = 0;

	MilesSetStartupParameters(&startup_parameters);
	MilesAllocTrack(2);
	__int64 startup = MilesStartup(&logM);
	std::cout << "Start up: " << startup << "\r\n";

	auto output = MilesOutputDirectSound();
	unk a1;
	a1.sound_function = (long long*)output;
	a1.endpointID = NULL; // Default audio Device 
	a1.maybe_sample_rate = 48000;
	a1.channel_count = 2;
	__int64 driver = MilesDriverCreate((long long*)&a1);
	MilesDriverRegisterBinkAudio(driver);
	MilesEventSetStreamingCacheLimit(driver, 0x4000000);
	MilesDriverSetMasterVolume(driver, 0.5);
	auto queue = MilesQueueCreate(driver);
	MilesEventInfoQueueEnable(driver);

	Bank bank = LoadProject((Driver)driver);
	recorder = new Recorder(bank);

	auto events = MilesBankGetEventCount(bank);
	struct {
		int fielda;
		int fieldb;
	} out;
	SetupHooks((PVOID)driver, &hook_GET_AUDIO_BUFFER_AND_SET_SIZE, &hook_TRANSFER_MIXED_AUDIO_TO_SOUND_BUFFER);
	while (true) {

		std::cout << "n: ";
		std::cin >> i;

		if (i >= events) {
			std::cout << "invalid" << std::endl;
			continue;
		}

		if (i < 0) {
			MilesQueueEventRunByTemplateId(queue, (int*)& out);
			MilesQueueEventRun(queue, "STOPNOW");
			MilesQueueSubmit(queue);
			continue;
		}

		if (recorder->Record(i)) {
			timeLastRecvFrame = timeGetTime();
			std::cout << "Recording " << recorder->GetName() << std::endl;
		}

		MilesBankGetEventTemplateId(bank, i, (long long*)& out);
		MilesQueueEventVolume(queue, 1);
		MilesQueueControllerValue(queue, "GameMusicVolume", 1);
		MilesQueueControllerValue(queue, "DialogueVolume", 1);
		MilesQueueEventRunByTemplateId(queue, (int*)& out);
		MilesQueueSubmit(queue);
	}
		
	std::cin >> i;
	return 0;
}
