#ifndef _TRIGGERBOT_H_
#define _TRIGGERBOT_H_

#include "..\main.h"
[junk_enable /]	
class Triggerbot
{
public:
	void Start()
	{
	try{
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			
			if (cfg->panicMode)
				continue;
							
			if (!cfg->triggerbotEnabled)
				continue;
			
			if (!csgo->IsInGame())
				continue;
				
			if (LocalEntity.IsDead())
				continue;
			
			//printf("Accuracy: %.4f\n", LocalEntity.GetAccuracyPenalty());
			if (GetAsyncKeyState(cfg->keys.triggerbot_hold) & 0x8000) {
				//if (/*InCross() != -1*/FovTriggerbot() && LocalEntity.GetAccuracyPenalty() <= 0.01f) {
				if (/*InCross() != -1*/FovTriggerbot()) {
					Sleep(cfg->triggerbot.delay);
					csgo->PressAttackKey();
					Sleep(10);
					csgo->ReleaseAttackKey();
					Sleep(5);
				}
			}
		}
	} catch(...) {
		mem->debuglog(__FILE__);
	}
	}

private:	
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
	
	bool FovTriggerbot()
	{
		Vector current, PunchAngles;
		csgo->GetViewAngles(current);

		PunchAngles = LocalEntity.GetPunchAngles();
		PunchAngles.z = 0.0f;
		current -= PunchAngles * 2;
	 
		for (int i = 0; i < csgo->GetMaxClients(); i++) {
			if (EntityList[ i ].GetTeamNum() == LocalEntity.GetTeamNum())
				continue;

			if (!EntityList[ i ].IsValid())
				continue;
			
			if (InBone(current, i, 8, 3))
				return true;
				
			if (InBone(current, i, 6, 5))
				return true;
				
			if (InBone(current, i, 4, 4))
				return true;
			
		}
		return false;
	}
	
	bool InBone( Vector &current, int m_target, int bone, float fov )
	{
		auto direction = EntityList[ m_target ].GetBonePosition( bone ) - LocalEntity.GetEyePosition();
		VectorNormalize( direction );
 
		auto target = VectorAngles( direction );
		ClampAngles( target );
		
		Vector v_dist = LocalEntity.GetOrigin() - EntityList[ m_target ].GetOrigin();
		auto distance = FastSQRT((v_dist.x * v_dist.x) + (v_dist.y * v_dist.y) + (v_dist.z * v_dist.z));
 
		// modify 20.f as needed, depending on bone, random atm
		if( CanHit( current, target, distance, fov ) )
		{
			return true;
		}
		
		return false;
	}
	
	bool CanHit( Vector current, Vector target, float distance, float max )
	{
		/*float hitchance = 75.f + ( cfg->angleTriggerbotChance / 4 );
		//printf("Accuracy: %.1f\n", LocalEntity.GetAccuracyPenalty());
		
		if( (!cfg->angleTriggerbotChance != 0.f) || (( 1.0f - LocalEntity.GetAccuracyPenalty() ) * 100.f >= hitchance ))
		{
			printf("HIT: %.5f; Penalty: %.5f\n", ( 1.0f - LocalEntity.GetAccuracyPenalty() ) * 100.f, LocalEntity.GetAccuracyPenalty());*/
			auto delta = target - current;
			ClampAngles( delta );
		 
			auto xdist = sinf( DEG2RAD( delta.Length() ) ) * distance / 2.f;
		 
			return xdist <= max;
		//}
		
		return false;
	}
	
	Vector VectorAngles( const Vector& direction )
	{
		float pitch, yaw;
	 
		if( !direction.x && !direction.y )
		{
			pitch = direction.z > 0.f ? 270.f : 90.f;
			yaw = 0.f;
		}
		else
		{
			pitch = RAD2DEG( atan2f( -direction.z, direction.LengthXY() ) );
			pitch += pitch < 0.f ? 360.f : 0.f;
	 
			yaw = RAD2DEG( atan2f( direction.y, direction.x ) );
			yaw += yaw < 0.f ? 360.f : 0.f;
		}
	 
		return Vector( pitch, yaw, 0.f );
	}
	 
	float VectorNormalize( Vector& vector )
	{
		auto length = vector.Length();
		vector = length ? vector / length : Vector();
	 
		return length;
	}
	 
	float AngleNormalize( float angle )
	{
		return isfinite( angle ) ? remainder( angle, 360.f ) : 0.f;
	}
	 
	void ClampAngles( Vector& angles )
	{
		angles.x = std::max( -89.f, std::min( 89.f, AngleNormalize( angles.x ) ) );
		angles.y = AngleNormalize( angles.y );
		angles.z = 0.f;
	}
[junk_disable /]
};

#endif 