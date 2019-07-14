#include <Windows.h>
#include <Miles.h>
#pragma once
#define BUFFER_SIZE 1024 * 1024 * 100 // 100 MB
#define WAV_INITIAL_POS = 44

class Recorder
{
public:
	Recorder(Bank bank);
	bool Save();
	bool Record(unsigned int eventId);
	char* GetName();
	void Append(PVOID buffer, unsigned int length);
	bool Active();
	~Recorder();
	
private:
	unsigned char* data = new unsigned char[BUFFER_SIZE];
	unsigned int cursor;
	char* eventName;
	unsigned int eventId;
	Bank bank;
	bool active;
	DWORD timeLastNonSilentSample; // Timestamp of the last received audio buffer frame that contains audio (ie not full of 0s)
	bool firstSampleReceived;

	static bool IsDataSilent(unsigned short* buffer, int size);
	void Reset();
};

