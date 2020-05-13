#include "includes.h"
//https://guidedhacking.com/threads/ezp1z1s-csgo-direct3d9-hooking-esp-tutorial-series.14570/

// window height and width
int windowHeight, windowWidth;

// callback to be called for each toplevel window
BOOL CALLBACK enumWind(HWND handle, LPARAM lp) {
	DWORD procId;
	GetWindowThreadProcessId(handle, &procId);
	if (GetCurrentProcessId() != procId)
		return TRUE;

	std::cout << GetCurrentProcessId() << std::endl;

	window = handle;
	return FALSE;
}

// get process window
HWND GetProcessWindow() {
	window = GetForegroundWindow();

	//EnumWindows(enumWind, NULL);

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

	std::cout << "first" << std::endl;
	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!pD3D)
		return false;

	std::cout << "second" << std::endl;

	IDirect3DDevice9* pDummyDevice = nullptr;

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = false;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetProcessWindow();
	
	char baslik[120];
	GetWindowTextA(window, baslik, 120);

	std::cout << baslik << std::endl;
	std::cout << windowWidth << ", " << windowHeight << std::endl;

	HRESULT dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

	if (dummyDevCreated != S_OK) {
		std::cout << "not ok" << std::endl;
		d3dpp.Windowed = !d3dpp.Windowed;
		HRESULT dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

		if (dummyDevCreated != S_OK) {
			std::cout << "not ok again" << std::endl;
			pD3D->Release();
			return false;
		}
	}

	std::cout << "memcpy" << std::endl;

	memcpy(pTable, *(void***)(pDummyDevice), size);
	pDummyDevice->Release();
	pD3D->Release();
	return true;
}