#pragma once

class Tactical_Window
{
public:
	char pad_0000[336]; //0x0000
	void* pScene_widget; //0x0150
	char pad_0154[584]; //0x0154
	float camerax;
	float cameray;
	float cameraz;
	char pad_03A8[4]; //0x03A8
	float* camera_angle; //0x03AC
}; //Size: 0x03B8