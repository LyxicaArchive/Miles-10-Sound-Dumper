#include "stdafx.h"
#include "Recorder.h"
#include "Miles.h"

Recorder::Recorder(Bank bank) : bank(bank)
{

}

bool Recorder::Save() 
{
	char filename[256];
	byte headerData[44];
	FILE* file;

	MilesFillWavHeader(&headerData, 48000, 2, cursor);
	snprintf(filename, 256, "%s.wav", eventName);

	fopen_s(&file, filename, "wb");
	if (file == NULL) { return false; }

	fwrite(headerData, 1, 44, file);
	fwrite(data, 1, cursor, file);
	fclose(file);
	Reset();
	return true;
}

bool Recorder::Record(unsigned int eventId)
{
	if (Active()) { return false; }

	eventName = MilesBankGetEventName(bank, eventId);
	this->eventId = eventId;
}

char* Recorder::GetName()
{
	if (!Active()) { return NULL; }

	return eventName;
}

void Recorder::Append(PVOID buffer, unsigned int length)
{
	memcpy(data + cursor, buffer, length);
	cursor += length;
}

bool Recorder::Active()
{
	return eventName != NULL;
}

Recorder::~Recorder()
{
	delete[] data;
}

void Recorder::Reset() 
{
	memset(data, 0, cursor);
	cursor = 0;
	eventName = NULL;
	eventId = NULL;
}