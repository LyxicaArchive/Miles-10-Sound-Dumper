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
#include "args.hxx"	

args::ArgumentParser parser("Miles 10 Sound Dumper by Lyxica v1.0-beta3");
args::ValueFlag<std::string> audioFolder(parser, "/audio/ship", "Folder containing Miles audio files (mprj, mbnk, mstr)", { "folder" }, { "./audio/ship" });
args::ValueFlag<std::string> outputFolder(parser, "/miles_audio", "Folder to place the audio files in", { 'o', "out" }, { "./miles_audio" });
args::Flag listBankEvents(parser, "EVENTLIST", "List all event IDs and names contained in the Mile's bank", { 'l', "list" });
args::Flag muteSound(parser, "QUIET", "Mute audio while recording events", { 'm', "mute" });
args::PositionalList<int> eventIDs(parser, "EVENT IDs", "Enter either one or two event IDs. Entering only one will cause that event to be recorded. Entering two event IDs will record every event between the two event IDs.");
args::Group advancedGroup(parser, "ADVANCED");
args::ValueFlag<int> noiseFloor(advancedGroup, "0x2000", "Adjust the noise floor when detecting silence. Any samples below this value will be considered silent.", { "noise" }, 0x2000);
args::HelpFlag help(parser, "help", "Display this help menu", { 'h', "help" });
std::vector<int> queuedEvents;
Project project;

int events;
struct {
	int fielda;
	int fieldb;
} out;

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
		MilesQueueControllerValue(project.queue, "LobbyMusicVolume", 1);
		MilesQueueControllerValue(project.queue, "SfxVolume", 1);
		MilesQueueEventRunByTemplateId(project.queue, (int*)& out);
		SetupBusVolumes(project.driver);
		MilesQueueSubmit(project.queue);

		while (recorder->Active()) {
			Sleep(25);
		}
		queuedEvents.pop_back();
		Sleep(100); // Give the MilesMixer thread time to process any changes.
	}
}
void _Play(Project project) {
	std::string input;
	int i;
	while (true) {
		while (recorder->Active()) {
			if (GetAsyncKeyState(VK_ESCAPE) & 0x80) {
				StopPlaying(project.queue);
				break;
			}
		}
		
		while (true) 
		{
			std::cout << "Play Event ID: ";
			std::cin >> input;
			if (cstrIsDigits(input.c_str()))
			{
				i = atoi(input.c_str());
				break;
			}
			else {
				std::cout << "Invalid event ID. Expected number between 0 and " << events << std::endl;
			}
		}

		if (i < 0) {
			StopPlaying(project.queue);
			continue;
		}
		if (i >= events) {
			std::cout << "Invalid event ID. Expected number between 0 and " << events << std::endl;
			continue;
		}

		std::cout << "Playing " << MilesBankGetEventName(project.bank, i) << " (enter negative number to stop)" << std::endl;
		MilesBankGetEventTemplateId(project.bank, i, (long long*)& out);
		MilesQueueEventVolume(project.queue, 1);
		MilesQueueControllerValue(project.queue, "GameMusicVolume", 1);
		MilesQueueControllerValue(project.queue, "DialogueVolume", 1);
		MilesQueueControllerValue(project.queue, "LobbyMusicVolume", 1);
		MilesQueueControllerValue(project.queue, "SfxVolume", 1);

		MilesQueueEventRunByTemplateId(project.queue, (int*)& out);
		SetupBusVolumes(project.driver);
		MilesQueueSubmit(project.queue);
	}
}
bool cstrIsDigits(const char* string)
{
	int x = 0;
	auto len = strlen(string);

	if (string[0] == '-') { x = 1; }
	while (x < len) {
		if (!isdigit(*(string + x)))
			return false;

		++x;
	}

	return true;
}
int main(int argc, char* argv[])
{	
	try
	{
		parser.ParseCLI(argc, argv);
	}
	catch (args::Help)
	{
		std::cout << parser;
		return 0;
	}
	catch (args::ParseError e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << parser;
		return 1;
	}

	if (!std::filesystem::exists(std::filesystem::path(args::get(audioFolder))))
	{
		std::cout << "Folder " << args::get(audioFolder) << " not found" << std::endl;
		return 1;
	}

	project = SetupMiles(&logM, args::get(audioFolder), listBankEvents);
	recorder = new Recorder(project.bank);
	events = MilesBankGetEventCount(project.bank);

	if (listBankEvents) {
		auto names = GetEventNames(project.bank);
		for (const auto& name : names) {
			std::cout << name;
		}

		return 1;
	}

	auto ids = args::get(eventIDs);
	if (ids.size() > 2)
	{
		std::cout << "Two is the maximum number of event IDs that can be entered." << std::endl;
	}

	if (ids.size() > 0) 
	{
		auto ids = args::get(eventIDs);
		if (ids.size() == 1)
		{
			queuedEvents.push_back(ids[0]);
		}
		else
		{
			for (int i = ids[0]; i <= ids[1]; i++) {
				queuedEvents.push_back(i);
			}
		}
		SetupHooks(project.driver, &hook_GET_AUDIO_BUFFER_AND_SET_SIZE, &hook_TRANSFER_MIXED_AUDIO_TO_SOUND_BUFFER);
		_Record(project);
	}
	else 
	{
		_Play(project);
	}
}
