#ifndef _ACTUALAIMBOT_H_
#define _ACTUALAIMBOT_H_

#include "..\main.h"
[junk_enable /]	
class ActualAimbot
{
public:
	ActualAimbot() = default;
	~ActualAimbot() = default;

	int GetPlayerClosestToCrosshair(int bone)
	{
		int index = -1;
		float best = fov;
		
		if (SaveTargetAim)
		{
			if (m_Target != -1 && EntityList[ m_Target ].IsVisible())
				return m_Target;
		}

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
	
	int GetBoneClosestToCrosshair()
	{
		int index = -1;
		int bestBone = -1;
		
		float maxfov = fov;
		const int size = 6;
		int bones[size] = { 8,7,6,5,4,3 };

		if (SaveTargetAim)
		{
			if (m_Target != -1 && EntityList[ m_Target ].IsVisible())
				return m_Target;
		}

		for (int i = 0; i < csgo->GetMaxClients(); i++) {
			
			if (EntityList[ i ].GetTeamNum() == LocalEntity.GetTeamNum())
				continue;

			if (!EntityList[ i ].IsValid())
				continue;
				
			if (!EntityList[ i ].IsVisible())
				continue;
			
			Vector ViewAngles, PunchAngles;
			csgo->GetViewAngles(ViewAngles);
			PunchAngles = LocalEntity.GetPunchAngles();
			PunchAngles.z = 0.0f;
			ViewAngles -= PunchAngles * 2;
			
			for (int num = 0; num < size; num++) {
				float fov = GetFov(ViewAngles, LocalEntity.GetEyePosition(), EntityList[ i ].GetBonePosition(bones[num]));
				if (fov < maxfov) {
					maxfov = fov;
					bestBone = bones[ num ];
					index = i;
				}
			}
		}

		if (index != -1)
		{
			if (TargetBone == -1)
			{
				TargetBone = bestBone;
			}
			return index; 
		}
		else 
		{
			TargetBone = -1;
			return -1;
		}
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
		float tickBase = mem->Read<float>(LocalEntity.GetPointer() + ofs->m_nTickBase);
		float intervalPerTick = csgo->GetIntervalPerTick();
		float flServerTime = tickBase * intervalPerTick;

		return (!(nextPrimaryAttack > flServerTime));
	}
	
	/*int Nearest(int target)
    {
		static float MAXFOV = 360.f;
		static int DOT_NOTVALID = -1;
		static Vector curView;
		csgo->GetViewAngles(curView);
		
    	int best;
    	float maxfov = MAXFOV;
    	const int size = 7;
    	int bones[size] = { 6,5,4,3,2,1,0 };
    	for (int num = 0; num < size; num++) {
    		if (target != DOT_NOTVALID) {
    			float fov = GetFov(curView, LocalEntity.GetEyePosition(), EntityList[ target ].GetBonePosition(bones[num]));
    			if (fov < maxfov) {
    				maxfov = fov;
    				best = bones[num ];
    			}
    		}
    	}
    	return best;
    }*/
	
	float GetSmoothValue(Vector &angles)
	{
		bool adaptive_smooth_enable = false;
		int ID = LocalEntity.GetWeaponType();
		//[junk_disable /]
		switch (ID)
		{
			case EWeaponType::WeapType_Rifle:
				if (cfg->RiflesAdaptiveSmooth)
					adaptive_smooth_enable = true;
				break;
			case EWeaponType::WeapType_Pistol:
				if (cfg->PistolsAdaptiveSmooth)
					adaptive_smooth_enable = true;
				break;
			case EWeaponType::WeapType_SMG:
				if (cfg->SMGsAdaptiveSmooth)
					adaptive_smooth_enable = true;
				break;
			case EWeaponType::WeapType_Sniper:
				if (cfg->SnipersAdaptiveSmooth)
					adaptive_smooth_enable = true;
				break;
			case EWeaponType::WeapType_Shotgun:
				if (cfg->ShotGunsAdaptiveSmooth)
					adaptive_smooth_enable = true;
				break;
			case EWeaponType::WeapType_LMG:
				if (cfg->LMGsAdaptiveSmooth)
					adaptive_smooth_enable = true;
				break;
			default:
				break;
		}
		//[junk_enable /]
		float smooth_value = fSmoothFactor;
		
		if (adaptive_smooth_enable)
		{
			Vector curView; csgo->GetViewAngles(curView);
			Vector ang_delta = curView - angles;
			float adaptive_smooth = abs(ang_delta.LengthXY()) * cfg->AdaptiveSmoothMultiplier;
			
			smooth_value += adaptive_smooth;
		}
		
		return smooth_value;
	}

	Vector GetAimbotAngles()
	{

		if (m_Target != -1 && !EntityList[ m_Target ].IsValid()) //комент, чтобы игнорило выпавших из фова
			DropTarget();

		if (!LocalEntity.GetActiveWeaponAmmo())
			return Vector(0, 0, 0);

		if (boneToAimAt)
		{
			TargetBone = boneToAimAt;
			m_Target = GetPlayerClosestToCrosshair(boneToAimAt);
		}
		else
		{
			m_Target = GetBoneClosestToCrosshair();
		}

		if (m_Target == -1)
			return Vector(0, 0, 0);

		// Check if bones are actually updated
		Vector enemyPos; Vector targetOrigin = EntityList[ m_Target ].GetOrigin();
		enemyPos = EntityList[ m_Target ].GetBonePosition(TargetBone);
		

		float check = (float)abs((targetOrigin - enemyPos).Length());

		if (check > 75.0f)
			return Vector(0, 0, 0);
		
		Vector ViewAngles;
		Vector position = LocalEntity.GetEyePosition();

		// Velocity Compensate
		Vector myVelocity = LocalEntity.GetVelocity();
		Vector targetVelocity = EntityList[ m_Target ].GetVelocity();
		Vector distVec = position - enemyPos;
		float dist = FastSQRT((distVec.x * distVec.x) + (distVec.y * distVec.y) + (distVec.z * distVec.z));

		if (compensate && dist > 0.001f)
		{
			enemyPos.x += (targetVelocity.x) / dist;
			enemyPos.y += (targetVelocity.y) / dist;
			enemyPos.z += (targetVelocity.z) / dist;
			enemyPos.x -= (myVelocity.x) / dist;
			enemyPos.y -= (myVelocity.y) / dist;
			enemyPos.z -= (myVelocity.z) / dist;
		}

		// Calculate Angle to look at
		ViewAngles = CalcAngle(position, enemyPos);
		ViewAngles.Normalize();

		// RCS 
		Vector PunchAngles = LocalEntity.GetPunchAngles();
		PunchAngles.z = 0.0f;
		ViewAngles -= PunchAngles * rcsScale;

		ViewAngles.Normalize();
		
		float SMOOTH_VALUE = GetSmoothValue(ViewAngles);
		if (!cfg->spiralAim)
		{
			// Smooth Angles
			Vector curView;
			csgo->GetViewAngles(curView);
			Vector qDelta(ViewAngles.x - curView.x, ViewAngles.y - curView.y, 0.0f);

			qDelta.Normalize();
			ViewAngles.x = curView.x + qDelta.x / SMOOTH_VALUE;
			ViewAngles.y = curView.y + qDelta.y / SMOOTH_VALUE;

			ViewAngles.Normalize();
		}
		else
		{				
			Vector vecCurrentViewAngles; csgo->GetViewAngles(vecCurrentViewAngles);
			
			Vector vecViewAngleDelta = ViewAngles - vecCurrentViewAngles; 
			vecViewAngleDelta.Normalize();
			
			vecViewAngleDelta += {vecViewAngleDelta.y/cfg->spiralAimX, vecViewAngleDelta.x/cfg->spiralAimY, 0.0f};
			vecViewAngleDelta.Normalize();
			
			vecViewAngleDelta /= SMOOTH_VALUE;
			vecViewAngleDelta.Normalize();
			
			ViewAngles = vecCurrentViewAngles + vecViewAngleDelta;
			ViewAngles.Normalize();
		}
		return ViewAngles;
	}
	
	bool IsKillDelay() {
		if (g_Target != -1)
			if (!EntityList[ g_Target ].IsValid()) {
				Sleep(KillDelay);
				return true;
			}
			
		return false;
	}
	
	void MoveMouse_round(float xPixels, float yPixels)
	{ 		
		INPUT input = { 0 };
		input.mi.mouseData = 0;
		input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
		input.type = INPUT_MOUSE;
		input.mi.dx = roundf(xPixels);
		input.mi.dy = roundf(yPixels);
		input.mi.dwExtraInfo = 0;
		input.mi.time = 0;
		SendInput(1, &input, sizeof( INPUT ) );
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
				
			if (!cfg->aimbotEnabled || cfg->rageModeEnabled)
				continue;
			
			if (!cfg->aimbot[LocalEntity.GetActiveWeapon()].enable)
				continue;
				
			if (!csgo->IsInGame())
				continue;
				
			if (LocalEntity.IsDead())
				continue;
			
			fov = cfg->aimbot[LocalEntity.GetActiveWeapon()].fov;
			StartB = cfg->aimbot[LocalEntity.GetActiveWeapon()].startbullet;
			EndB = cfg->aimbot[LocalEntity.GetActiveWeapon()].endbullet;
			Delay = cfg->aimbot[LocalEntity.GetActiveWeapon()].delay;
			KillDelay = cfg->aimbot[LocalEntity.GetActiveWeapon()].killdelay;
			rcsScale = cfg->aimbot[LocalEntity.GetActiveWeapon()].rcsAmount;
			fSmoothFactor = cfg->aimbot[LocalEntity.GetActiveWeapon()].smooth;
			boneToAimAt = cfg->aimbot[LocalEntity.GetActiveWeapon()].bone;
			faceitAim = cfg->faceitAim;
			SaveTargetAim = cfg->SaveTargetAim;
			
			//printf("g_Target: %d\n",g_Target);
			if (!GetAsyncKeyState(cfg->keys.aimbot_hold))
			{
				DropTarget();
				TargetBone = -1;
				continue;
			}
				
			int work = LocalEntity.GetShotsFired();
			
			anglesToAim = GetAimbotAngles();	


			if (IsKillDelay()) {
				g_Target = -1;
				continue;
			} 
			
			if (anglesToAim == Vector(0, 0, 0))
				continue;
				
			g_Target = m_Target;
			
			if (faceitAim)
			{
				static Vector angDelta;
				csgo->GetViewAngles(angDelta); 
				angDelta -= anglesToAim;
							   
				float xPixels = angDelta.y / (0.022f * 2 * 1.0f);
				float yPixels = -angDelta.x / (0.022f * 2 * 1.0f);
				
				static int xSleep = 0;
				static int ySleep = 0;
				if (abs(xPixels) < 1.f && GetTickCount() > xSleep)
				{
					if(xPixels > 0)
					{
						xPixels = 1;
					}
					if(xPixels < 0)
					{
						xPixels = -1;
					}
					xSleep = GetTickCount() + 1;
				}
				
				if (abs(yPixels) < 1.f && GetTickCount() > ySleep)
				{
					if(yPixels > 0)
					{
						yPixels = 1;
					}
					if(yPixels < 0)
					{
						yPixels = -1;
					}
					ySleep = GetTickCount() + 1;
				}
				
				if (StartB <= work && work <= EndB)
					MoveMouse_round(xPixels, yPixels);
			}
			else
			{
				if (StartB <= work && work <= EndB)
					csgo->SetViewAngles(anglesToAim);
			}
		}
	} catch(...) {
		mem->debuglog(__FILE__);
	}
	}

private:	

	Vector anglesToAim;
	
	int m_Target = -1;
	int g_Target = -1;
	int TargetBone = -1;
	
	bool SaveTargetAim = false;
	bool faceitAim = false;
	float fov = 2.5;
	int StartB, EndB;
	float Delay;
	float KillDelay;
	float rcsScale = 2.0f;
	float fSmoothFactor = 1.f;
	bool compensate = true;
	int boneToAimAt = 35;
	[junk_disable /]
};

#endif //_ACTUALAIMBOT_H_