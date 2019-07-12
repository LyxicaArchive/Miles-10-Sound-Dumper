// MSD.cpp : Defines the entry point for the console application.
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
#include "MSD.h"
#include "hooks.h"
#include "Recorder.h"
#include <filesystem>
bool EXPORT_EVENT_NAMES = false;
bool RECORDING_SESSION = false;
int events;
struct {
	int fielda;
	int fieldb;
} out;
std::vector<int> queuedEvents;

Recorder* recorder = 0;
byte* buffer_addr;
int write_size;

__int64 hook_GET_AUDIO_BUFFER_AND_SET_SIZE(__int64* a1, byte** BUFFER, int size) {
	write_size = size;
	auto return_val = hook1(a1, BUFFER, size);
	buffer_addr = *BUFFER;
	return return_val;
}

__int64 hook_TRANSFER_MIXED_AUDIO_TO_SOUND_BUFFER(__int64* a1) {
	if (recorder->Active()) {
		recorder->Append(buffer_addr, write_size);
	}

	return hook2(a1);
}

void PrintHelp() {
	std::cout << "--------- OPTIONS --------" << std::endl
				<< "\tMSD\t\t\t\t\t-- Open MSD to play sounds" << std::endl
				<< "\tMSD <EventID>\t\t\t\t-- Dump event ID" << std::endl
				<< "\tMSD <EventIDStart> <EventIDEnd>\t\t-- Dump a range of sounds, inclusive" << std::endl
				<< "\tMSD -l\t\t\t\t\t-- Display list of all event IDs and names contained in audio files" << std::endl;
}

void WINAPI logM(int number, char* message)
{
	std::cout << "Message received: " << message << "\r\n";
}

void _Record(Project project) {
	while (queuedEvents.size() > 0) {
		recorder->Record(queuedEvents.back());
		std::cout << "Recording " << recorder->GetName() << std::endl;

		MilesBankGetEventTemplateId(project.bank, queuedEvents.back(), (long long*)& out);
		MilesQueueEventVolume(project.queue, 1);
		MilesQueueControllerValue(project.queue, "GameMusicVolume", 1);
		MilesQueueControllerValue(project.queue, "DialogueVolume", 1);
		MilesQueueEventRunByTemplateId(project.queue, (int*)& out);
		SetupBusVolumes(project.driver);
		MilesQueueSubmit(project.queue);

		while (recorder->Active()) {
			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
				StopPlaying(project.queue);
			}
			Sleep(25);
		}
		queuedEvents.pop_back();
		Sleep(100); // Give the MilesMixer thread time to process any changes.
	}
}
void _Play(Project project) {
	int i;
	while (true) {
		while (recorder->Active()) {
			if (GetAsyncKeyState(VK_ESCAPE) & 0x80) {
				StopPlaying(project.queue);
				break;
			}
		}
		 
		std::cout << "Play Event ID: ";
		std::cin >> i;

		if (i < 0) {
			StopPlaying(project.queue);
			continue;
		}
		if (i >= events) {
			std::cout << "Invalid event ID" << std::endl;
			continue;
		}

		std::cout << "Playing " << MilesBankGetEventName(project.bank, i) << " (enter negative number to stop)" << std::endl;
		MilesBankGetEventTemplateId(project.bank, i, (long long*)& out);
		MilesQueueEventVolume(project.queue, 1);
		MilesQueueControllerValue(project.queue, "GameMusicVolume", 1);
		MilesQueueControllerValue(project.queue, "DialogueVolume", 1);
		MilesQueueEventRunByTemplateId(project.queue, (int*)& out);
		SetupBusVolumes(project.driver);
		MilesQueueSubmit(project.queue);
	}
}
bool cstrIsDigits(char* string)
{
	int x = 0;
	auto len = strlen(string);

	while (x < len) {
		if (!isdigit(*(string + x)))
			return false;

		++x;
	}

	return true;
}
int main(int argc, char* argv[])
{
	switch (argc)
	{
	case 1:
		RECORDING_SESSION = false;
		break;
	case 2:
		if (strcmp(argv[1], "-l") == 0)
		{
			EXPORT_EVENT_NAMES = true;
		}
		else
		{
			if (!cstrIsDigits(argv[1]))
			{
				std::cout << "Event ID passed contained invalid characters" << std::endl;
				PrintHelp();
				return 1;
			}
			queuedEvents.push_back(atoi(argv[1]));
			RECORDING_SESSION = true;
		}
		break;
	case 3: 
		if (!cstrIsDigits(argv[1]))
		{
			std::cout << "First event ID passed contained invalid characters" << std::endl;
			PrintHelp();
			return 1;
		}
		if (!cstrIsDigits(argv[2]))
		{
			std::cout << "Second event ID passed contained invalid characters" << std::endl;
			PrintHelp();
			return 1;
		}
		for (int i = atoi(argv[1]); i <= atoi(argv[2]); i++) {
			queuedEvents.push_back(i);
		}
		RECORDING_SESSION = true;
		break;
	default:
		PrintHelp();
		return 1;
	}
	if (!std::filesystem::exists(std::filesystem::path("./audio/ship/")))
	{
		std::cout << "Couldn't find ./audio/ship/ folder. Is MSD inside the Apex Legends folder?" << std::endl;
		return 1;
	}

	Project project = SetupMiles(&logM, EXPORT_EVENT_NAMES);
	recorder = new Recorder(project.bank);

	events = MilesBankGetEventCount(project.bank);
	if (EXPORT_EVENT_NAMES) {
		auto names = GetEventNames(project.bank);
		for (const auto& name : names) {
			std::cout << name;
		}

		return 1;
	}
	if (RECORDING_SESSION)
	{
		SetupHooks(project.driver, &hook_GET_AUDIO_BUFFER_AND_SET_SIZE, &hook_TRANSFER_MIXED_AUDIO_TO_SOUND_BUFFER);
		_Record(project);
	}
	else {
		_Play(project);
	}
	
	return 0;
}
