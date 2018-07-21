#include "main.h"

Ofs* ofs = new Ofs();
[junk_enable /]	
[enc_string_enable /]
std::string Ofs::toHex(DWORD offset) {
    std::stringstream ss;
    ss << std::hex << offset;
    std::string x = ss.str();
    for (auto &c : x) c = toupper(c);

    return x;
}

void Ofs::LoadOffsets() {
	LPCTSTR iniFile = TEXT("C:\\KateBot\\Config.ini");

	m_dwClientState = GetPrivateProfileInt("Offsets","m_dwClientState", 0, iniFile);
	if (m_dwClientState == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_dwClientState"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_dwLocalPlayer = GetPrivateProfileInt("Offsets","m_dwLocalPlayer", 0, iniFile);
	if (m_dwLocalPlayer == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_dwLocalPlayer"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_dwEntityList = GetPrivateProfileInt("Offsets","m_dwEntityList", 0, iniFile);
	if (m_dwEntityList == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_dwEntityList"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_dwGlowObject = GetPrivateProfileInt("Offsets","m_dwGlowObject", 0, iniFile);
	if (m_dwGlowObject == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_dwGlowObject"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_dwForceJump = GetPrivateProfileInt("Offsets","m_dwForceJump", 0, iniFile);
	if (m_dwForceJump == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_dwForceJump"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_dwForceAttack = GetPrivateProfileInt("Offsets","m_dwForceAttack", 0, iniFile);
	if (m_dwForceAttack == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_dwForceAttack"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_dwIndex = GetPrivateProfileInt("Offsets","m_dwIndex", 0, iniFile);
	if (m_dwIndex == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_dwIndex"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_hActiveWeapon = GetPrivateProfileInt("Offsets","m_hActiveWeapon", 0, iniFile);
	if (m_hActiveWeapon == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_hActiveWeapon"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_iCrossHairID = GetPrivateProfileInt("Offsets","m_iCrossHairID", 0, iniFile);
	if (m_iCrossHairID == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_iCrossHairID"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_iHealth = GetPrivateProfileInt("Offsets","m_iHealth", 0, iniFile);
	if (m_iHealth == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_iHealth"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_iTeamNum = GetPrivateProfileInt("Offsets","m_iTeamNum", 0, iniFile);
	if (m_iTeamNum == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_iTeamNum"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_iShotsFired = GetPrivateProfileInt("Offsets","m_iShotsFired", 0, iniFile);
	if (m_iShotsFired == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_iShotsFired"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_bDormant = GetPrivateProfileInt("Offsets","m_bDormant", 0, iniFile);
	if (m_bDormant == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_bDormant"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_fFlags = GetPrivateProfileInt("Offsets","m_fFlags", 0, iniFile);
	if (m_fFlags == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_fFlags"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_lifeState = GetPrivateProfileInt("Offsets","m_lifeState", 0, iniFile);
	if (m_lifeState == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_lifeState"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_bSpottedByMask = GetPrivateProfileInt("Offsets","m_bSpottedByMask", 0, iniFile);
	if (m_bSpottedByMask == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_bSpottedByMask"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_vecOrigin = GetPrivateProfileInt("Offsets","m_vecOrigin", 0, iniFile);
	if (m_vecOrigin == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_vecOrigin"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_vecViewOffset = GetPrivateProfileInt("Offsets","m_vecViewOffset", 0, iniFile);
	if (m_vecViewOffset == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_vecViewOffset"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_angEyeAngles = GetPrivateProfileInt("Offsets","m_angEyeAngles", 0, iniFile);
	if (m_angEyeAngles == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_angEyeAngles"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_dwViewAngles = GetPrivateProfileInt("Offsets","m_dwViewAngles", 0, iniFile);
	if (m_dwViewAngles == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_dwViewAngles"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_vecVelocity = GetPrivateProfileInt("Offsets","m_vecVelocity", 0, iniFile);
	if (m_vecVelocity == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_vecVelocity"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_aimPunchAngle = GetPrivateProfileInt("Offsets","m_aimPunchAngle", 0, iniFile);
	if (m_aimPunchAngle == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_aimPunchAngle"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_dwBoneMatrix = GetPrivateProfileInt("Offsets","m_dwBoneMatrix", 0, iniFile);
	if (m_dwBoneMatrix == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_dwBoneMatrix"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_flNextPrimaryAttack = GetPrivateProfileInt("Offsets","m_flNextPrimaryAttack", 0, iniFile);
	if (m_flNextPrimaryAttack == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_flNextPrimaryAttack"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_nTickBase = GetPrivateProfileInt("Offsets","m_nTickBase", 0, iniFile);
	if (m_nTickBase == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_nTickBase"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_dwGlobalVars = GetPrivateProfileInt("Offsets","m_dwGlobalVars", 0, iniFile);
	if (m_dwGlobalVars == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_dwGlobalVars"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_dwPlayerResource = GetPrivateProfileInt("Offsets","m_dwPlayerResource", 0, iniFile);
	if (m_dwPlayerResource == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_dwPlayerResource"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_iPlayerC4 = GetPrivateProfileInt("Offsets","m_iPlayerC4", 0, iniFile);
	if (m_iPlayerC4 == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_iPlayerC4"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_bIsDefusing = GetPrivateProfileInt("Offsets","m_bIsDefusing", 0, iniFile);
	if (m_bIsDefusing == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_bIsDefusing"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_dwInGame = GetPrivateProfileInt("Offsets","m_dwInGame", 0, iniFile);
	if (m_dwInGame == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_dwInGame"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_iItemDefinitionIndex = GetPrivateProfileInt("Offsets","m_iItemDefinitionIndex", 0, iniFile);
	if (m_iItemDefinitionIndex == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_iItemDefinitionIndex"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_nDeltaTick = GetPrivateProfileInt("Offsets","m_nDeltaTick", 0, iniFile);
	if (m_nDeltaTick == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_nDeltaTick"<<"\n";
	   system("pause");
	   exit(0);
	}
	
	m_iWorldModelIndex = GetPrivateProfileInt("Offsets","m_iWorldModelIndex", 0, iniFile);
	if (m_iWorldModelIndex == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_iWorldModelIndex"<<"\n";
	   system("pause");
	   exit(0);
	}
	m_nModelIndex = GetPrivateProfileInt("Offsets","m_nModelIndex", 0, iniFile);
	if (m_nModelIndex == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_nModelIndex"<<"\n";
	   system("pause");
	   exit(0);
	}
	m_hViewModel = GetPrivateProfileInt("Offsets","m_hViewModel", 0, iniFile);
	if (m_hViewModel == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_hViewModel"<<"\n";
	   system("pause");
	   exit(0);
	}
	m_iAccountID = GetPrivateProfileInt("Offsets","m_iAccountID", 0, iniFile);
	if (m_iAccountID == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_iAccountID"<<"\n";
	   system("pause");
	   exit(0);
	}
	m_nFallbackPaintKit = GetPrivateProfileInt("Offsets","m_nFallbackPaintKit", 0, iniFile);
	if (m_nFallbackPaintKit == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_nFallbackPaintKit"<<"\n";
	   system("pause");
	   exit(0);
	}
	m_OriginalOwnerXuidLow = GetPrivateProfileInt("Offsets","m_OriginalOwnerXuidLow", 0, iniFile);
	if (m_OriginalOwnerXuidLow == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_OriginalOwnerXuidLow"<<"\n";
	   system("pause");
	   exit(0);
	}
	m_iItemIDLow = GetPrivateProfileInt("Offsets","m_iItemIDLow", 0, iniFile);
	if (m_iItemIDLow == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_iItemIDLow"<<"\n";
	   system("pause");
	   exit(0);
	}
	m_hMyWeapons = GetPrivateProfileInt("Offsets","m_hMyWeapons", 0, iniFile);
	if (m_hMyWeapons == 0) {
	   std::cout<<"\n";
	   std::cout<<"Error reading offset: m_hMyWeapons"<<"\n";
	   system("pause");
	   exit(0);
	}
}

void Ofs::getOffsets() {
	LoadOffsets();
	
	m_dwClientState = mem->Read<DWORD_PTR>(engine->GetImage() + m_dwClientState);
}
