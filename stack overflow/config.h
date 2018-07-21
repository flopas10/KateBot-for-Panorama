#pragma once
#include "main.h"
#include "Decrypt.h"
[enc_string_enable /]
class Config {
public:
	void SaveConfig();
	void LoadConfig();
	
	int InGameFireKey = 191;
	
	float AdaptiveSmoothMultiplier = 1.0f;
	bool RiflesAdaptiveSmooth = false;
	bool PistolsAdaptiveSmooth = false;
	bool SnipersAdaptiveSmooth = false;
	bool SMGsAdaptiveSmooth = false;
	bool ShotGunsAdaptiveSmooth = false;
	bool LMGsAdaptiveSmooth = false;
	bool spiralAim = false;
	float spiralAimX = 1.3f;
	float spiralAimY = 3.7f;
	bool faceitAim = false;
	bool SaveTargetAim = false;
	bool bspParsing = false;
	bool skinchangerAutoUpdate = false;
	bool glowEspEnabled = false;
	bool aimbotEnabled = false;
	bool rageModeEnabled = false;
	bool triggerbotEnabled = false;
	bool bunnyhopEnabled = false;
	bool skinchangerEnabled = false;
	bool panicMode = false;
	
	bool needIndexes = true;
	
	struct keys {
		int aimbot_toggle;
		int ragemode_toggle;
		int aimbot_hold;
		int glowesp_toggle;
		int triggerbot_hold;
		int bunnyhop_hold;
		int skinchanger_toggle;
	}keys;
	
	struct aimbot {
		[swap_lines]
		bool enable = false;
		int startbullet = 0;
		int endbullet = 0;
		float delay = 0.f;
		float fov = 0.f;
		float smooth = 0.f;
		int bone = 0;
		float rcsAmount = 0.f;
		float killdelay = 0.f;
		[/swap_lines]
	}aimbot[100];
	
	struct skin {
		[swap_lines]
		int id = 0;
		int statTrack = 0;
		[/swap_lines]
	}skin[100];
	
	struct glowesp {
		int alpha;
		bool onlyVisible;
		bool bodycolorenable;
		int bodyred;
		int bodygreen;
		int bodyblue;
		int bodyalpha;
	}glowesp;
	
	struct triggerbot {
		float delay;
	}triggerbot;
	
	int knifeModel = 0;
	
private:	
	char* WeaponType(int ID);
	
	std::unordered_map<int, std::string> m_mapWeapons =
	{
		{ 1, "deagle" }, { 2, "dual_berettas" }, { 3, "five_seven" },
		{ 4, "glock"} , { 7, "ak47" }, { 8, "aug" },
		{ 9, "awp" }, { 10, "famas" }, { 11, "g3sg1" },
		{13, "galil"}, { 14, "m249"}, {16, "m4a4"},
		{17, "mac10"}, {19, "p90"}, {24, "ump45"},
		{25, "xm1014"}, {26, "bizon"}, {27, "mag7"},
		{28, "negev"}, {29, "sawed_off"}, {30, "tec9"},
		{31, "zeus"}, {32, "p2000"}, {33, "mp7"},
		{34, "mp9"}, {35, "nova"}, {36, "p250"},
		{38, "scar-20"}, {39, "ssg553"}, {40, "scout"},
		{60, "m4a1s"}, {61, "usp"}, {63, "cz75"},
		{64, "r8"}
	};
};

extern Config* cfg;