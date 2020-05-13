#define _CRT_SECURE_NO_WARNINGS

#include "includes.h"

struct vec2
{
	float x;
	float y;
};

#include "Warband.h"

void* d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene oEndScene = nullptr;
extern LPDIRECT3DDEVICE9 pDevice = nullptr;

// void __thiscall get_screen_pos_from_world_pos_maybe(int this,float *param_1_00,undefined4 param_3)

typedef float*(__thiscall* get_screen_pos_from_world_pos_t)(void* t, float *vec3, float *vec32);
get_screen_pos_from_world_pos_t get_screen_pos_from_world_pos;

typedef bool (__thiscall* is_enemy_t)(Agent* t, Agent* tr);
is_enemy_t is_enemy;

typedef int(__thiscall* get_immediate_enemy_t)(Agent* t);
get_immediate_enemy_t get_immediate_enemy;

typedef int(__thiscall* player_attack_dir_t)(Agent* t, int id);
player_attack_dir_t player_attack_dir;

typedef int(__thiscall* get_attack_dir_t)(Agent* t, int id);
get_attack_dir_t get_attack_dir;

void word_to_screen(D3DXVECTOR3 world_pos, float *output)
{
	DWORD tactical_window = 0xdd9b18;
	Tactical_Window* tw = (Tactical_Window*)(*(uintptr_t*)tactical_window);
	float *screen_pos = get_screen_pos_from_world_pos((void*)(tw->pScene_widget), world_pos, world_pos);

	if (screen_pos[0] < 0 || screen_pos[0]>1 || screen_pos[1] < 0 || screen_pos[1]>1)
		return;

	output[0] = screen_pos[0] * windowWidth;
	output[1] = windowHeight - windowHeight * (screen_pos[1] * 1.33);
}

// hook function
void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
	if (!pDevice)
		pDevice = o_pDevice;

	DWORD cur_mission = 0x8b829c;
	DWORD tactical_window = 0xdd9b18;
	Tactical_Window* tw = (Tactical_Window*)(*(uintptr_t*)tactical_window);

	DrawText("CHEAT RUNNING", windowWidth/2, 5, D3DCOLOR_ARGB(255, 0, 255, 0));

	Agent* localAgent = getLocalAgent();

	size_t agentlist_size = *(size_t*)(*(uintptr_t*)cur_mission + 0x8);
	for (int i = 0; i < agentlist_size; i++)
	{
		Agent* agent = getAgent(i);

		if (localAgent == nullptr || agent == nullptr)
			continue;

		if (agent != localAgent && agent->health>0)
		{
			D3DXVECTOR3 pos = agent->position;
			float agent_screen_pos[2] = { -1 };
			word_to_screen(pos, agent_screen_pos);

			if (agent_screen_pos[0]==-1)
				continue;

			rglStrategic_Entity* strat_ent = agent->prgl_strat_ent;
			if (strat_ent != nullptr)
			{
				if (strat_ent->ent_min != nullptr && strat_ent->ent_max!=nullptr)
				{
					D3DXVECTOR3 min = strat_ent->ent_min;
					D3DXVECTOR3 max = strat_ent->ent_max;
						
					float mins[2] = {-1};
					word_to_screen(min, mins);
					float maxs[2] = {-1};
					word_to_screen(max, maxs);

					float width = fabs(mins[1] - maxs[1])/4;

					if (mins[0]!=-1 && maxs[0]!=-1)
					{
						D3DCOLOR color = D3DCOLOR_ARGB(255, 0, 255, 0);
						if (is_enemy(localAgent, agent))
						{
							color = D3DCOLOR_ARGB(255, 255, 0, 0);
							if (get_immediate_enemy(localAgent) == agent->index)
							{
								color = D3DCOLOR_ARGB(255, 255, 0, 255);
								/*int dir = player_attack_dir(agent, localAgent->index);
								std::stringstream st;
								if (dir == 3)
									st << "top";
								if (dir == 2)
									st << "left";
								if (dir == 1)
									st << "right";
								if (dir == 0)
									st << "bottom";
								std::string res(st.str());
								DrawText(res.c_str(), agent_screen_pos[0], agent_screen_pos[1], D3DCOLOR_ARGB(255, 0, 255, 0));*/
							}
							DrawLine(windowWidth / 2, windowHeight, agent_screen_pos[0], mins[1], 1, color);
						}
						if (agent->is_horse == 1)
							color = D3DCOLOR_ARGB(255, 0, 0, 255);

						DrawLine(agent_screen_pos[0]-width, mins[1], agent_screen_pos[0]+width, mins[1], 1, color);
						DrawLine(agent_screen_pos[0] - width, maxs[1], agent_screen_pos[0] + width, maxs[1], 1, color);
						DrawLine(agent_screen_pos[0] - width, mins[1], agent_screen_pos[0] - width, maxs[1], 1, color);
						DrawLine(agent_screen_pos[0] + width, mins[1], agent_screen_pos[0] + width, maxs[1], 1, color);


						float val = (agent->health / agent->max_health);
						int h = val * (width*4);

						DrawFilledRect(agent_screen_pos[0] + width, mins[1]-width*4, max(width / 4,2), width*4, D3DCOLOR_ARGB(255, 33, 33, 33));
						DrawFilledRect(agent_screen_pos[0] + width, mins[1]-h, max(width / 4,2), h, D3DCOLOR_ARGB(255, (int)((1 - val) * 255), (int)(val * 255), 0));
						
						std::stringstream st;
						st << std::hex << agent;
						std::string res(st.str());
						DrawText(res.c_str(), agent_screen_pos[0], agent_screen_pos[1], D3DCOLOR_ARGB(255, 0, 255, 0));
					}
				}
			}
		}
	}

	if (localAgent != nullptr)
	{
		std::stringstream st;
		st << std::hex << localAgent;
		std::string res(st.str());
		DrawText(res.c_str(), 25, 25, D3DCOLOR_ARGB(255, 0, 255, 0));
	}

	// call og function
	oEndScene(pDevice);
}

DWORD WINAPI MainThread(HMODULE hModule)
{
	//AllocConsole();
	//freopen("CONOUT$", "w", stdout);

	HANDLE handle = GetCurrentProcess();

	DWORD base = (uintptr_t)GetModuleHandle(NULL);
	get_screen_pos_from_world_pos = (get_screen_pos_from_world_pos_t)(0x53eef0);
	is_enemy = (is_enemy_t)(0x4adcc0);
	get_immediate_enemy = (get_immediate_enemy_t)(0x4afc40);
	player_attack_dir = (player_attack_dir_t)(0x4b00d0);
	get_attack_dir = (get_attack_dir_t)(0x4b00d0);

	// hook
	if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {
		memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);

		oEndScene = (tEndScene)TrampHook((char*)d3d9Device[42], (char*)hkEndScene, 7);
	}

	while (!GetAsyncKeyState(VK_END) & 1)
	{
		Sleep(500);
	}

	// unhook d3d9
	Patch((BYTE*)d3d9Device[42], EndSceneBytes, 7);

	//FreeConsole();
	FreeLibraryAndExitThread(hModule, NULL);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		//DisableThreadLibraryCalls(hModule);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread, hModule, NULL, NULL);
	}

    return TRUE;
}

