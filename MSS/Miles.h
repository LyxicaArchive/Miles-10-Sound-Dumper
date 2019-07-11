#pragma once
#include <Windows.h>
#include <xmmintrin.h>
typedef __int64* Driver;
typedef __int64* Bank;

// imported from miles
extern "C" __declspec(dllimport) int MilesStartup(void (WINAPI *logger)(int a, char *b));
extern "C" __declspec(dllimport) int MilesIntAuditionReport(const char *msg);
extern "C" __declspec(dllimport) __int64 MilesDriverCreate(__int64 *output);
extern "C" __declspec(dllimport) __int64 MilesProjectLoad(__int64 a1, const char *a2, const char *a3, const char *a4);
extern "C" __declspec(dllimport) __int64 MilesProjectGetStatus(__int64 a1);
extern "C" __declspec(dllimport) void MilesAllocTrack(__int64 a1);
extern "C" __declspec(dllimport) char* MilesProjectStatusToString(__int64 a1);
extern "C" __declspec(dllimport) Bank MilesBankLoad(__int64 a1, const char *a2, const char *a3, const char *a4, __int64 a5);
extern "C" __declspec(dllimport) __int64 MilesBankGetStatus(Bank bank, int *a2);
extern "C" __declspec(dllimport) char* MilesBankStatusToString(__int64 a1);
extern "C" __declspec(dllimport) __int64 MilesBankPatch(Bank bank, const char *a2, const char *a3);
extern "C" __declspec(dllimport) __int64* MilesProjectGetBus(__int64 a1, const char *a2);
extern "C" __declspec(dllimport) __int64* MilesSampleCreate(__int64 a1, __int64 a2, __int64 a3);
extern "C" __declspec(dllimport) void MilesSamplePlay(__int64* a1);
extern "C" __declspec(dllimport) bool MilesSampleSetSourceRaw(__int64* sample, __int64 a2, __int64 a3, __int64 a4, __int16 a5);
extern "C" __declspec(dllimport) bool MilesSampleSetSource(__int64* sample, __int64 a2, __int64 a3, __int64 a4);
extern "C" __declspec(dllimport) int MilesSetStartupParameters(int *a);
extern "C" __declspec(dllimport) __int64 (*MilesOutputDirectSound())(__int64* OUT_buffer, int, int, unsigned int, int, __int64);
extern "C" __declspec(dllimport) __int64 MilesDriverRegisterDecoder(__int64* a1, __int64*);
extern "C" __declspec(dllimport) __int64 MilesBankGetEventCount(Bank bank);
extern "C" __declspec(dllimport) __int64 MilesBankGetSourceCount(__int64);
extern "C" __declspec(dllimport) char* MilesBankGetSourceName(__int64, int);
extern "C" __declspec(dllimport) char* MilesBankGetEventName(Bank bank, int);
extern "C" __declspec(dllimport) __int64 MilesBankGetEventTemplateId(Bank bank, int, __int64*);
extern "C" __declspec(dllimport) __int64 MilesQueueCreate(__int64);
extern "C" __declspec(dllimport) __int64 MilesQueueSubmit(__int64);
extern "C" __declspec(dllimport) __int64 MilesQueueEventRunByTemplateId(__int64, int*);
extern "C" __declspec(dllimport) __int64 MilesQueueEventRun(__int64, const char*);
extern "C" __declspec(dllimport) __int64 MilesQueueEventFilterId(__int64, __int64);
extern "C" __declspec(dllimport) __int64 MilesQueueEventVolume(__int64, float);
extern "C" __declspec(dllimport) __int64 MilesDriverSetMasterVolume(__int64 a1, float a2);
extern "C" __declspec(dllimport) __int64 MilesEventInfoQueueEnable(__int64 a1);
extern "C" __declspec(dllimport) __int64 MilesEventSetStreamingCacheLimit(__int64, __int64);
extern "C" __declspec(dllimport) __int64 MilesEventControllerNames(__int64, __int64, __int64*, unsigned int);
extern "C" __declspec(dllimport) __m128  MilesBusGetVolumeLevel(__int64 a1);
extern "C" __declspec(dllimport) __int64 MilesBusSetVolumeLevel(__int64* a1, float a2);
extern "C" __declspec(dllimport) __int64 MilesQueueControllerValue(__int64 a1, const char* a2, float a3);
extern "C" __declspec(dllimport) __int64 MilesQueueEventInfoMask(__int64 a1, int a2);
extern "C" __declspec(dllimport) __int64 MilesQueueEventControllerValue(__int64 a1, const char* a2, float a3);
extern "C" __declspec(dllimport) __int64 MilesBankGetSourceTemplateId(__int64, int, __int64*);
extern "C" __declspec(dllimport) __int64 MilesQueueEvent3DPosition(__int64 a1, float a2, float a3, float a4);
extern "C" __declspec(dllimport)    void MilesFillWavHeader(__int64 OUT_BUFFER, int sampleRateHz, unsigned __int16 channels, int size);

// imported from abinkwin64
extern "C" __declspec(dllimport) __int64 MilesDriverRegisterBinkAudio(__int64 a);

// Helpers
void SetupBusVolumes(__int64 driver)
{
	/*MilesBusSetVolumeLevel(MilesProjectGetBus(driver, "Master"), 1);
	MilesBusSetVolumeLevel(MilesProjectGetBus(driver, "Voice_Comm_bus"), 1);
	MilesBusSetVolumeLevel(MilesProjectGetBus(driver, "SFX_ui"), 1);
	MilesBusSetVolumeLevel(MilesProjectGetBus(driver, "SFX"), 1);*/
}