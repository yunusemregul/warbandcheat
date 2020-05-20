#include "includes.h"
//https://guidedhacking.com/threads/ezp1z1s-csgo-direct3d9-hooking-esp-tutorial-series.14570/

// window height and width
int windowHeight, windowWidth;

// get process window
HWND GetProcessWindow() {
	window = FindWindow(NULL, "Mount&Blade Warband");

	RECT size;
	GetWindowRect(window, &size);
	windowWidth = size.right - size.left;
	windowHeight = size.bottom - size.top;

	windowHeight -= 29;
	windowWidth -= 5;

	return window;
}

// get get vTable
bool GetD3D9Device(void** pTable, size_t size) {
	if (!pTable)
		return false;

	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!pD3D)
		return false;

	IDirect3DDevice9* pDummyDevice = nullptr;

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = false;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetProcessWindow();

	HRESULT dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

	if (dummyDevCreated != S_OK) {
		d3dpp.Windowed = !d3dpp.Windowed;
		HRESULT dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

		if (dummyDevCreated != S_OK) {
			cout << "* Can't create dummy device!" << endl;
			pD3D->Release();
			return false;
		}
	}

	memcpy(pTable, *(void***)(pDummyDevice), size);
	pDummyDevice->Release();
	pD3D->Release();
	cout << "* Successfully created dummy D3D9 device!" << endl;
	return true;
}