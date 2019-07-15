#include "stdafx.h"
#include "Recorder.h"
#include "Miles.h"
#include <iostream>
#include "args.hxx"

extern args::ValueFlag<int> noiseFloor;
extern args::Flag muteSound;

bool Recorder::IsDataSilent(unsigned short* buffer, int size) {
	for (int i = 0; i < size/2; i++) { // size is bytes
		if (buffer[i] >= args::get(noiseFloor)) { return false; } // Samples are stored BIG-ENDIAN, this seems like a decent noise floor
	}

	return true;
}

Recorder::Recorder(Bank bank) : bank(bank)
{
	Reset();
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
	timeLastNonSilentSample = timeGetTime();
	this->eventId = eventId;
	active = true;
	return true;
}

char* Recorder::GetName()
{
	if (!Active()) { return NULL; }

	return eventName;
}

void Recorder::Append(PVOID buffer, unsigned int length)
{
	/// Three situations:
	// Audio recording just started, and the event starts with silence. After enough continuous silence, kill it. (Event with no audio)
	// Audio recording has been in progress, and we just received a silent sample. After enough continuous silence, kill it. (End of event)
	// Audio recording has been in progress, no silence. 
	auto isSilent = IsDataSilent((unsigned short*)buffer, length);
	if (!firstSampleReceived && !isSilent) { firstSampleReceived = true; }
	if (isSilent)
	{
		if (firstSampleReceived) 
		{
			if (timeGetTime() - timeLastNonSilentSample > 250)
			{
				Save();
				return;
			}
		} 
		else
		{
			if (timeGetTime() - timeLastNonSilentSample > 3000)
			{
				Save();
				return;
			}
			return; // Don't record silence when at the beginning of an event
		}
	}
	else 
	{
		timeLastNonSilentSample = timeGetTime();
	}
	memcpy(data + cursor, buffer, length);
	cursor += length;
	if (muteSound) 
	{
		memset(buffer, 0, length);
	}
}

bool Recorder::Active()
{
	return active;
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
	active = false;
	firstSampleReceived = false;
}