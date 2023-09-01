#include "stdafx.h"
#include "Miles.h"
#include <winreg.h>
#include <iostream>
#include <filesystem>
#include <regex>
#include <conio.h>
#include "args.hxx"

namespace fs = std::filesystem;

extern args::ValueFlag<std::string> exportLanguage;

void SetupBusVolumes(Driver driver)
{
	MilesBusSetVolumeLevel(MilesProjectGetBus(driver, "Master"), 0.7f);
	MilesBusSetVolumeLevel(MilesProjectGetBus(driver, "Voice_Comm_bus"), 0.7f);
	MilesBusSetVolumeLevel(MilesProjectGetBus(driver, "MUSIC"), 0.7f);
}

std::vector<std::string> GetEventNames(Bank bank)
{
	std::vector<std::string> collectedNames;
	for (int i = 0; i < MilesBankGetEventCount(bank); i++) {
		std::stringstream line;
		line << i << "," << MilesBankGetEventName(bank, i) << std::endl;
		collectedNames.push_back(line.str());
	}

	return collectedNames;
}

bool GetMatchingFile(std::regex reg, std::string* out, std::string dir_path)
{
	for (const auto& entry : fs::directory_iterator(fs::path(dir_path)))
	{
		if (std::regex_search(entry.path().string(), reg))
		{
			if (out) 
			{
				*out = entry.path().string();
			}
			return true;
		}
	}
	return false;
}

bool GetLocalizedLanguage(std::vector<std::string> *out, std::string dir_path)
{
	auto reg = std::regex("general_(\\w*)_patch_.*.mstr");
	for (const auto& entry : fs::directory_iterator(fs::path(dir_path)))
	{
		std::smatch languageMatch;
		std::string path = entry.path().string();
		if (std::regex_search(path, languageMatch, reg) && languageMatch[1].str()!="stream")
		{
			auto in = std::find((*out).begin(), (*out).end(), languageMatch[1].str());
			if (in == (*out).end()) {
				(*out).push_back(languageMatch[1].str());
			}
		}
	}
	if ((*out).size())
		return true;
	else
		return false;
}

unsigned int GetPatchCount(std::string dir_path)
{
	unsigned int count = 0;
	auto reg = std::regex(".*_patch_(\\d*).mstr");
	for (const auto& entry : fs::directory_iterator(fs::path(dir_path)))
	{
		std::smatch languageMatch;
		std::string path = entry.path().string();
		if (std::regex_search(path, languageMatch, reg))
		{
			unsigned int num = std::stoi(languageMatch[1].str());
			if (num > count)
			{
				count = num;
			}
		}
	}
	return count;
}

bool IsPatched(std::string dir_path)
{
	return GetMatchingFile(std::regex("_patch_"), 0, dir_path);
}

void GetChosenLanguage(std::string* language,std::vector<std::string>* found_language) {
	std::string text;
	for (int i = 0; i < (*found_language).size(); i++) {
		text += (*found_language)[i] + " ";
	}
	std::cout << "Found those Language: " + text << std::endl;
	std::cout << "Choose Language which you want to export(use Tab key to switch): " + (*found_language)[0];
	int key = _getch();
	int index = 0;
	while (true)
	{
		if (key == 13) {
			*language = (*found_language)[index];
			std::cout << std::endl;
			break;
		}
		else if (key == 9) {
			std::cout << std::string((*found_language)[index].length(), '\b');
			if (index < ((*found_language).size() - 1)) {
				index++;
			}
			else {
				index = 0;
			}
			std::cout << (*found_language)[index];
			key = _getch();
		}
		else
		{
			key = _getch();
		}
	};
}


