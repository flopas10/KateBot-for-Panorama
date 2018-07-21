#include "..\main.h"

Entity LocalEntity;
Entity EntityList[ 65 ];
[junk_enable /]	
[enc_string_enable /]
Entity::Entity()
{
	m_iIndex = NULL;

	m_dwPointer = NULL;
	m_dwBoneMatrix = NULL;

	m_iTeamNum = NULL;
	m_iHealth = NULL;
	m_iFlags = NULL;


	m_bDead = true;
}

Entity::~Entity()
{

}

void Entity::Update( int iIndex )
{
	//Clear();
	m_iIndex = iIndex;

	m_dwPointer = mem->Read<DWORD_PTR>(client->GetImage() + ofs->m_dwEntityList + m_iIndex * 0x10); 

	if( !m_dwPointer )
		return;

	//    Read Player Chunk    //
	mem->Read(m_dwPointer, chunk, sizeof(chunk));
	//	End Read Player Chunk //

	
	m_dwIndex = *reinterpret_cast< int * >( chunk + ofs->m_dwIndex );
	m_iHealth = *reinterpret_cast< int * >( chunk + ofs->m_iHealth );

	m_dwBoneMatrix		= *reinterpret_cast< DWORD_PTR * >( chunk + ofs->m_dwBoneMatrix );

	if( !m_dwBoneMatrix )
		return;

	mem->Read( m_dwBoneMatrix, m_BoneMatrix, sizeof( Matrix3x4[ 128 ] ) );
	
	m_vOrigin				= *reinterpret_cast< Vector * >( chunk + ofs->m_vecOrigin );
	m_vEye					= m_vOrigin + *reinterpret_cast< Vector * >( chunk + ofs->m_vecViewOffset );
	m_vEyeAngles			= *reinterpret_cast< Vector * >( chunk + ofs->m_angEyeAngles );
	m_vVelocity				= *reinterpret_cast< Vector * >( chunk + ofs->m_vecVelocity );
	m_vPunch				= *reinterpret_cast< Vector * >( chunk + ofs->m_aimPunchAngle );
	m_iTeamNum				= *reinterpret_cast< int * >( chunk + ofs->m_iTeamNum );
	m_iFlags				= *reinterpret_cast< int * >( chunk + ofs->m_fFlags );
	m_iCrossHairID			= *reinterpret_cast< int * >( chunk + ofs->m_iCrossHairID );				
	m_hActiveWeapon			= *reinterpret_cast< int * >( chunk + ofs->m_hActiveWeapon );
	m_iShotsFired			= *reinterpret_cast< int * >( chunk + ofs->m_iShotsFired );
	m_bDead					= *reinterpret_cast< bool * >( chunk + ofs->m_lifeState );
	m_bIsDormant			= *reinterpret_cast< bool * >( chunk + ofs->m_bDormant );
	m_i64SpottedByMask		= *reinterpret_cast< INT64 * >( chunk + ofs->m_bSpottedByMask );
	
	if (!cfg->bspParsing)
		CheckVisible();
}

DWORD_PTR Entity::GetPointer()
{
	return m_dwPointer;
}

DWORD_PTR Entity::GetLocalPointer()
{
	return m_dwLocalPointer;
}

DWORD_PTR Entity::GetBoneMatrix()
{
	return m_dwBoneMatrix;
}

Matrix3x4 Entity::GetBoneMatrix( int iBone )
{
	return m_BoneMatrix[ iBone ];
}

Vector Entity::GetPunchAngles()
{
	return m_vPunch;
}

Vector Entity::GetOrigin()
{
	return m_vOrigin;
}

Vector Entity::GetEyeAngles()
{
	return m_vEyeAngles;
}

Vector Entity::GetEyePosition()
{
	return m_vEye;
}

Vector Entity::GetVelocity()
{
	return m_vVelocity;
}

Vector Entity::GetBonePosition( int iBone )
{
	return Vector( m_BoneMatrix[ iBone ].m[ 0 ][ 3 ], m_BoneMatrix[ iBone ].m[ 1 ][ 3 ], m_BoneMatrix[ iBone ].m[ 2 ][ 3 ] );
}

int Entity::GetActiveWeapon()
{
	int weaponId = mem->Read<int>( GetActiveWeaponBase() + ofs->m_iItemDefinitionIndex );
	if( weaponId > 64 || weaponId < 0 ) // If number is greater than array size return 0 to prevent crash
		weaponId = 42;	// set weapon ID to knife

	return weaponId;
}

DWORD_PTR Entity::GetActiveWeaponBase()
{ 
	DWORD_PTR dwBaseCombatWeaponIndex = *reinterpret_cast< DWORD_PTR * >( chunk + ofs->m_hActiveWeapon );
	dwBaseCombatWeaponIndex &= 0xFFF;
	return mem->Read<DWORD_PTR>( client->GetImage() + ofs->m_dwEntityList + (dwBaseCombatWeaponIndex - 1) * 0x10 );
}

float Entity::GetAccuracyPenalty()
{
	return mem->Read<float>( GetActiveWeaponBase() + 0x32C0 );
}

bool Entity::IsActiveWeaponNotVaild()
{
	int WeaponId = GetActiveWeapon();

	return( WeaponId == WID_KnifeGG || WeaponId == WID_Knife || WeaponId == WID_Flashbang || WeaponId == WID_HEFrag || WeaponId == WID_Smoke || WeaponId == WID_Molly || WeaponId == WID_Decoy || WeaponId == WID_Firebomb || WeaponId == WID_C4 );
}

