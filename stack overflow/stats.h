#pragma once

class Ofs {
public:
	[swap_lines]
	DWORD m_dwMapDirectory = 0x188;
	DWORD m_nDeltaTick;
	DWORD m_iWorldModelIndex;
	DWORD m_nModelIndex;
	DWORD m_hViewModel;
	DWORD m_iAccountID;
	DWORD m_nFallbackPaintKit;
	DWORD m_OriginalOwnerXuidLow;
	DWORD m_iItemIDLow;
	DWORD m_hMyWeapons;
	DWORD m_bIsDefusing;
	DWORD m_iPlayerC4;
	DWORD m_dwPlayerResource;
	DWORD m_vecVelocity;
	DWORD m_aimPunchAngle;
	DWORD m_nTickBase;
	DWORD m_dwLocalPlayer;
	DWORD m_dwEntityList;
	DWORD m_dwForceAttack;
	DWORD m_dwClientState;
	DWORD m_dwGlowObject;
	DWORD m_dwForceJump;
	DWORD m_dwGlobalVars;
	DWORD m_hActiveWeapon;
	DWORD m_iCrossHairID;
	DWORD m_dwIndex;
	DWORD m_iHealth;
	DWORD m_iTeamNum;
	DWORD m_iShotsFired;
	DWORD m_bDormant;
	DWORD m_fFlags;
	DWORD m_bSpottedByMask;
	DWORD m_dwInGame;
	DWORD m_lifeState;
	DWORD m_vecOrigin;
	DWORD m_vecViewOffset;
	DWORD m_dwBoneMatrix;
	DWORD m_angEyeAngles;
	DWORD m_dwViewAngles;
	DWORD m_flNextPrimaryAttack;
	DWORD m_iItemDefinitionIndex;
	DWORD m_iClip1;
	[/swap_lines]

	std::string toHex(DWORD offset);
	void getOffsets();
private:
	void LoadOffsets();
};

extern Ofs* ofs;