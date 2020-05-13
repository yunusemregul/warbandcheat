#pragma once
//https://guidedhacking.com/threads/ezp1z1s-csgo-direct3d9-hooking-esp-tutorial-series.14570/

typedef HRESULT(APIENTRY* tEndScene)(LPDIRECT3DDEVICE9 pDevice);

bool GetD3D9Device(void** pTable, size_t size);

static HWND window;

HWND GetProcessWindow();

extern int windowHeight, windowWidth;

extern LPDIRECT3DDEVICE9 pDevice;