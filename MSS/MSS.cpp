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
#include <filesystem>
bool EXPORT_EVENT_NAMES = false;

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

bool cstrIsDigits(char* string)
{
	int x = 0;
	int len = strlen(string);

	while (x < len) {
		if (!isdigit(*(string + x)))
			return false;

		++x;
	}

	return true;
}
int main(int argc, char* argv[])
{
	int i;
	if (argc != 2 && argc != 3) {
		std::cout << "Invalid parameters" << std::endl;
		return 1;
	}
	if (!std::filesystem::exists(std::filesystem::path("./audio/ship/"))) 
	{
		std::cout << "Couldn't find ./audio/ship/ folder. Is MSS inside the Apex Legends folder?" << std::endl;
		return 1;
	}

	if (strcmp(argv[1], "-l") == 0) 
	{
		EXPORT_EVENT_NAMES = true;
	}

	Project project = SetupMiles(&logM, EXPORT_EVENT_NAMES);
	recorder = new Recorder(project.bank);

	auto events = MilesBankGetEventCount(project.bank);
	struct {
		int fielda;
		int fieldb;
	} out;

	if (EXPORT_EVENT_NAMES) {
		auto names = GetEventNames(project.bank);
		for (const auto& name : names) {
			std::cout << name;
		}

		return 1;
	}
	else if (argc == 2 && cstrIsDigits(argv[1])) {

	}
	else {
		std::cout << "Invalid parameters" << std::endl;
		return 1;
	}

	SetupHooks(project.driver, &hook_GET_AUDIO_BUFFER_AND_SET_SIZE, &hook_TRANSFER_MIXED_AUDIO_TO_SOUND_BUFFER);
	while (true) {
		while (recorder->Active()) {
			if (GetAsyncKeyState(VK_ESCAPE) & 0x80) {
				std::cout << "STOP" << std::endl;
				StopPlaying(project.queue);
				break;
			}
		}

		std::cout << "n: ";
		std::cin >> i;

		if (i >= events) {
			std::cout << "invalid" << std::endl;
			continue;
		}

		if (recorder->Record(i)) {
			timeLastRecvFrame = timeGetTime();
			std::cout << "Recording " << recorder->GetName() << " (ESC to stop)" << std::endl;
		}

		MilesBankGetEventTemplateId(project.bank, i, (long long*)& out);
		MilesQueueEventVolume(project.queue, 1);
		MilesQueueControllerValue(project.queue, "GameMusicVolume", 1);
		MilesQueueControllerValue(project.queue, "DialogueVolume", 1);
		MilesQueueEventRunByTemplateId(project.queue, (int*)& out);
		MilesQueueSubmit(project.queue);
	}
		
	std::cin >> i;
	return 0;
}
