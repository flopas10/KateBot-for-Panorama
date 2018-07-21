#include "main.h"

Config* cfg = new Config();

[junk_enable /]	
[enc_string_enable /]
void Config::SaveConfig()
{
	for (auto& map_data : m_mapWeapons)
	{
		char * weaponName = new char[map_data.second.size() + 1];
		std::copy(map_data.second.begin(), map_data.second.end(), weaponName);
		weaponName[map_data.second.size()] = '\0';
		
		char* weapontype = WeaponType(map_data.first);
		
		g_pFiles->WriteBoolParam	(weapontype,weaponName, ".aimbot.enable",aimbot[map_data.first].enable);
		g_pFiles->WriteIntParam		(weapontype,weaponName, ".aimbot.start",aimbot[map_data.first].startbullet);
		g_pFiles->WriteIntParam		(weapontype,weaponName, ".aimbot.end",aimbot[map_data.first].endbullet);
		g_pFiles->WriteFloatParam	(weapontype,weaponName, ".aimbot.delay",aimbot[map_data.first].delay);
		g_pFiles->WriteFloatParam	(weapontype,weaponName, ".aimbot.fov",aimbot[map_data.first].fov);
		g_pFiles->WriteFloatParam	(weapontype,weaponName, ".aimbot.smooth",aimbot[map_data.first].smooth);
		g_pFiles->WriteIntParam		(weapontype,weaponName, ".aimbot.bone",aimbot[map_data.first].bone);
		g_pFiles->WriteFloatParam	(weapontype,weaponName, ".aimbot.rcsAmount",aimbot[map_data.first].rcsAmount);
		g_pFiles->WriteFloatParam	(weapontype,weaponName, ".aimbot.killDelay",aimbot[map_data.first].killdelay);
		
		delete[] weaponName;
	}
}

