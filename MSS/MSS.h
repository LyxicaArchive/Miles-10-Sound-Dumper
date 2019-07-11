#pragma once

__int64 hook_GET_AUDIO_BUFFER_AND_SET_SIZE(__int64* a1, byte** BUFFER, int size);
bool valid_data(byte* buffer, int size);
void dump_data();
__int64 hook_TRANSFER_MIXED_AUDIO_TO_SOUND_BUFFER(__int64* a1);
void WINAPI logM(int number, char* message);
void print_bus_volumes(__int64 driver);
void full_all_buses(__int64 driver);
void print_source_names(__int64 bank);
int main();

typedef __int64* Driver;
typedef __int64* Bank;

