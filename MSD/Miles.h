#pragma once
#include <Windows.h>
#include <xmmintrin.h>
#include <vector>
#include <string>
#include <sstream>

typedef __int64* Driver;
typedef __int64* Bank;
typedef __int64* Bus;
typedef __int64* Queue;

// imported from miles
extern "C" __declspec(dllimport) int MilesStartup(void (WINAPI *logger)(int a, char *b));
extern "C" __declspec(dllimport) int MilesIntAuditionReport(const char *msg);
extern "C" __declspec(dllimport) Driver MilesDriverCreate(__int64 *output);
extern "C" __declspec(dllimport) __int64 MilesProjectLoad(Driver driver, const char *a2, const char *a3, const char *a4);
extern "C" __declspec(dllimport) __int64 MilesProjectGetStatus(Driver driver);
extern "C" __declspec(dllimport) void MilesAllocTrack(__int64 a1);
extern "C" __declspec(dllimport) char* MilesProjectStatusToString(__int64 a1);
extern "C" __declspec(dllimport) Bank MilesBankLoad(Driver driver, const char *a2, const char *a3, const char *a4, __int64 a5, __int64 a6);
extern "C" __declspec(dllimport) int MilesBankGetStatus(Bank bank, int *a2);
extern "C" __declspec(dllimport) char* MilesBankStatusToString(__int64 a1);
extern "C" __declspec(dllimport) __int64 MilesBankPatch(Bank bank, const char *a2, const char *a3);
extern "C" __declspec(dllimport) Bus MilesProjectGetBus(Driver driver, const char *a2);
extern "C" __declspec(dllimport) __int64* MilesSampleCreate(__int64 a1, __int64 a2, __int64 a3);
extern "C" __declspec(dllimport) void MilesSamplePlay(__int64* a1);
extern "C" __declspec(dllimport) bool MilesSampleSetSourceRaw(__int64* sample, __int64 a2, __int64 a3, __int64 a4, __int16 a5);
extern "C" __declspec(dllimport) bool MilesSampleSetSource(__int64* sample, __int64 a2, __int64 a3, __int64 a4);
extern "C" __declspec(dllimport) int MilesSetStartupParameters(int *a);
extern "C" __declspec(dllimport) __int64 (*MilesOutputDirectSound())(__int64* OUT_buffer, int, int, unsigned int, int, __int64);
extern "C" __declspec(dllimport) __int64 MilesDriverRegisterDecoder(Driver driver, __int64*);
extern "C" __declspec(dllimport) int MilesBankGetEventCount(Bank bank);
extern "C" __declspec(dllimport) __int64 MilesBankGetSourceCount(Bank bank);
extern "C" __declspec(dllimport) char* MilesBankGetSourceName(Bank bank, int);
extern "C" __declspec(dllimport) char* MilesBankGetEventName(Bank bank, int);
extern "C" __declspec(dllimport) __int64 MilesBankGetEventTemplateId(Bank bank, int, __int64*);
extern "C" __declspec(dllimport) Queue MilesQueueCreate(Driver driver);
extern "C" __declspec(dllimport) __int64 MilesQueueSubmit(Queue queue);
extern "C" __declspec(dllimport) __int64 MilesQueueEventRunByTemplateId(Queue queue, int*);
extern "C" __declspec(dllimport) __int64 MilesQueueEventRun(Queue queue, const char*);
extern "C" __declspec(dllimport) __int64 MilesQueueEventFilterId(__int64, __int64);
extern "C" __declspec(dllimport) __int64 MilesDriverSetMasterVolume(Driver driver, float a2);
extern "C" __declspec(dllimport) __int64 MilesEventInfoQueueEnable(Driver driver);
extern "C" __declspec(dllimport) __int64 MilesEventSetStreamingCacheLimit(Driver driver, __int64);
extern "C" __declspec(dllimport) __int64 MilesEventControllerNames(__int64, __int64, __int64*, unsigned int);
extern "C" __declspec(dllimport) __m128  MilesBusGetVolumeLevel(Bus bus);
extern "C" __declspec(dllimport) __int64 MilesBusSetVolumeLevel(Bus bus, float a2);
extern "C" __declspec(dllimport) __int64 MilesQueueControllerValue(Queue queue, const char* a2, float a3);
extern "C" __declspec(dllimport) __int64 MilesQueueEventInfoMask(__int64 a1, int a2);
extern "C" __declspec(dllimport) __int64 MilesQueueEventControllerValue(__int64 a1, const char* a2, float a3);
extern "C" __declspec(dllimport) __int64 MilesBankGetSourceTemplateId(Bank bank, int, __int64*);
extern "C" __declspec(dllimport) __int64 MilesQueueEvent3DPosition(__int64 a1, float a2, float a3, float a4);
extern "C" __declspec(dllimport)    void MilesFillWavHeader(PVOID OUT_BUFFER, int sampleRateHz, unsigned __int16 channels, int size);
extern "C" __declspec(dllimport)    void MilesBankCommitPatches(Bank bank);

// imported from abinkwin64
extern "C" __declspec(dllimport) __int64 MilesDriverRegisterBinkAudio(Driver driver);

// Helpers
struct Project {
	Driver driver;
	Bank bank;
	Queue queue;
};

void SetupBusVolumes(Driver driver);
std::vector<std::string> GetEventNames(Bank bank);
Project SetupMiles(void (WINAPI* callback)(int, char*), std::string dir_path, bool silent);
void StopPlaying(Queue queue);

// Structures
struct unk {
	__int64* sound_function = 0;
	LPCWSTR* endpointID = 0;
	INT32 channel_count = 0;
	INT32 maybe_sample_rate = 0;
	WORD field18 = 0;
	WORD field1A = 0;
	__int64 hrtf_buffer = 0;
};
