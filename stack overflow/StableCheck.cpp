#include "VisibleCheck.h"

#pragma optimize( "", off )
typedef int(__fastcall* UTIL_TraceLine_t)(const VCStructs::Vector&, 
                                          const VCStructs::Vector&, 
										  unsigned int, 
										  const DWORD, 
										  int, 
										  DWORD);
										  
bool WINAPI hkCreateMove( float flInputSampleTime , VCStructs::CUserCmd* pCmd )
{
	VCStructs::hkCreateMoveVars_t* Vars = (VCStructs::hkCreateMoveVars_t*)0x00123456; // hkCreateMove + 0x9 = pointer to Vars; 
																					  // Initialize in InitCreateMoveHook()
	
	DWORD pLocalPlayer 			= *(DWORD*)(Vars->m_dwLocalPlayer);
	bool IsInGame 				= *(int*)(Vars->m_dwClientState + 0x108) == 6; 	// 0x108 = m_dwInGame
	bool IsInForceFullUpdate    = *(int*)(Vars->m_dwClientState + Vars->m_nDeltaTick) == -1;
	UTIL_TraceLine_t TraceLine 	= (UTIL_TraceLine_t)Vars->m_dwUTIL_TraceLine;
	
	if (pCmd->command_number && IsInGame && pLocalPlayer && !IsInForceFullUpdate)
	{
		bool localIsAlive = !*(bool*)(pLocalPlayer + Vars->m_lifeState);
		if (localIsAlive) // Check LocalPlayer IsAlive
		{
			// Get Local Player EyePosition 
			Vars->localOrigin 		 = *(VCStructs::Vector*)(pLocalPlayer + Vars->m_vecOrigin);
			Vars->localViewOffset 	 = *(VCStructs::Vector*)(pLocalPlayer + Vars->m_vecViewOffset);
			Vars->localEyePosition.x = Vars->localOrigin.x + Vars->localViewOffset.x;
			Vars->localEyePosition.y = Vars->localOrigin.y + Vars->localViewOffset.y;
			Vars->localEyePosition.z = Vars->localOrigin.z + Vars->localViewOffset.z;
				
			for (int i = 0; i < 64; i++) {
				Vars->visibleStruct->player[i] = false;
				DWORD pEntity = *(DWORD*)(Vars->m_dwEntityList + i * 0x10);
				
				if (!pEntity)
					continue;
				if (pEntity == pLocalPlayer)
					continue;
					
				bool isDead 	= *(bool*)(pEntity + Vars->m_lifeState);
				bool isDormant 	= *(bool*)(pEntity + Vars->m_bDormant);
				int  health 	= *(int*)(pEntity + Vars->m_iHealth);
				
				if (isDead || isDormant || health <= 0)
					continue;
				
				// Get Enemy Head Position
				DWORD boneMatrix = *(DWORD*)(pEntity + Vars->m_dwBoneMatrix);
				if (!boneMatrix)
					continue;

				Vars->enemyHeadPosition.x = *(float*)(boneMatrix + 0x30 * 8 + 0x0C);
				Vars->enemyHeadPosition.y = *(float*)(boneMatrix + 0x30 * 8 + 0x1C);
				Vars->enemyHeadPosition.z = *(float*)(boneMatrix + 0x30 * 8 + 0x2C);
				
				// Visible check
				TraceLine(Vars->localEyePosition, Vars->enemyHeadPosition, 0x4600400B, pLocalPlayer, 0, Vars->m_dwTraceOutput);
				
				float fraction = *(float*)(Vars->m_dwTraceOutput + 0x2C); // 0x2C - fraction offset in trace_t struct
				if (fraction > Vars->min_fraction)
				{
					Vars->visibleStruct->player[i] = true;
				}
			}
		}
	}

	return true;
}
DWORD WINAPI hkCreateMoveEnd(){return 0;}
#pragma optimize( "", on )

[junk_enable /]

CVisibleCheck::CVisibleCheck(HANDLE hProcess)
{
	hooked = false; this->hProcess = hProcess;
	
	for (int i = 0; i < sizeof(visibleStruct.player); i++) {
		visibleStruct.player[i] = false;
	}
}
	
