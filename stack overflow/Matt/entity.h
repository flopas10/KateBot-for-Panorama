#pragma once
#define FL_ONGROUND			(1 << 0)
#define FL_DUCKING			(1 << 1)
#define FL_INAIR			256
#define FL_INAIRDUCKING		262

class Entity
{
public:
	Entity();
	~Entity();

	void		Update( int iIndex );

	DWORD_PTR	GetPointer();
	DWORD_PTR	GetLocalPointer();
	DWORD_PTR	GetBoneMatrix();

	Matrix3x4	GetBoneMatrix( int iBone );


	Vector		GetPunchAngles();
	Vector      GetEyeAngles();
	Vector		GetOrigin();
	Vector		GetVelocity();
	Vector		GetEyePosition();
	Vector		GetBonePosition( int iBone );

	int         GetActiveWeapon();
	int 		GetActiveWeaponAmmo();
	int 		GetShotsFired();
	
	float 		GetAccuracyPenalty();
	
	DWORD_PTR   GetActiveWeaponBase();
	bool		IsActiveWeaponNotVaild();
	bool		IsActiveWeaponPistol();
	EWeaponType GetWeaponType();

	int			GetTeamNum();
	int			GetHealth();
	int			GetFlags();
	int			GetCrosshairEnt();
	int 		GetIndex();

	bool		IsValid();
	bool		IsPlayer();
	bool		IsDead();
	bool        IsSpottedByMask();
	bool		IsDormant();
	bool 		IsJumping();

	bool		IsVisible();
	void		CheckVisible();
	void		SetVisible(bool IsVisible);

	void		Clear();

private:
	char chunk[0xAA50];
		
	int				m_iIndex;

	DWORD_PTR		m_dwPointer;
	DWORD_PTR		m_dwLocalPointer;
	DWORD_PTR		m_dwBoneMatrix;

	Matrix3x4		m_BoneMatrix[ 128 ];

	Vector			m_vEyeAngles;
	Vector			m_vOrigin;
	Vector			m_vEye;
	Vector			m_vPunch;
	Vector			m_vVelocity;

	int				m_iTeamNum;
	int				m_iHealth;
	int				m_iFlags;
	int				m_iCrossHairID;
	int				m_hActiveWeapon;
	int				m_iWeaponId;
	int 			m_iShotsFired;
	int 			m_dwIndex;

	bool			m_bDead;
	bool			m_bIsDormant;
	bool			m_bVisible;
	bool			m_bHasBomb;

	INT64			m_i64SpottedByMask;
	
};

extern Entity LocalEntity;
extern Entity EntityList[ 65 ];
