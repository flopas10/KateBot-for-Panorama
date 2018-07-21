#include "..\main.h"

Engine* csgo = new Engine();

Engine::Engine()
{}

Engine::~Engine()
{}
[junk_enable /]	
[enc_string_enable /]
bool Engine::IsConnected()
{
	if( !ofs->m_dwClientState )
		return false;
	return ( mem->Read<int>( ofs->m_dwClientState + ofs->m_dwInGame) == ( int )SIGNONSTATE_CONNECTED );
}

bool Engine::IsInGame()
{
	if( !ofs->m_dwClientState )
		return false;
	return ( mem->Read<int>( ofs->m_dwClientState + ofs->m_dwInGame) == ( int )SIGNONSTATE_FULL );
}

bool Engine::IsTeamSelected()
{
	return ( LocalEntity.GetTeamNum() == 2 || LocalEntity.GetTeamNum() == 3 );
}

int Engine::GetLocalPlayer()
{
	if( !ofs->m_dwClientState )
		return -1;
	return mem->Read<int>( ofs->m_dwClientState + 0x180 ); //m_dwLocalPlayerIndex
}

int Engine::GetMaxObjects()
{
	return mem->Read<int>(client->GetImage() + ofs->m_dwGlowObject + 0x4);
}

void Engine::GetViewAngles( Vector& viewangles )
{
	if( !ofs->m_dwClientState )
		return;
	viewangles = mem->Read<Vector>( ofs->m_dwClientState + ofs->m_dwViewAngles );
}

void Engine::SetViewAngles( const Vector& viewangles )
{
	if( !ofs->m_dwClientState || viewangles.IsZero() || !viewangles.IsTrue())
		return;
	mem->Write<Vector>( ofs->m_dwClientState + ofs->m_dwViewAngles, viewangles );
}

Vector Engine::RetViewAngles()
{
	if( !ofs->m_dwClientState )
		return { 0, 0, 0 };
	return mem->Read<Vector>( ofs->m_dwClientState + ofs->m_dwViewAngles );
}


// Played around with this,
// -Works 100% for low fov, so can be used for a legit bot
// -Works 100% client side, good for raging
// -Full FOV does not fully work demo/spec side, twitches at times because 
//		we're not inside the same thread with the same timings
// 
//  I feel like I've broken this :/
void Engine::SetViewAnglesPSilent( Vector& viewangles )
{
	/*if( !g_pAimbot->IsAbleToShoot() )
		return;

	int userCMDSequenceNum = 0;
	Vector oldAngles;
	DWORD_PTR userCMD = ofs->userCMD;

	int curSequenceNum = mem->Read<int>( ofs->m_dwClientState + 0x4CA8 ) + 1;
	userCMD += ( curSequenceNum % 150 ) * 0x64;

	SetSendPacket( false );

	while( userCMDSequenceNum != curSequenceNum )
	{
		g_pEngine->GetViewAngles( oldAngles );
		userCMDSequenceNum = mem->Read<int>( userCMD + 0x4 );
	}

	g_pMath->ClampAngle( viewangles ); // Just too make sure!!

	for( int i = 0; i < 20; i++ )  // Called outside of a hook so just keep setting it untill we hit the timing
	{
		mem->Write<Vector>( userCMD + 0xC, viewangles );
	}

	g_pEngine->SetViewAngles( oldAngles ); // Set view angles asap 
	Sleep( 6 );	// Changed from 7 to 5, could be the reason for shooting twice?

	SetSendPacket( true ); */
}


const char* Engine::GetMapDirectory()
{
	if( !ofs->m_dwClientState )
		return nullptr;
	mem->Read( ofs->m_dwClientState + ofs->m_dwMapDirectory, m_pMapDirectory, sizeof( char[ 255 ] ) );
	return m_pMapDirectory;
}

std::string Engine::GetGameDirectory()
{
	char filename[MAX_PATH];
	if (GetModuleFileNameEx(mem->m_hProcess, NULL, filename, MAX_PATH) == 0) {
		return nullptr;
	}
	
	std::string tmp_path(filename);
	int pos = tmp_path.find("csgo");
	tmp_path = tmp_path.substr(0, pos);
	tmp_path = tmp_path + "csgo\\";
	return tmp_path;
}


void Engine::GlobalsSetup()
{
	m_dwGlobals = mem->Read< DWORD_PTR >( client->GetImage() + ofs->m_dwGlobalVars );
}

