#pragma once

struct unk {
	__int64* sound_function = 0;
	LPCWSTR* endpointID = 0;
	INT32 channel_count = 0;
	INT32 maybe_sample_rate = 0;
	WORD field18 = 0;
	WORD field1A = 0;
	__int64 hrtf_buffer = 0;
};

__int64 hook_GET_AUDIO_BUFFER_AND_SET_SIZE(__int64* a1, byte** BUFFER, int size);
bool valid_data(byte* buffer, int size);
void dump_data();
__int64 hook_TRANSFER_MIXED_AUDIO_TO_SOUND_BUFFER(__int64* a1);
int main(int argc, char* argv[]);