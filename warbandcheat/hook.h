#pragma once
//https://guidedhacking.com/threads/ezp1z1s-csgo-direct3d9-hooking-esp-tutorial-series.14570/

void Patch(BYTE* dst, BYTE* src, unsigned int size);

bool Hook(char* src, char* dst, int len);

char* TrampHook(char* src, char* dst, unsigned int len);