float Engine::GetCurTime()
{
	return mem->Read<float>( m_dwGlobals + 0x10 );
}

float Engine::GetFrameTime()
{
	return mem->Read<float>( m_dwGlobals + 0x14 );
}

int	Engine::GetMaxClients()
{
	return mem->Read<int>( m_dwGlobals + 0x18 );
}

float Engine::GetIntervalPerTick()
{
	return mem->Read<float>( m_dwGlobals + 0x20 );
}

float Engine::GetInterpolationAmount()
{
	return mem->Read<float>( m_dwGlobals + 0x24 );
}


int Engine::GetClassID(DWORD adr)
{

	int vt = mem->Read<int>(adr + 0x8);	// Vtable
	int fn = mem->Read<int>(vt + 2 * 0x4);	// Function
	int cls = mem->Read<int>(fn + 0x1);	// Class
	int clsn = mem->Read<int>(cls + 8);	// Class Name
	return mem->Read<int>(cls + 20);	// ClassID
}

char* Engine::GetClassNameFromPlayer(DWORD adr)
{
	int vt = mem->Read<int>(adr + 0x8);
	int fn = mem->Read<int>(vt + 2 * 0x4);
	int cls = mem->Read<int>(fn + 0x1);
	int clsn = mem->Read<int>(cls + 8);

	int namePointer = mem->Read<int>(clsn);
	char* nameData = new char[32];

	for (int i = 0; i < 32; i++) {
		nameData[i] = mem->Read<char>(namePointer + i);
	}
	
	return nameData;
}
[junk_disable /]	

bool Engine::IsClassIDAWeapon(int iClassID)
{
	switch (iClassID)
	{
		case CID_CAK47:
		case CID_CDEagle:
		case CID_CWeaponAWP:
		case CID_CWeaponAug:
		case CID_CWeaponBizon:
		case CID_CWeaponElite:
		case CID_CWeaponFamas:
		case CID_CWeaponFiveSeven:
		case CID_CWeaponG3SG1:
		case CID_CWeaponGalilAR:
		case CID_CWeaponGlock:
		case CID_CWeaponHKP2000:
		case CID_CWeaponMAC10:
		case CID_CWeaponM249:
		case CID_CWeaponM4A1:
		case CID_CWeaponM3:
		case CID_CWeaponMag7:
		case CID_CWeaponMP7:
		case CID_CWeaponMP9:
		case CID_CWeaponNegev:
		case CID_CWeaponNOVA:
		case CID_CWeaponP250:
		case CID_CWeaponP90:
		case CID_CWeaponSawedoff:
		case CID_CWeaponSCAR20:
		case CID_CWeaponSG556:
		case CID_CWeaponSSG08:
		case CID_CWeaponTaser:
		case CID_CWeaponTec9:
		case CID_CWeaponUSP:
		case CID_CWeaponUMP45:
		case CID_CWeaponXM1014:


		return true;
	}

	return false;
}

bool Engine::IsClassIDAGrenade(int iClassID)
{
	switch (iClassID)
	{
		case CID_CFlashbang:
		case CID_CHEGrenade:
		case CID_CDecoyGrenade:
		case CID_CIncendiaryGrenade:
		case CID_CMolotovGrenade:
		case CID_CSmokeGrenade:
		case CID_CBaseCSGrenadeProjectile:
		case CID_CSmokeGrenadeProjectile:
		case CID_CMolotovProjectile:
		case CID_CDecoyProjectile:
		return true;
	}

	return false;
}
[junk_enable /]	

void Engine::PressAttackKey()
{
	if (!GetAsyncKeyState(cfg->InGameFireKey))
	{   
		INPUT    Input = { 0 };
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

void Engine::ReleaseAttackKey()
{
	if (GetAsyncKeyState(cfg->InGameFireKey))
	{
		INPUT    Input = { 0 };
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
}

void Engine::PAttack()
{
	if (!mem->Read<bool>( client->GetImage() + ofs->m_dwForceAttack ))
		mem->Write<bool>( client->GetImage() + ofs->m_dwForceAttack, true );
}

void Engine::MAttack()
{
	if (mem->Read<bool>( client->GetImage() + ofs->m_dwForceAttack ))
		mem->Write<bool>( client->GetImage() + ofs->m_dwForceAttack, false );
}

void Engine::ForceFullUpdate()
{
	if( !ofs->m_dwClientState )
		return;
		
	mem->Write<int>(ofs->m_dwClientState + ofs->m_nDeltaTick, -1);
}
