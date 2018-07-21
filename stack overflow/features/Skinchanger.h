#ifndef _SKINCHANGER_H_
#define _SKINCHANGER_H_

#include "..\main.h"
[junk_enable /]	
class Skinchanger
{
public:
	void Start()
	{
	try {
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			
			if (cfg->panicMode)
				continue;
				
			if (!miscUtils->IsCSGOActiveWindow())
				continue;
				
			if (!cfg->skinchangerEnabled)
				continue;
			
			if (!csgo->IsInGame())
				continue;
				
			if (!csgo->IsTeamSelected())
				continue;
				
			if (LocalEntity.IsDead())
				continue;
			
			
			
			static bool PreNeedUpdate = false;
			for (int i = 1; i < 10; i++) {
				DWORD Weapon = mem->Read<DWORD>(LocalEntity.GetPointer() + ofs->m_hMyWeapons + ((i - 1) * 0x04));
				DWORD WeaponBase = mem->Read<DWORD>(client->GetImage() + ofs->m_dwEntityList + ((Weapon & 0xfff) - 1) * 0x10);

				int WeaponID = mem->Read<int>(WeaponBase + ofs->m_iItemDefinitionIndex);
				if (WeaponID < 65) {
					if (cfg->skin[WeaponID].id != 0) {
						if (mem->Read<int>(WeaponBase + ofs->m_iItemIDLow) != -1) {
							int MyAccountID = mem->Read<int>(WeaponBase + ofs->m_OriginalOwnerXuidLow);
							SetSkin(WeaponBase, cfg->skin[WeaponID].id, cfg->skin[WeaponID].statTrack, 0, 0, MyAccountID);
							if (cfg->skinchangerAutoUpdate && !chokeUpdate)
								needUpdate = true;
						}
					}
				}
			}
			
			if (needUpdate && !chokeUpdate) {
				csgo->ForceFullUpdate();
				needUpdate = false;
				chokeUpdate = 2000;
			}
			
			if (chokeUpdate) {
				chokeUpdate--;
				if (chokeUpdate < 0) chokeUpdate = 0;
			}
			
			
			
			if (cfg->knifeModel != 0) {
				DWORD WeaponBase = LocalEntity.GetActiveWeaponBase();
				int WeaponDefID = LocalEntity.GetActiveWeapon();
				
				[junk_disable /]
				if (cfg->needIndexes) {
					DWORD hWeapon = mem->Read<DWORD>(LocalEntity.GetPointer() + ofs->m_hViewModel);
					DWORD KnifeBase = mem->Read<DWORD>(client->GetImage() + ofs->m_dwEntityList + ((hWeapon & 0xFFF) - 1) * 0x10);
					
					switch ( WeaponDefID )  {
						case 61:
							startpoint = mem->Read<int>(KnifeBase + ofs->m_nModelIndex);
							knifeId = startpoint + 28 + (3 * cfg->knifeModel-3);
							cfg->needIndexes = false;
							break;
						case 4:
							startpoint = mem->Read<int>(KnifeBase + ofs->m_nModelIndex);
							knifeId = startpoint + 273 + (3 * cfg->knifeModel-3);
							cfg->needIndexes = false;
							break;
						case 32:
							startpoint = mem->Read<int>(KnifeBase + ofs->m_nModelIndex);
							knifeId = startpoint + 128 + (3 * cfg->knifeModel-3);
							cfg->needIndexes = false;
							break;
						case 1:
							startpoint = mem->Read<int>(KnifeBase + ofs->m_nModelIndex);
							knifeId = startpoint + 300 + (3 * cfg->knifeModel-3);
							cfg->needIndexes = false;
							break;
					}
[junk_enable /]
				} else if ((IsKnife(WeaponDefID) && cfg->needIndexes == false && WeaponBase >= 1000)) {
					DWORD hWeapon = mem->Read<DWORD>(LocalEntity.GetPointer() + ofs->m_hViewModel);
					DWORD KnifeBase = mem->Read<DWORD>(client->GetImage() + ofs->m_dwEntityList + ((hWeapon & 0xFFF) - 1) * 0x10);
					if (KnifeBase >= 1000) {
						setKnifeData(knifeId);
						
						if (mem->Read<int>(KnifeBase + ofs->m_nModelIndex) != knifeId)
							mem->Write<int>(KnifeBase + ofs->m_nModelIndex, knifeId);
						
						tSkinModel tmpSkinModel = mem->Read<tSkinModel>(WeaponBase + ofs->m_iWorldModelIndex);
						if (tmpSkinModel.off1 != knifeId + 1 || tmpSkinModel.off2 != knifeId) 
							mem->Write<tSkinModel>(WeaponBase + ofs->m_iWorldModelIndex,SkinModel);
							
						if (mem->Read<int>(WeaponBase + ofs->m_iItemDefinitionIndex) != GetKnifeID(cfg->knifeModel)) {
							mem->Write<int>(WeaponBase + ofs->m_iItemDefinitionIndex, GetKnifeID(cfg->knifeModel));
						}
					}
				}
			}
			
			if (GetAsyncKeyState(48)) {
				csgo->ForceFullUpdate();
				//config()->needIndexes = true;
			}
		}
	} catch(...) {
		mem->debuglog(__FILE__);
	}
	}

private:	
	bool needUpdate = false;
	int chokeUpdate = 0;
	
	int startpoint = 0;
	int knifeId = 0;
	
	[junk_disable /]
	int GetKnifeID(int ID) {
		switch (ID) {
			case 1:
				return 500;
			case 2:
				return 505;
			case 3:
				return 506;
			case 4:
				return 507;
			case 5:
				return 508;
			case 6:
				return 509;
			case 7:
				return 512;
			case 8:
				return 514;
			case 9:
				return 515;
			case 10:
				return 516;
		}
		return 42;
	}
	
	struct tWeaponData {
		int m_nFallbackPaintKit;
		int m_nFallbackSeed;
		float m_flFallbackWear;
		int m_nFallbackStatTrak;
	}WeaponData;
	
	struct tSkinModel {
		int off1;
		int off2;
	}SkinModel;
	
	[junk_enable /]
	void SetSkin(DWORD WeapEnt, int PaintKit, int StatTrak, int Seed, float Wear, int MyAccountID) {
		//Set Weapon Data
		//WeaponData wepData;
		WeaponData.m_nFallbackPaintKit = PaintKit;
		WeaponData.m_nFallbackSeed = Seed;
		WeaponData.m_flFallbackWear = Wear;
		WeaponData.m_nFallbackStatTrak = StatTrak;
		
		//Write to Memory
		mem->Write<tWeaponData>(WeapEnt + ofs->m_nFallbackPaintKit, WeaponData);
		mem->Write<int>(WeapEnt + ofs->m_iAccountID, MyAccountID);
		mem->Write<int>(WeapEnt + ofs->m_iItemIDLow, -1);
	}
	
	void setKnifeData(int knifeId) {
		SkinModel.off1 = knifeId+1;
		SkinModel.off2 = knifeId;
	}
	
	
	bool IsKnife(int weaponID) {
		if (((((weaponID != 42) && (weaponID != 59)) && ((weaponID != 500) && (weaponID != 505))) && (((weaponID != 506) && (weaponID != 507)) && ((weaponID != 508) && (weaponID != 509)))) && (((weaponID != 512) && (weaponID != 514)) && ((weaponID != 515) && (weaponID != 516))))
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	
[junk_disable /]
};

#endif 