void Config::LoadConfig()
{
	keys.aimbot_toggle = g_pFiles->ReadInt("Keys","Aimbot.Toggle");
	keys.aimbot_hold = g_pFiles->ReadInt("Keys","Aimbot.Hold");
	keys.glowesp_toggle = g_pFiles->ReadInt("Keys","GlowESP.Toggle");
	keys.triggerbot_hold = g_pFiles->ReadInt("Keys","Triggerbot.Hold");
	keys.bunnyhop_hold = g_pFiles->ReadInt("Keys","Bunnyhop.Hold");
	keys.skinchanger_toggle = g_pFiles->ReadInt("Keys","Skinchanger.Toggle");
	keys.ragemode_toggle = g_pFiles->ReadInt("Keys","RageMode.Toggle");
	//keys.panic_mode = g_pFiles->ReadInt("Keys","PANIC_MODE");
	
	glowEspEnabled = g_pFiles->ReadBool("Features","GlowESP.Enable");
	aimbotEnabled = g_pFiles->ReadBool("Features","Aimbot.Enable");
	triggerbotEnabled = g_pFiles->ReadBool("Features","Triggerbot.Enable");
	bunnyhopEnabled = g_pFiles->ReadBool("Features","Bunnyhop.Enable");
	skinchangerEnabled = g_pFiles->ReadBool("Features","Skinchanger.Enable");
	
	AdaptiveSmoothMultiplier = g_pFiles->ReadFloat("Settings","Aimbot.AdaptiveSmooth.Multiplier");
	RiflesAdaptiveSmooth = g_pFiles->ReadBool("Settings","Aimbot.AdaptiveSmooth.Rifles");
	PistolsAdaptiveSmooth = g_pFiles->ReadBool("Settings","Aimbot.AdaptiveSmooth.Pistol");
	SnipersAdaptiveSmooth = g_pFiles->ReadBool("Settings","Aimbot.AdaptiveSmooth.Sniper");
	SMGsAdaptiveSmooth = g_pFiles->ReadBool("Settings","Aimbot.AdaptiveSmooth.SMG");
	ShotGunsAdaptiveSmooth = g_pFiles->ReadBool("Settings","Aimbot.AdaptiveSmooth.Shotgun");
	LMGsAdaptiveSmooth = g_pFiles->ReadBool("Settings","Aimbot.AdaptiveSmooth.LMG");
	
	spiralAim = g_pFiles->ReadBool("Settings","Aimbot.Spiral.Enable");
	spiralAimX = g_pFiles->ReadFloat("Settings","Aimbot.Spiral.X");
	spiralAimY = g_pFiles->ReadFloat("Settings","Aimbot.Spiral.Y");
	
	SaveTargetAim = g_pFiles->ReadBool("Settings","Aimbot.SaveTarget");
	faceitAim = g_pFiles->ReadBool("Settings","Aimbot.FaceitMode");
	bspParsing = g_pFiles->ReadBool("Settings","BSPParsing");
	skinchangerAutoUpdate = g_pFiles->ReadBool("Settings","Skinchanger.AutoUpdate");
	triggerbot.delay = g_pFiles->ReadFloat("Settings","Triggerbot.Delay");
	glowesp.alpha = g_pFiles->ReadInt("Settings","GlowESP.Alpha");
	glowesp.onlyVisible = g_pFiles->ReadBool("Settings","GlowESP.OnlyVisible.Enable");
	glowesp.bodycolorenable = g_pFiles->ReadBool("Settings","GlowESP.BodyColors.Enable");
	glowesp.bodyred = g_pFiles->ReadInt("Settings","GlowESP.BodyRed");
	glowesp.bodygreen = g_pFiles->ReadInt("Settings","GlowESP.BodyGreen");
	glowesp.bodyblue = g_pFiles->ReadInt("Settings","GlowESP.BodyBlue");
	glowesp.bodyalpha = g_pFiles->ReadInt("Settings","GlowESP.BodyAlpha");
	knifeModel = g_pFiles->ReadInt("Settings","KnifeChanger.Model(0-10)");
	
	for (auto& map_data : m_mapWeapons)
	{
		char * weaponName = new char[map_data.second.size() + 1];
		std::copy(map_data.second.begin(), map_data.second.end(), weaponName);
		weaponName[map_data.second.size()] = '\0';
		
		char* weapontype = WeaponType(map_data.first);
		
		aimbot[map_data.first].enable 		= g_pFiles->ReadBoolParam(weapontype,weaponName, ".aimbot.enable");
		aimbot[map_data.first].startbullet 	= g_pFiles->ReadIntParam(weapontype,weaponName, ".aimbot.start");
		aimbot[map_data.first].endbullet 	= g_pFiles->ReadIntParam(weapontype,weaponName, ".aimbot.end");
		aimbot[map_data.first].delay 		= g_pFiles->ReadFloatParam(weapontype,weaponName, ".aimbot.delay");
		aimbot[map_data.first].fov 			= g_pFiles->ReadFloatParam(weapontype,weaponName, ".aimbot.fov");
		aimbot[map_data.first].smooth 		= g_pFiles->ReadFloatParam(weapontype,weaponName, ".aimbot.smooth");
		aimbot[map_data.first].bone 		= g_pFiles->ReadIntParam(weapontype,weaponName, ".aimbot.bone");
		aimbot[map_data.first].rcsAmount 	= g_pFiles->ReadFloatParam(weapontype,weaponName, ".aimbot.rcsAmount");
		aimbot[map_data.first].killdelay 	= g_pFiles->ReadFloatParam(weapontype,weaponName, ".aimbot.killDelay");
		skin[map_data.first].id 	= g_pFiles->ReadIntParam(weapontype,weaponName, ".skin.id");
		skin[map_data.first].statTrack 	= g_pFiles->ReadIntParam(weapontype,weaponName, ".skin.statTrack");
		
		delete[] weaponName;
	}
	
	needIndexes = true;
}
[junk_disable /]	
char* Config::WeaponType(int ID)
{
	switch (ID)
	{
		case WID_Negev:
		case WID_M249:
		return "LMG";

		case WID_AWP:
		case WID_G3SG1_Auto:
		case WID_SCAR_Auto:
		case WID_Scout:
		return "Snipers";

		case WID_XM1014:
		case WID_MAG7:
		case WID_Nova:
		case WID_SawedOff:
		return "Shotguns";

		case WID_MAC10:
		case WID_P90:
		case WID_UMP45:
		case WID_PPBizon:
		case WID_MP7:
		case WID_MP9:
		return "SMGs";

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
		case WID_Zeus:
		return "Pistols";

		case WID_AK47:
		case WID_AUG:
		case WID_FAMAS:
		case WID_M4A4:
		case WID_M4A1S:
		case WID_Galil:
		case WID_SG553:
		return "Rifles";

		default:
		return "null";
	}
}