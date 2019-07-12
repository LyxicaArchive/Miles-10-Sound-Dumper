#include "stdafx.h"
#include "Miles.h"
#include <winreg.h>
#include <iostream>
#include <filesystem>
#include <regex>

namespace fs = std::filesystem;

void SetupBusVolumes(Driver driver)
{
	MilesBusSetVolumeLevel(MilesProjectGetBus(driver, "Master"), 1);
	MilesBusSetVolumeLevel(MilesProjectGetBus(driver, "Voice_Comm_bus"), 1);
	MilesBusSetVolumeLevel(MilesProjectGetBus(driver, "SFX_ui"), 1);
	MilesBusSetVolumeLevel(MilesProjectGetBus(driver, "SFX"), 1);
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

bool GetMatchingFile(std::regex reg, std::string* out)
{
	for (const auto& entry : fs::directory_iterator(fs::path("./audio/ship/")))
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

bool GetLocalizedLanguage(std::string* out)
{
	auto reg = std::regex("general_(\\w*)_patch_\\d.mstr");
	for (const auto& entry : fs::directory_iterator(fs::path("./audio/ship/")))
	{
		std::smatch languageMatch;
		std::string path = entry.path().string();
		if (std::regex_search(path, languageMatch, reg))
		{
			*out = languageMatch[1].str();
			return true;
		}
	}
	return false;
}

bool IsPatched()
{
	return GetMatchingFile(std::regex("_patch_"), 0);
}

Bank LoadProject(Driver driver) 
{ 
	std::string project;
	std::string language;
	GetMatchingFile(std::regex("audio.mprj$"), &project);
	GetLocalizedLanguage(&language);

	MilesProjectLoad(driver, project.c_str(), language.c_str(), "audio");

	__int64 status = MilesProjectGetStatus(driver);
	while (status == 0) {
		Sleep(500);
		status = MilesProjectGetStatus(driver);
	}
	status = MilesProjectGetStatus(driver);
	std::cout << "status: " << MilesProjectStatusToString(status) << std::endl;

	std::string mbnk;
	std::string general;
	std::string localized;
	GetMatchingFile(std::regex("general\\.mbnk$"), &mbnk);
	GetMatchingFile(std::regex("general_stream\\.mstr"), &general);
	GetMatchingFile(std::regex("general_\\w*\\.mstr"), &localized);
	Bank bank = MilesBankLoad(driver, mbnk.c_str(), general.c_str(), localized.c_str(), 0);

	if (IsPatched()) {
		std::string general_patch;
		std::string localized_patch;
		GetMatchingFile(std::regex("general_stream_patch_\\d\\.mstr"), &general_patch);
		GetMatchingFile(std::regex("general_\\w*_patch_\\d\\.mstr"), &localized_patch);
		MilesBankPatch(bank, general_patch.c_str(), localized_patch.c_str());
	}

	int bank_status = MilesBankGetStatus(bank, 0);
	while (bank_status == 0) {
		bank_status = MilesBankGetStatus(bank, 0);
	}
	std::cout << "bank_status: " << MilesBankStatusToString(bank_status) << std::endl;

	return bank;
}

void StopPlaying(Queue queue) 
{
	MilesQueueEventRun(queue, "STOPNOW");
	MilesQueueSubmit(queue);
}