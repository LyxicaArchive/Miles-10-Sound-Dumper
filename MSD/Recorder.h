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
	unsigned int cursor = 0;
	char* eventName = 0;
	unsigned int eventId = 0;
	Bank bank;
	bool active = false;

	void Reset();
};