bool CVisibleCheck::UpdateVisibleStruct()
{
	if (!hooked)
		return false;
	if (!dwVisibleStruct)
		return false;
	if (!ReadProcessMemory(hProcess, (LPVOID)dwVisibleStruct, &visibleStruct, sizeof(visibleStruct_t), NULL))
		return false;
	
	return true;
}
bool CVisibleCheck::IsVisible(int entityId)
{
	if (!hooked)
		return false;
	if (!dwVisibleStruct)
		return false;
	
	return visibleStruct.player[entityId];
}
bool CVisibleCheck::InitCreateMoveHook(
			DWORD m_dwIClientMode, 
			DWORD m_dwUTIL_TraceLine,
			DWORD m_dwLocalPlayer,
			DWORD m_dwEntityList,
			DWORD m_dwClientState,
			DWORD m_nDeltaTick,
			DWORD m_dwBoneMatrix,
			DWORD m_vecViewOffset,
			DWORD m_vecOrigin,
			DWORD m_lifeState,
			DWORD m_iTeamNum,
			DWORD m_iHealth,
			DWORD m_bDormant)
{
	if (hooked)
		return true;
	
	// Check offsets
	if (!m_dwIClientMode)
		return false;
	if (!m_dwUTIL_TraceLine)
		return false;
	if (!m_dwLocalPlayer)
		return false;
	if (!m_dwEntityList)
		return false;
	if (!m_dwClientState)
		return false;
	if (!m_nDeltaTick)
		return false;
	if (!m_dwBoneMatrix)
		return false;
	if (!m_vecViewOffset)
		return false;
	if (!m_vecOrigin)
		return false;
	if (!m_lifeState)
		return false;
	if (!m_iTeamNum)
		return false;
	if (!m_iHealth)
		return false;
	if (!m_bDormant)
		return false;
	
	// Set CreateMove local Vars
	VCStructs::hkCreateMoveVars_t hkCreateMoveVars;
	hkCreateMoveVars.m_dwClientState 		= m_dwClientState;
	hkCreateMoveVars.m_dwEntityList 		= m_dwEntityList;
	hkCreateMoveVars.m_dwLocalPlayer 		= m_dwLocalPlayer;
	hkCreateMoveVars.m_nDeltaTick 			= m_nDeltaTick;
	hkCreateMoveVars.m_dwBoneMatrix 		= m_dwBoneMatrix;
	hkCreateMoveVars.m_vecViewOffset 		= m_vecViewOffset;
	hkCreateMoveVars.m_vecOrigin 			= m_vecOrigin;
	hkCreateMoveVars.m_lifeState 			= m_lifeState;
	hkCreateMoveVars.m_iTeamNum 			= m_iTeamNum;
	hkCreateMoveVars.m_iHealth 				= m_iHealth;
	hkCreateMoveVars.m_bDormant 			= m_bDormant;
	hkCreateMoveVars.m_dwUTIL_TraceLine 	= m_dwUTIL_TraceLine;
	hkCreateMoveVars.min_fraction			= 0.97f;
	
	// Allocate memory for result TraceLine function
	hkCreateMoveVars.m_dwTraceOutput = 
		(DWORD)VirtualAllocEx(hProcess, NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	
	// Allocate memory for visibleStruct
	hkCreateMoveVars.visibleStruct = 
		(visibleStruct_t*)VirtualAllocEx(hProcess, NULL, sizeof(visibleStruct_t), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	
	// Allocate memory for Hook
	LPVOID hookCodeAddress = 
		VirtualAllocEx(hProcess, NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	
	if (!hkCreateMoveVars.m_dwTraceOutput || !hkCreateMoveVars.visibleStruct || !(DWORD)hookCodeAddress)
		return false;
	
	// Write hkCreateMove function in to CSGO
	if (!WriteProcessMemory(hProcess, hookCodeAddress, hkCreateMove, (DWORD)hkCreateMoveEnd - (DWORD)hkCreateMove, NULL))
		return false;
	
	// Write hkCreateMoveVars(Local Variables for CreateMove hook)
	DWORD dw_hkCreateMoveVars = (DWORD)hookCodeAddress + (DWORD)hkCreateMoveEnd - (DWORD)hkCreateMove;
	if (!WriteProcessMemory(hProcess, (LPVOID)dw_hkCreateMoveVars, &hkCreateMoveVars, sizeof(VCStructs::hkCreateMoveVars_t), NULL))
		return false;
	
	// Set pointer to hkCreateMoveVars
	if (!WriteProcessMemory(hProcess, (LPVOID)((DWORD)hookCodeAddress + 0x9), &dw_hkCreateMoveVars, sizeof(DWORD), NULL))
		return false;
	
	Hook(m_dwIClientMode, 24, (DWORD)hookCodeAddress);
	
	dwVisibleStruct = (DWORD)hkCreateMoveVars.visibleStruct;
	hooked = true;
	return hooked;
}

DWORD CVisibleCheck::GetVFunc(DWORD inst, int Index)
{
	DWORD table;
	ReadProcessMemory(hProcess, (LPVOID)inst, &table, sizeof(DWORD), NULL);
	DWORD func = table + sizeof(DWORD) * Index;
	return func;
}

void CVisibleCheck::Hook(DWORD Instance, int Index, DWORD HookFunc)
{
	uintptr_t VFunc = GetVFunc(Instance, Index);
	DWORD dwProtection;
	VirtualProtectEx(hProcess, (LPVOID)VFunc, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &dwProtection);
	WriteProcessMemory(hProcess, (LPVOID)VFunc, &HookFunc, sizeof(DWORD), NULL);
	VirtualProtectEx(hProcess, (LPVOID)VFunc, sizeof(DWORD), dwProtection, &dwProtection);
}