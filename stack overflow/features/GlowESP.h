#ifndef _GLOWESP_H_
#define _GLOWESP_H_

#include "..\main.h"
[junk_enable /]	

class GlowESP
{
public:
	void Start()
	{
	try{
		for (;;) {
			std::this_thread::sleep_for(std::chrono::milliseconds(25));
			
			if (cfg->panicMode)
				continue;
				
			if (!miscUtils->IsCSGOActiveWindow())
				continue;
				
			if (!cfg->glowEspEnabled)
				continue;
				
			if (!csgo->IsInGame())
				continue;			
			
			DWORD GlowPointer = mem->Read<DWORD>(client->GetImage() + ofs->m_dwGlowObject);

			for (int g = 0; g < csgo->GetMaxObjects(); g++) {
				
				DWORD Entity = mem->Read<DWORD>(client->GetImage() + ofs->m_dwEntityList + g * 0x10);
						
				if (Entity != 0) {
					int EntityClassID = mem->Read<int>(mem->Read<int>(mem->Read<int>(mem->Read<int>(Entity + 0x8) + 2 * 0x4) + 0x1) + 20);
					if (EntityClassID == 36)
					{
						indexBomb = mem->Read<int>(Entity + ofs->m_iPlayerC4);
					}
				}
				
				DWORD glowBase = GlowPointer + (g * sizeof(GlowObjectDefinition_t));
				glowObj = mem->Read<GlowObjectDefinition_t>(glowBase);
				
				if (GlowPointer != NULL && glowObj.dwBase != NULL)
				{				
					int ClassID = mem->Read<int>(mem->Read<int>(mem->Read<int>(mem->Read<int>(glowObj.dwBase + 0x8) + 2 * 0x4) + 0x1) + 20);
					
					if (ClassID == 35) {
						bool IsDormant = mem->Read<bool>(glowObj.dwBase + ofs->m_bDormant);
						
						if (!IsDormant)
						{
							int TeamNum = mem->Read<int>(glowObj.dwBase + ofs->m_iTeamNum); 
							int LocalPlayer_TeamNum = LocalEntity.GetTeamNum();
							
							if (TeamNum != LocalPlayer_TeamNum)
							{
								int Health = mem->Read<int>(glowObj.dwBase + ofs->m_iHealth); 
								
								if (TeamNum != 2 && TeamNum != 3)
									continue;
								
								int Index = mem->Read<int>(glowObj.dwBase + ofs->m_dwIndex);
								
								bool isDefusing = false;
								if (TeamNum == 3) 
									isDefusing = mem->Read<bool>(glowObj.dwBase + ofs->m_bIsDefusing);
									
								if (Index == indexBomb || isDefusing)
								{
									DrawGlow(glowBase, CBlue );
								}
								else if (Health <= 100 && Health > 75)
								{
									DrawGlow(glowBase, CGreen );
								}
								else if (Health <= 75 && Health > 50)
								{
									DrawGlow(glowBase, CYellow);
								}
								else if (Health <= 50 && Health > 25)
								{
									DrawGlow(glowBase, COrange);
								}
								else if (Health <= 25 && Health > 1)
								{
									DrawGlow(glowBase, CRed);
								}
								else if (Health <= 1 && Health > 0)
								{
									DrawGlow(glowBase, CWhite);
								}
							}
						}
					}
				}
			}
		}
	} catch(...) {
		mem->debuglog(__FILE__);
	}
	}

private:
[junk_disable /]	
	int indexBomb = -1;

	int getEntityID(DWORD glowBase)
	{
		DWORD dwBase = mem->Read< DWORD >(glowBase);
	    for (int iIndex = 0; iIndex < csgo->GetMaxClients(); iIndex++) {
	        [add_junk /]
	        if (dwBase == EntityList[ iIndex ].GetPointer())
	        {
	            return iIndex;
	        }
	    }
	    return -1;
	}
	
	struct GlowColor
	{
		float r;
		float g;
		float b;
		float a;
	};
	
	GlowColor CGreen = { 0.f, 0.4f, 0.f, 1.f};
	GlowColor CYellow = { 0.4f, 0.4f, 0.f, 1.f};
	GlowColor CRed = { 0.4f, 0.f, 0.f, 1.f};
	GlowColor CBlue = { 0.f, 0.f, 0.4f, 1.f};
	GlowColor COrange = { 1.f, 0.5, 0.f, 1.f};
	GlowColor CPink = { 1.f, 0.55f, 0.7f, 1.f};
	GlowColor CWhite = { 1.f, 1.f, 1.f, 1.f};
	[junk_enable /]	
	void DrawGlow(DWORD dwBase, GlowColor Color)
	{
		glowObj.r = Color.r;
		glowObj.g = Color.g;
		glowObj.b = Color.b;
		//glowObj.a = Color.a;
		glowObj.a = sanitizeChannel(cfg->glowesp.alpha);
		glowObj.m_bRenderWhenOccluded = true;
		glowObj.m_bRenderWhenUnoccluded = false;
		glowObj.m_bFullBloom = false;
		
		mem->Write<GlowObjectDefinition_t>(dwBase, glowObj);

		// int entID = getEntityID(dwBase);
		// bool entVisible = entID != -1 ? EntityList[ entID ].IsVisible() : false;
		
		// if (cfg->glowesp.onlyVisible || entVisible)
		if (cfg->glowesp.onlyVisible)
			mem->Write<bool>(dwBase + 0x2C, true);//(OnlyVisible)
		
		if (cfg->glowesp.bodycolorenable && csgo->IsTeamSelected())
			mem->Write(glowObj.dwBase + 0x70, ColorESP(cfg->glowesp.bodyred * 1.0f, cfg->glowesp.bodygreen * 1.0f, cfg->glowesp.bodyblue * 1.0f, cfg->glowesp.bodyalpha * 1.0f));
	}
	
	float sanitizeChannel(int value) {
        if (value > 255) value = 255;
        if (value < 0) value = 0;
        return (float) value / 255;
	}
	
	[junk_disable /]	
	struct GlowObjectDefinition_t
	{
		DWORD dwBase;
		float r;
		float g;
		float b;
		float a;
		uint8_t unk1[16];
		bool m_bRenderWhenOccluded;
		bool m_bRenderWhenUnoccluded;
		bool m_bFullBloom;
		uint8_t unk2[14];
	}glowObj;
};
	
#endif