int Entity::GetActiveWeaponAmmo()
{
	DWORD_PTR WeaponBase = GetActiveWeaponBase();

	return mem->Read<int>( WeaponBase + ofs->m_iClip1 );
}

bool Entity::IsActiveWeaponPistol()
{
	int WeaponId = GetActiveWeapon();

	return( WeaponId == WID_Deagle || WeaponId == WID_Revolver || WeaponId == WID_Dual_Berettas || WeaponId == WID_Five_Seven || WeaponId == WID_Glock || WeaponId == WID_P250 || WeaponId == WID_USP || WeaponId == WID_Tec9 || WeaponId == WID_Zeus || WeaponId == WID_P2000 );
}

EWeaponType Entity::GetWeaponType()
{
	int id = GetActiveWeapon();
[junk_disable /]	
	switch (id)
	{
		case WID_C4:
		return EWeaponType::WeapType_C4Explosive;

		case WID_Zeus:
		return EWeaponType::WeapType_ZeusGun;

		case WID_Negev:
		case WID_M249:
		return EWeaponType::WeapType_LMG;

		case WID_AWP:
		case WID_G3SG1_Auto:
		case WID_SCAR_Auto:
		case WID_Scout:
		return EWeaponType::WeapType_Sniper;

		case WID_XM1014:
		case WID_MAG7:
		case WID_Nova:
		case WID_SawedOff:
		return EWeaponType::WeapType_Shotgun;

		case WID_Flashbang:
		case WID_Smoke:
		case WID_Firebomb:
		case WID_HEFrag:
		case WID_Molly:
		return EWeaponType::WeapType_Grenade;

		case WID_MAC10:
		case WID_P90:
		case WID_UMP45:
		case WID_PPBizon:
		case WID_MP7:
		case WID_MP9:
		return EWeaponType::WeapType_SMG;

		case WID_Deagle:
		case WID_Dual_Berettas:
		case WID_Five_Seven:
		case WID_USP:
		case WID_Glock:
		case WID_Tec9:
		case WID_P2000:
		case WID_P250:
		case WID_CZ75:
		case WID_Revolver:
		return EWeaponType::WeapType_Pistol;

		case WID_AK47:
		case WID_AUG:
		case WID_FAMAS:
		case WID_M4A4:
		case WID_M4A1S:
		case WID_Galil:
		case WID_SG553:
		return EWeaponType::WeapType_Rifle;

		case WID_Default_Knife:
		case WID_ButterflyKnife:
		case WID_FlipKnife:
		case WID_HuntsmanKnife:
		case WID_M9BayonetKnife:
		case WID_KarambitKnife:
		case WID_FalchionKnife:
		case WID_ShadowDaggerKnife:
		case WID_BayonetKnife:
		case WID_GutKnife:
		case WID_BowieKnife:
		return EWeaponType::WeapType_KnifeType;

		default:
		return (EWeaponType)-1;
	}
}
[junk_enable /]	
int Entity::GetTeamNum()
{
	return m_iTeamNum;
}

int Entity::GetHealth()
{
	return m_iHealth;
}

int Entity::GetCrosshairEnt()
{
	return m_iCrossHairID;
}

int Entity::GetShotsFired()
{
	return m_iShotsFired;
}

int Entity::GetIndex()
{
	return m_dwIndex;
}

int Entity::GetFlags()
{
	return m_iFlags;
}

bool Entity::IsValid()
{
	if( !GetPointer() )
		return false;

	if( !GetBoneMatrix() )
		return false;

	if( !GetTeamNum() )
		return false;

	if( IsDead() )
		return false;

	if( IsDormant() )
		return false;

	if( GetOrigin().IsZero() )
		return false;

	if( GetHealth() < 1 )
		return false;

	return true;
}

bool Entity::IsPlayer()
{
	if( !GetPointer() )
		return false;

	if( !GetTeamNum() )
		return false;

	return true;
}

bool Entity::IsDead()
{
	return m_bDead;
}

bool Entity::IsDormant()
{
	return m_bIsDormant;
}

bool Entity::IsJumping()
{
	int fhh = m_iFlags;
	if (fhh & 0x1 == 1 || fhh & 0x1 == 3 || fhh & 0x1 == 5 || fhh  & 0x1 == 7)
	{
		return false;
	}
	return true;
}

bool Entity::IsSpottedByMask()
{
	return ( m_i64SpottedByMask & ( 1 << csgo->GetLocalPlayer() ) );
}

void Entity::CheckVisible()
{	
	m_bVisible = IsSpottedByMask();
}

void Entity::SetVisible(bool IsVisible)
{
	m_bVisible = IsVisible;
}

bool Entity::IsVisible()
{
	return m_bVisible;
}

void Entity::Clear()
{
	m_iIndex = NULL;
	m_dwIndex = NULL;
	m_dwPointer = NULL;
	m_dwBoneMatrix = NULL;

	memset( m_BoneMatrix, NULL, sizeof( Matrix3x4[ 128 ] ) );

	m_vOrigin = Vector( 0, 0, 0 );
	m_vEye = Vector( 0, 0, 0 );
	m_vEyeAngles = Vector( 0, 0, 0 );
	m_vVelocity = Vector( 0, 0, 0 );
	m_vPunch = Vector( 0, 0, 0 );

	m_iTeamNum = NULL;
	m_iHealth = NULL;
	m_iFlags = NULL;
	m_iCrossHairID = NULL;
	m_hActiveWeapon = NULL;
	m_iWeaponId = NULL;
	m_iShotsFired = NULL;

	m_bDead = true;
	m_bIsDormant = false;
	m_i64SpottedByMask = NULL;
}