Project SetupMiles(void (WINAPI* callback)(int, char*), std::string dir_path, bool silent)
{ 
	Project project;
	int startup_parameters = 0;

	MilesSetStartupParameters(&startup_parameters);
	MilesAllocTrack(2);
	__int64 startup = MilesStartup(callback);
	if (!silent)
	{
		std::cout << "Start up: " << startup << "\r\n";
	}

	auto output = MilesOutputDirectSound();
	unk a1;
	a1.sound_function = (long long*)output;
	a1.endpointID = NULL; // Default audio Device 
	a1.maybe_sample_rate = 48000;
	a1.channel_count = 2;
	project.driver = MilesDriverCreate((long long*)& a1);

	MilesDriverRegisterBinkAudio(project.driver);
	MilesEventSetStreamingCacheLimit(project.driver, 0x4000000);
	MilesDriverSetMasterVolume(project.driver, 1);
	project.queue = MilesQueueCreate(project.driver);
	MilesEventInfoQueueEnable(project.driver);

	std::string mprj;
	std::vector<std::string> found_language;
	std::string language;
	GetMatchingFile(std::regex(".mprj$"), &mprj, dir_path);
	GetLocalizedLanguage(&found_language, dir_path);
	if (found_language.empty()) {
		std::cout << "Can't found language file!" << std::endl;
		std::cout << "Press any key to exit!" << std::endl;
		int temp = _getch();
		exit(0);
	}
	if(args::get(exportLanguage).empty()) {
		language = found_language[0];
		if ( found_language.size() != 1 && !silent ) {
			GetChosenLanguage(&language, &found_language);
		}
	}
	else {
		std::string mlanguage = args::get(exportLanguage).c_str();
		auto in = std::find(found_language.begin(), found_language.end(), mlanguage);
		if (in == found_language.end()) {
			language = found_language[0];
		}
		else {
			language = mlanguage;
		}
	}


	std::string project_name = mprj;

	//Extract project name from .mprj path. Sourced from https://stackoverflow.com/questions/8520560/get-a-file-name-from-a-path
	const size_t last_slash_idx = project_name.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		project_name.erase(0, last_slash_idx + 1);
	}
	const size_t period_idx = project_name.rfind('.');
	if (std::string::npos != period_idx)
	{
		project_name.erase(period_idx);
	}

	MilesProjectLoad(project.driver, mprj.c_str(), language.c_str(), project_name.c_str());

	__int64 status = MilesProjectGetStatus(project.driver);
	while (status == 0) {
		Sleep(500);
		status = MilesProjectGetStatus(project.driver);
	}
	status = MilesProjectGetStatus(project.driver);
	if (!silent) 
	{ 
		std::cout << "status: " << MilesProjectStatusToString(status) << std::endl; 
	}

	std::string mbnk;
	std::string general;
	std::string localized;
	GetMatchingFile(std::regex("general\\.mbnk$"), &mbnk, dir_path);
	GetMatchingFile(std::regex("general_stream\\.mstr"), &general, dir_path);
	GetMatchingFile(std::regex("general_((?!stream)\\w)*\\.mstr"), &localized, dir_path);
	project.bank = MilesBankLoad(project.driver, mbnk.c_str(), general.c_str(), localized.c_str(), 1, 0);
	auto patch_count = GetPatchCount(dir_path);

	for (unsigned int i = 1; i <= patch_count; i++)
	{
		std::string general_patch;
		std::string localized_patch;
		std::stringstream general_patch_str;
		std::stringstream localized_patch_str;
		general_patch_str << "general_stream_patch_" << i << "\\.mstr";
		localized_patch_str << "general_((?!stream)\\w)*\\_patch_" << i << "\\.mstr";
		GetMatchingFile(std::regex(general_patch_str.str()), &general_patch, dir_path);
		GetMatchingFile(std::regex(localized_patch_str.str()), &localized_patch, dir_path);
		MilesBankPatch(project.bank, general_patch.c_str(), localized_patch.c_str());
	}

	if (patch_count > 0)
	{
		MilesBankCommitPatches(project.bank);
	}

	int bank_status = MilesBankGetStatus(project.bank, 0);
	while (bank_status == 0) {
		bank_status = MilesBankGetStatus(project.bank, 0);
	}
	if (!silent) 
	{ 
		std::cout << "bank_status: " << MilesBankStatusToString(bank_status) << std::endl;
	}

	return project;
}

void StopPlaying(Queue queue) 
{
	MilesQueueEventRun(queue, "STOPNOW");
	MilesQueueSubmit(queue);
}