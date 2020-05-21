#include "includes.h"
//https://guidedhacking.com/threads/ezp1z1s-csgo-direct3d9-hooking-esp-tutorial-series.14570/

ID3DXLine* LineL;
ID3DXFont* FontF;

void onLostDevice()
{
	if(LineL)
		LineL->OnLostDevice();
	if(FontF)
		FontF->OnLostDevice();
}

void onResetDevice()
{
	if(LineL)
		LineL->OnResetDevice();
	if (FontF)
		FontF->OnResetDevice();
}

// filled rectangle
void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color) {
	if (!pDevice)
		return;

	D3DRECT rect = { x,y,x + w,y + h };
	pDevice->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}

void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color)
{
	if (!pDevice)
		return;

	if (!LineL)
	{
		D3DXCreateLine(pDevice, &LineL);
		cout << "> Line not present so created it." << endl;
	}

	D3DXVECTOR2 Line[2];
	Line[0] = D3DXVECTOR2(x1, y1);
	Line[1] = D3DXVECTOR2(x2, y2);
	LineL->SetWidth(thickness);
	LineL->Draw(Line, 2, color);
}

void DrawText(const char* text, int x, int y, D3DCOLOR color)
{
	if (!pDevice)
		return;

	RECT rect;

	if (!FontF)
	{
		D3DXCreateFont(pDevice, 14, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &FontF);
		cout << "> Font not present so created it." << endl;
	}
	
	SetRect(&rect, x + 1, y + 1, x + 1, y + 1);
	FontF->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));

	SetRect(&rect, x, y, x, y);
	FontF->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
}