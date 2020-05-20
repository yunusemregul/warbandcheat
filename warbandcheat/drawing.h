#pragma once
//https://guidedhacking.com/threads/ezp1z1s-csgo-direct3d9-hooking-esp-tutorial-series.14570/

void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR col);
void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color);
void DrawText(const char* text, int x, int y, D3DCOLOR color);

void onLostDevice();
void onResetDevice();