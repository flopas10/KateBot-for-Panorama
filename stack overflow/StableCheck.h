#pragma once
#include "Main.h"

class CVisibleCheck
{
public:
	CVisibleCheck(HANDLE hProcess);
	
	bool 	InitCreateMoveHook(
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
				DWORD m_bDormant);
	bool 	UpdateVisibleStruct();
	bool 	IsVisible(int entityId);
	
	struct visibleStruct_t
	{
		bool player[64];
	};
private:
	HANDLE 				hProcess;
	DWORD 				dwVisibleStruct;
	visibleStruct_t 	visibleStruct;
	bool 				hooked;
	
	DWORD 	GetVFunc(DWORD inst, int Index);
	void 	Hook(DWORD Instance, int Index, DWORD HookFunc);
};

extern CVisibleCheck* VisibleCheck;

namespace VCStructs
{
	struct Vector
	{
		float x, y, z;
	};
	
	struct hkCreateMoveVars_t
	{
		// Offsets
		DWORD m_dwUTIL_TraceLine;
		DWORD m_dwClientState;
		DWORD m_dwEntityList;
		DWORD m_dwLocalPlayer;
		DWORD m_nDeltaTick;
		DWORD m_dwBoneMatrix;
		DWORD m_vecViewOffset;
		DWORD m_vecOrigin;
		DWORD m_lifeState;
		DWORD m_iTeamNum;
		DWORD m_iHealth;
		DWORD m_bDormant;
		DWORD m_dwTraceOutput; // Output trace_t struct
		CVisibleCheck::visibleStruct_t* visibleStruct; // Player's visible struct
		
		// Local variables
		Vector localOrigin;
		Vector localViewOffset;
		Vector localEyePosition;
		Vector enemyHeadPosition;
		float min_fraction;
	};
	
	class CUserCmd
	{
	public:
		virtual ~CUserCmd( ) { };
		int		command_number;
		int		tick_count;
		Vector	viewangles;
		Vector	aimdirection;
		float	forwardmove;
		float	sidemove;
		float	upmove;
		int		buttons;
		BYTE    impulse;
		int		weaponselect;
		int		weaponsubtype;
		int		random_seed;
		short	mousedx;
		short	mousedy;
		bool	hasbeenpredicted;
		char	pad_0x4C[0x18];
	};
}