#ifndef _RAGEMODE_H_
#define _RAGEMODE_H_

#include "..\main.h"
[junk_enable /]	
class RageMode
{
public:
	RageMode() = default;
	~RageMode() = default;

	int GetPlayerClosestToCrosshair(int bone)
	{
		int index = -1;
		float best = 180;
		
		if (m_Target != -1 && EntityList[ m_Target ].IsVisible())
			return m_Target;

		for (int i = 0; i < csgo->GetMaxClients(); i++) {
			
			if (EntityList[ i ].GetTeamNum() == LocalEntity.GetTeamNum())
				continue;

			if (!EntityList[ i ].IsValid())
				continue;
				
			if (!EntityList[ i ].IsVisible())
				continue;
			
			Vector ViewAngles;
			csgo->GetViewAngles(ViewAngles);
			
			float tmp = GetFov(ViewAngles, LocalEntity.GetEyePosition(), EntityList[ i ].GetBonePosition(bone));

			if (tmp < best)
			{
				best = tmp;
				index = i;
			}
		}

		if (index != -1) return index; else return -1;
	}

	void MakeVector(Vector angle, Vector& vector)
	{
		float pitch = float(angle[0] * M_PI / 180);
		float yaw = float(angle[1] * M_PI / 180);
		float tmp = float(cos(pitch));
		vector.x = float(-tmp * -cos(yaw));
		vector.y = float(sin(yaw)*tmp);
		vector.z = float(-sin(pitch));
	}

	float GetFov(Vector angle, Vector src, Vector dst)
	{
		Vector ang, aim;
		ang = CalcAngle(src, dst);
		MakeVector(angle, aim);
		MakeVector(ang, ang);

		float mag = FastSQRT(pow(aim.x, 2) + pow(aim.y, 2) + pow(aim.z, 2));
		float u_dot_v = aim.DotProduct(ang);

		return RAD2DEG(acos(u_dot_v / (pow(mag, 2))));
	}

	Vector CalcAngle(Vector src, Vector dst)
	{
		Vector ret;
		Vector vDelta = src - dst;
		float fHyp = FastSQRT((vDelta.x * vDelta.x) + (vDelta.y * vDelta.y));

		ret.x = (atanf(vDelta.z / fHyp)) * (180.0f / (float)M_PI);
		ret.y = (atanf(vDelta.y / vDelta.x)) * (180.0f / (float)M_PI);

		if (vDelta.x >= 0.0f)
			ret.y += 180.0f;

		return ret;
	}

	void DropTarget()
	{
		m_Target = -1;
	}

	bool CanShoot()
	{
		DWORD weapBase = LocalEntity.GetActiveWeaponBase();
		float nextPrimaryAttack = mem->Read<float>(weapBase + ofs->m_flNextPrimaryAttack);
		return (!( nextPrimaryAttack > csgo->GetCurTime() ));
	}


	Vector GetAimbotAngles()
	{

		if (m_Target != -1 && !EntityList[ m_Target ].IsValid()) //комент, чтобы игнорило выпавших из фова
			DropTarget();

		if (!LocalEntity.GetActiveWeaponAmmo())
			return Vector(0, 0, 0);

		m_Target = GetPlayerClosestToCrosshair(boneToAimAt);

		if (m_Target == -1)
			return Vector(0, 0, 0);

		// Check if bones are actually updated
		Vector targetOrigin = EntityList[ m_Target ].GetOrigin();
		Vector enemyPos = EntityList[ m_Target ].GetBonePosition( boneToAimAt );
		
		Vector ViewAngles;
		Vector position = LocalEntity.GetEyePosition();

		// Calculate Angle to look at
		ViewAngles = CalcAngle(position, enemyPos);
		ViewAngles.Normalize();

		// RCS
		Vector PunchAngles = LocalEntity.GetPunchAngles();
		PunchAngles.z = 0.0f;
		ViewAngles -= PunchAngles * rcsScale;

		ViewAngles.Normalize();
		
		return ViewAngles;
	}
	
	void Start()
	{
	try{
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			
			if (cfg->panicMode)
				continue;
			
			if (!miscUtils->IsCSGOActiveWindow())
				continue;
				
			if (!cfg->rageModeEnabled)
				continue;
				
			if (!csgo->IsInGame())
				continue;
				
			if (LocalEntity.IsDead())
				continue;

			int WeaponType = LocalEntity.GetWeaponType();
			if ( WeaponType != EWeaponType::WeapType_LMG &&
				 WeaponType != EWeaponType::WeapType_Sniper &&
				 WeaponType != EWeaponType::WeapType_Shotgun &&
				 WeaponType != EWeaponType::WeapType_SMG &&
				 WeaponType != EWeaponType::WeapType_Pistol &&
				 WeaponType != EWeaponType::WeapType_Rifle)
				continue;
			
			if ( CanShoot() )
			{
				int weaponId = LocalEntity.GetActiveWeapon();
				if (weaponId == WID_Scout || weaponId == WID_Deagle)
				{
					float vel = LocalEntity.GetVelocity().LengthXY();
					
					if ( vel <= 60 )
						trigger();

				} else
					trigger();
			}

			anglesToAim = GetAimbotAngles();	
			
			if (anglesToAim == Vector(0, 0, 0))
				continue;

			csgo->SetViewAngles(anglesToAim);
		}
	} catch(...) {
		mem->debuglog(__FILE__);
	}
	}

private:	

	Vector anglesToAim;
	int m_Target = -1;
	float rcsScale = 2.0f;
	int boneToAimAt = 8;

	int InCross() 
	{
		int idInX = mem->Read<int>(LocalEntity.GetPointer() + ofs->m_iCrossHairID);
		if (idInX != 0) {
			for (int i = 0; i < csgo->GetMaxClients(); i++) {
				//std::this_thread::yield();
				if (EntityList[ i ].GetIndex() == idInX) {
					if (EntityList[ i ].GetHealth() > 0 && EntityList[ i ].GetTeamNum() != LocalEntity.GetTeamNum()) 
						return i;
				}
			}
		}
		return -1;
	}

	void trigger()
	{
		int _incross = InCross() ;
								
		if (_incross != -1)
		{
			csgo->PAttack();
			Sleep(5);
			csgo->MAttack();
		}
	}
	[junk_disable /]
};

#endif //_RAGEMODE_H_