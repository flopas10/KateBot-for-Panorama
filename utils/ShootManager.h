#ifndef _SHOOTMANAGER_H_
#define _SHOOTMANAGER_H_

#include "..\main.h"

class ShootManager
{
public:
	void Start()
	{
		for (;; Sleep(1)) {
			if (cfg->rageModeEnabled)
			{
				Fire();
			} else if (CanShoot()) 
			{
				switch (LocalEntity.GetWeaponType())
				{
					case EWeaponType::WeapType_Rifle:
					case EWeaponType::WeapType_Pistol:
					case EWeaponType::WeapType_SMG:
					case EWeaponType::WeapType_Sniper:
					case EWeaponType::WeapType_Shotgun:
					case EWeaponType::WeapType_LMG:
						if (GetAsyncKeyState(cfg->keys.aimbot_hold)) {
							Sleep(cfg->aimbot[LocalEntity.GetActiveWeapon()].delay);
							while (GetAsyncKeyState(cfg->keys.aimbot_hold)) {
								Fire();
								Sleep(1);
							}
							
							if (!GetAsyncKeyState(cfg->keys.aimbot_hold)) {
								csgo->PressAttackKey();
								Sleep(5);
								csgo->ReleaseAttackKey();
								break;
							}
						}
						break;
					default:
						Fire();
						break;
				}
				
				
				Fire();
			}
		}
	}
	
private:
	bool CanShoot()
	{
		POINT CurPos;
		GetCursorPos(&CurPos);
		int centerx = GetSystemMetrics(SM_CXSCREEN) / 2;
		int centery = GetSystemMetrics(SM_CYSCREEN) / 2;
		int difference = 15;
	 
		if (CurPos.x < centerx - difference || CurPos.x > centerx + difference)
			return false;
		if (CurPos.y < centery - difference || CurPos.y > centery + difference)
			return false; // so do I prevent the shots in the buy menu
	 
		if (!FindWindow(0, "Counter-Strike: Global Offensive")) 
			return false;
	 
		return true;
	}
	
	void Fire()
	{
		INPUT    Input = { 0 };
	 
		if (!GetAsyncKeyState(cfg->keys.aimbot_hold) && GetAsyncKeyState(cfg->InGameFireKey))
		{
			//Release Fire Key
			ZeroMemory(&Input, sizeof(INPUT));
			WORD vkey = cfg->InGameFireKey;
	 
			Input.type = INPUT_KEYBOARD;
			Input.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
			Input.ki.time = 0;
			Input.ki.dwExtraInfo = 0;
			Input.ki.wVk = vkey;
			Input.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE;
			SendInput(1, &Input, sizeof(INPUT));
		}
	 
		if (GetAsyncKeyState(cfg->keys.aimbot_hold) && !GetAsyncKeyState(cfg->InGameFireKey))
		{   
			//Press Fire Key
			ZeroMemory(&Input, sizeof(INPUT));
			WORD vkey = cfg->InGameFireKey;
	 
			Input.type = INPUT_KEYBOARD;
			Input.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
			Input.ki.time = 0;
			Input.ki.dwExtraInfo = 0;
			Input.ki.wVk = vkey;
			Input.ki.dwFlags = KEYEVENTF_SCANCODE;
			SendInput(1, &Input, sizeof(INPUT));
		}
	}
};
#endif
