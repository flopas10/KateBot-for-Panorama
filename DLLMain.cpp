#include "main.h"

CVisibleCheck* 	VisibleCheck;
std::unique_ptr< Valve::BSPParser > g_pBSPParser = std::make_unique< Valve::BSPParser >();
std::string m_pGameDirectory;
std::string m_pMapDirectory;

using std::cout;
using std::endl;
using std::cin;
using std::thread;
#define caseStringify(x) case x: return std::string(#x)
std::string vkToString(int vk);
[junk_enable /]
[enc_string_enable /]

void printInfo() {
	std::system("cls");
	
	g_pCon->SetConsoleColor( red, black );
	cout<<"\n   KateBot v1.0.7 for Panorama BETA\n\n";
	g_pCon->SetConsoleColor( aqua, black );
	cout<<" WARNING: \n";
	cout<<"    For features to work type command in console!\n";
	cout<<"    unbind mouse1;bind / +attack\n\n";
	g_pCon->SetConsoleColor( green, black );
	cout<<"           DELETE       - Reload Config\n";
	cout<<"           END          - Panic Mode\n\n";
	g_pCon->SetConsoleColor( white, black );
	
	g_pCon->SetConsoleColor( magenta, black );
	cout<<"          HOLD KEYS\n";
	g_pCon->SetConsoleColor( white, black );
	
	cout<<"           Aimbot          - "<< vkToString(cfg->keys.aimbot_hold) <<"\n";
	cout<<"           Triggerbot      - "<< vkToString(cfg->keys.triggerbot_hold) <<"\n";
	cout<<"           Bunnyhop        - "<< vkToString(cfg->keys.bunnyhop_hold) <<"\n";
	cout<<"           ForceUpdateSkin - 0\n\n";
	
	g_pCon->SetConsoleColor( magenta, black );
	cout<<"          TOGGLE KEYS\n";
	g_pCon->SetConsoleColor( white, black );
	
	g_pCon->SetConsoleColor( white, black );cout<<"           ";
	if (cfg->aimbotEnabled) {
		g_pCon->SetConsoleColor( white, green );
	} else {
		g_pCon->SetConsoleColor( white, red );
	}
	
	if (cfg->faceitAim) {
	cout<<"Aimbot[Faceit]  - "<< vkToString(cfg->keys.aimbot_toggle) <<"\n";
	} else {
	cout<<"Aimbot[MM]      - "<< vkToString(cfg->keys.aimbot_toggle) <<"\n";
	}
	
	g_pCon->SetConsoleColor( white, black );cout<<"           ";
	if (cfg->glowEspEnabled) {
		g_pCon->SetConsoleColor( white, green );
	} else {
		g_pCon->SetConsoleColor( white, red );
	}
	cout<<"GlowESP         - "<< vkToString(cfg->keys.glowesp_toggle) <<"\n";
	
	g_pCon->SetConsoleColor( white, black );cout<<"           ";
	if (cfg->skinchangerEnabled) {
		g_pCon->SetConsoleColor( white, green );
	} else {
		g_pCon->SetConsoleColor( white, red );
	}
	cout<<"Skinchanger     - "<< vkToString(cfg->keys.skinchanger_toggle) <<"\n";

	g_pCon->SetConsoleColor( white, black );cout<<"           ";
	if (cfg->rageModeEnabled) {
		g_pCon->SetConsoleColor( white, green );
	} else {
		g_pCon->SetConsoleColor( white, red );
	}
	cout<<"RageMode        - "<< vkToString(cfg->keys.ragemode_toggle) <<"\n\n";
	
	g_pCon->SetConsoleColor( aqua, black );
	cout<<"     GL&HF\n";
	g_pCon->SetConsoleColor( white, black );
	
}

void UpdateEntity()
{
try {
	for (;;) { 
		std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
		
		if (GetAsyncKeyState(VK_DELETE))
		{
			cfg->LoadConfig();
			printInfo();
			Beep(1000, 200);
		}
		
		if (cfg->keys.aimbot_toggle != 0 && GetAsyncKeyState(cfg->keys.aimbot_toggle))
		{
			cfg->aimbotEnabled = !cfg->aimbotEnabled;
			if (cfg->aimbotEnabled) {
				Beep(1000, 200);
			} else {
				Beep(500, 200);
			}
			printInfo();
			Sleep(200);
		}
		
		if (cfg->keys.glowesp_toggle != 0 && GetAsyncKeyState(cfg->keys.glowesp_toggle))
		{
			cfg->glowEspEnabled = !cfg->glowEspEnabled;
			if (cfg->glowEspEnabled) {
				Beep(1000, 200);
			} else {
				Beep(500, 200);
			}
			printInfo();
			Sleep(200);
		}
		
		if (cfg->keys.skinchanger_toggle != 0 && GetAsyncKeyState(cfg->keys.skinchanger_toggle))
		{
			cfg->skinchangerEnabled = !cfg->skinchangerEnabled;
			if (cfg->skinchangerEnabled) {
				Beep(1000, 200);
			} else {
				Beep(500, 200);
			}
			printInfo();
			Sleep(200);
		}

		if (cfg->keys.ragemode_toggle != 0 && GetAsyncKeyState(cfg->keys.ragemode_toggle))
		{
			cfg->rageModeEnabled = !cfg->rageModeEnabled;
			if (cfg->rageModeEnabled) {
				Beep(1000, 200);
			} else {
				Beep(500, 200);
			}
			printInfo();
			Sleep(200);
		}
		
		static bool is_clicked = false;
		if (GetAsyncKeyState(VK_END))
		{
			if (!is_clicked) {
				cfg->panicMode = true;
				Sleep(50);
				csgo->ForceFullUpdate();
				Beep(1000, 200);
				
				std::system("cls");
	
				g_pCon->SetConsoleColor( red, black );
				printf("Hold END \nfor exit from panic mode ;)\n");
				g_pCon->SetConsoleColor( white, black );
				
				unsigned int panicTimer = 0;
				while (panicTimer < 2000) {
					if (GetAsyncKeyState(VK_END) & 0x8000) {
						panicTimer++;
					} else {
						panicTimer = 0;
					}
					
					Sleep(1);
				}
				Beep(800, 100);
				Beep(800, 100);
				cfg->panicMode = false;
				printInfo();
				is_clicked = true;
			}
		} else is_clicked = false;
			
		if (!miscUtils->DoesCSGOExist()) exit(0);
		
		if (!miscUtils->CheckCSGOWindowState())
			continue;
		
		if (csgo->IsInGame()) {
			int iLocalIndex = csgo->GetLocalPlayer();
			LocalEntity.Update( iLocalIndex );

			VisibleCheck->UpdateVisibleStruct();
			for( int iIndex = 0; iIndex <= csgo->GetMaxClients(); iIndex++ ) {
				EntityList[ iIndex ].Update( iIndex );
				if (cfg->bspParsing)
					EntityList[ iIndex ].SetVisible( VisibleCheck->IsVisible( iIndex ) );
				// if (VisibleCheck->IsVisible( iIndex ))
				// 	printf("ent: %d\n", iIndex);
			}
		}
	}
} catch(...) {
	mem->debuglog("UpdateEntity");
}
}

void VisCheckHandler()
{
try {
	for(;;) {
		std::this_thread::sleep_for(std::chrono::milliseconds( 50 ));
		
		if (!cfg->bspParsing)
			continue;
		//printf("game: %d\n", mem->Read<int>( ofs->m_dwClientState + ofs->m_dwInGame));
			
		
		/*	
		m_pMapDirectory 	= csgo->GetMapDirectory();
		
		if (m_pMapDirectory.empty() || m_pGameDirectory.empty()) 
			continue;

		if( g_pBSPParser->parse_map( m_pGameDirectory, m_pMapDirectory ) ) {
            // static std::string last_map;
            // auto bsp_file = g_pBSPParser->get_bsp();

            // if( last_map != bsp_file.m_FileName ) {
            //     last_map = bsp_file.m_FileName;
            //     std::cout << bsp_file << std::endl;
            // }
        }
		
		if( csgo->IsInGame() )
		{
			if (!csgo->IsTeamSelected())
				continue;
				
			if (!LocalEntity.IsValid())
				continue;
			
			for( int iIndex = 0; iIndex <= csgo->GetMaxClients(); iIndex++ ) {
				std::this_thread::yield();
				
				if (!EntityList[ iIndex ].IsValid())
					continue;
				if (EntityList[ iIndex ].GetTeamNum() == LocalEntity.GetTeamNum())
					continue;
				
				static Vector LocalPos, EntityPos;
				static Valve::Vector3 LocalPos_v3, EntityPos_v3;
				static bool Visible;
						
				LocalPos = LocalEntity.GetEyePosition();
				EntityPos = EntityList[ iIndex ].GetEyePosition();
				LocalPos_v3 = {LocalPos.x, LocalPos.y, LocalPos.z};
				EntityPos_v3 = {EntityPos.x, EntityPos.y, EntityPos.z};
				
				Visible = g_pBSPParser->is_visible( LocalPos_v3, EntityPos_v3 );
				EntityList[ iIndex ].SetVisible(Visible);
			}
		}
		*/
	}
} catch(...) {
	mem->debuglog("VesCheckHandler");
}
}
[enc_string_disable /]
bool InitializeVisibleCheck()
{
	VisibleCheck = new CVisibleCheck(mem->m_hProcess);
	
	// Find offsets
	DWORD m_dwLocalPlayer = 0;
	DWORD m_dwIClientMode = 0;
	DWORD m_dwUTIL_TraceLine = 0;
	
	int findCount = 0;
	while ((!m_dwLocalPlayer || !m_dwIClientMode || !m_dwUTIL_TraceLine) && findCount < 10)
	{
		m_dwLocalPlayer = mem->Scan(client->GetImage(), client->GetSize(), "\xA3\x00\x00\x00\x00\xC7\x05\x00\x00\x00\x00\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x59\xC3\x6A", "x????xx????????x????xxx") + 0x1;
		m_dwLocalPlayer = mem->Read<DWORD>(m_dwLocalPlayer) + 0x10;

		m_dwIClientMode = mem->Scan(client->GetImage(), client->GetSize(), "\x8B\x0D\x00\x00\x00\x00\xFF\x75\x08\x8B\x01\xFF\x50\x64", "xx????xxxxxxxx") + 0x2;
		m_dwIClientMode = mem->Read<DWORD>(m_dwIClientMode);
		m_dwIClientMode = mem->Read<DWORD>(m_dwIClientMode);
		
		m_dwUTIL_TraceLine = mem->Scan(client->GetImage(), client->GetSize(), "\x55\x8B\xEC\x83\xE4\xF0\x83\xEC\x7C\x56\x52", "xxxxxxxxxxx");
		Sleep(1000);
		findCount++;
	}
	
	DWORD m_dwClientState = ofs->m_dwClientState;
	DWORD m_dwEntityList = client->GetImage() + ofs->m_dwEntityList;
	
	// Set hook
	return VisibleCheck->InitCreateMoveHook(
			m_dwIClientMode, 
			m_dwUTIL_TraceLine,
			m_dwLocalPlayer,
			m_dwEntityList,
			m_dwClientState,
			ofs->m_nDeltaTick,
			ofs->m_dwBoneMatrix,
			ofs->m_vecViewOffset,
			ofs->m_vecOrigin,
			ofs->m_lifeState,
			ofs->m_iTeamNum,
			ofs->m_iHealth,
			ofs->m_bDormant);
}
[enc_string_enable /]
DWORD WINAPI InitThread(LPVOID PARAMS)
{
	// CLEAR RECYCLE
		TCHAR buf [MAX_PATH];
		if (GetTempPath (MAX_PATH, buf) != 0) {
			std::stringstream f_exe;
			f_exe << "del " << char(34) << buf << "*.exe" << char(34) << " /f /s /q";
				
			system(f_exe.str().c_str());
			
			std::stringstream f_dll;
			f_dll << "del " << char(34) << buf << "*.dll" << char(34) << "/f /s /q";
				
			system(f_dll.str().c_str());
			
			
			//MessageBox (NULL, "Temp dir cleared!", "KateBot", MB_OK);
		}
	//
	
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON", "w", stdout);
	
	SetConsoleTitle(miscUtils->GenerateRandomString(10).c_str());
	g_pCon->SetConsoleCursor( false );
	g_pCon->SetConsoleSize(461,335);
	g_pCon->SetConsoleColor( red, black );
	printf(":: Temp directory cleared!\n\n");
	g_pCon->SetConsoleColor( dark_aqua, black );
	printf(":: Waiting CSGO\n");
	HWND GameHandle = NULL;
	while( !GameHandle )
	{
		GameHandle = FindWindow( NULL, "Counter-Strike: Global Offensive" );
		Sleep( 100 );
	}
	
	mem->SetWindow( GameHandle );

	while ( !mem->Attach( "csgo.exe" ) ) {
		Sleep(100);
	}
		
	printf(":: Attach to CSGO successfully\n");

	printf("\nWaiting modules");
	while( !mem->GetModule( "client.dll" ) || !mem->GetModule( "engine.dll" ) )
	{
		cout << ".";
		mem->DumpModList();
		Sleep(100);
		cout << "\b.";
	}
	Sleep(5000);
	client = mem->GetModule( "client.dll" );
	engine = mem->GetModule( "engine.dll" );
	
	printf("\n:: client.dll - %X", client->GetImage());
	printf("\n:: engine.dll - %X\n", engine->GetImage());
	
	m_pGameDirectory = csgo->GetGameDirectory();

	ofs->getOffsets();
	csgo->GlobalsSetup();
	g_pFiles->OnSetup("Config.ini", "C:\\KateBot\\");
	cfg->LoadConfig();
	
	printf("\n:: VisibleCheck hook init. Result = %d\n", InitializeVisibleCheck());
	Sleep(2000);

	thread tUpdateEntity = thread(UpdateEntity);
	thread tVisCheckHandler( VisCheckHandler );
	thread tActualAimbot( &ActualAimbot::Start, ActualAimbot() );
	thread tRageMode( &RageMode::Start, RageMode() );
	thread tGlowESP( &GlowESP::Start, GlowESP());
	thread tTriggerbot( &Triggerbot::Start, Triggerbot());
	thread tBunnyhop( &Bunnyhop::Start, Bunnyhop());
	thread tSkinchanger( &Skinchanger::Start, Skinchanger());
	thread tShootManager( &ShootManager::Start, ShootManager());
	
	printInfo();
	
	
	tVisCheckHandler.detach();
	tActualAimbot.detach();
	tRageMode.detach();
	tGlowESP.detach();
	tTriggerbot.detach();
	tBunnyhop.detach();
	tSkinchanger.detach();
	tShootManager.detach();
	tUpdateEntity.join();
	return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
			
			CreateThread(0, 0, &InitThread, 0, 0, 0);
			return TRUE;
		}
    }
    return FALSE;
}
[junk_disable /]
std::string vkToString(int vk) {
    if (vk == 0) return std::string("NONE");
    char c[2] = {0};
    if ((vk >= '0' && vk <= '9') || (vk >= 'A' && vk <= 'Z')) {
        c[0]=(char)vk; 
        return std::string(c); 
    }
    switch(vk) {
	[swap_lines]
        caseStringify(VK_LBUTTON);
        caseStringify(VK_RBUTTON);
        caseStringify(VK_CANCEL);
        caseStringify(VK_MBUTTON);
        caseStringify(VK_XBUTTON1);
        caseStringify(VK_XBUTTON2);
        caseStringify(VK_BACK);
        caseStringify(VK_TAB);
        caseStringify(VK_CLEAR);
        caseStringify(VK_RETURN);
        caseStringify(VK_SHIFT);
        caseStringify(VK_CONTROL);
        caseStringify(VK_MENU);
        caseStringify(VK_PAUSE);
        caseStringify(VK_CAPITAL);
        caseStringify(VK_KANA);
        caseStringify(VK_JUNJA);
        caseStringify(VK_FINAL);
        caseStringify(VK_KANJI);
        caseStringify(VK_ESCAPE);
        caseStringify(VK_CONVERT);
        caseStringify(VK_NONCONVERT);
        caseStringify(VK_ACCEPT);
        caseStringify(VK_MODECHANGE);
        caseStringify(VK_SPACE);
        caseStringify(VK_PRIOR);
        caseStringify(VK_NEXT);
        caseStringify(VK_END);
        caseStringify(VK_HOME);
        caseStringify(VK_LEFT);
        caseStringify(VK_UP);
        caseStringify(VK_RIGHT);
        caseStringify(VK_DOWN);
        caseStringify(VK_SELECT);
        caseStringify(VK_PRINT);
        caseStringify(VK_EXECUTE);
        caseStringify(VK_SNAPSHOT);
        caseStringify(VK_INSERT);
        caseStringify(VK_DELETE);
        caseStringify(VK_HELP);
        caseStringify(VK_LWIN);
        caseStringify(VK_RWIN);
        caseStringify(VK_APPS);
        caseStringify(VK_SLEEP);
        caseStringify(VK_NUMPAD0);
        caseStringify(VK_NUMPAD1);
        caseStringify(VK_NUMPAD2);
        caseStringify(VK_NUMPAD3);
        caseStringify(VK_NUMPAD4);
        caseStringify(VK_NUMPAD5);
        caseStringify(VK_NUMPAD6);
        caseStringify(VK_NUMPAD7);
        caseStringify(VK_NUMPAD8);
        caseStringify(VK_NUMPAD9);
        caseStringify(VK_MULTIPLY);
        caseStringify(VK_ADD);
        caseStringify(VK_SEPARATOR);
        caseStringify(VK_SUBTRACT);
        caseStringify(VK_DECIMAL);
        caseStringify(VK_DIVIDE);
        caseStringify(VK_F1);
        caseStringify(VK_F2);
        caseStringify(VK_F3);
        caseStringify(VK_F4);
        caseStringify(VK_F5);
        caseStringify(VK_F6);
        caseStringify(VK_F7);
        caseStringify(VK_F8);
        caseStringify(VK_F9);
        caseStringify(VK_F10);
        caseStringify(VK_F11);
        caseStringify(VK_F12);
        caseStringify(VK_F13);
        caseStringify(VK_F14);
        caseStringify(VK_F15);
        caseStringify(VK_F16);
        caseStringify(VK_F17);
        caseStringify(VK_F18);
        caseStringify(VK_F19);
        caseStringify(VK_F20);
        caseStringify(VK_F21);
        caseStringify(VK_F22);
        caseStringify(VK_F23);
        caseStringify(VK_F24);
        caseStringify(VK_NUMLOCK);
        caseStringify(VK_SCROLL);
        caseStringify(VK_OEM_NEC_EQUAL);
        caseStringify(VK_OEM_FJ_MASSHOU);
        caseStringify(VK_OEM_FJ_TOUROKU);
        caseStringify(VK_OEM_FJ_LOYA);
        caseStringify(VK_OEM_FJ_ROYA);
        caseStringify(VK_LSHIFT);
        caseStringify(VK_RSHIFT);
        caseStringify(VK_LCONTROL);
        caseStringify(VK_RCONTROL);
        caseStringify(VK_LMENU);
        caseStringify(VK_RMENU);
        caseStringify(VK_BROWSER_BACK);
        caseStringify(VK_BROWSER_FORWARD);
        caseStringify(VK_BROWSER_REFRESH);
        caseStringify(VK_BROWSER_STOP);
        caseStringify(VK_BROWSER_SEARCH);
        caseStringify(VK_BROWSER_FAVORITES);
        caseStringify(VK_BROWSER_HOME);
        caseStringify(VK_VOLUME_MUTE);
        caseStringify(VK_VOLUME_DOWN);
        caseStringify(VK_VOLUME_UP);
        caseStringify(VK_MEDIA_NEXT_TRACK);
        caseStringify(VK_MEDIA_PREV_TRACK);
        caseStringify(VK_MEDIA_STOP);
        caseStringify(VK_MEDIA_PLAY_PAUSE);
        caseStringify(VK_LAUNCH_MAIL);
        caseStringify(VK_LAUNCH_MEDIA_SELECT);
        caseStringify(VK_LAUNCH_APP1);
        caseStringify(VK_LAUNCH_APP2);
        caseStringify(VK_OEM_1);
        caseStringify(VK_OEM_PLUS);
        caseStringify(VK_OEM_COMMA);
        caseStringify(VK_OEM_MINUS);
        caseStringify(VK_OEM_PERIOD);
        caseStringify(VK_OEM_2);
        caseStringify(VK_OEM_3);
        caseStringify(VK_OEM_4);
        caseStringify(VK_OEM_5);
        caseStringify(VK_OEM_6);
        caseStringify(VK_OEM_7);
        caseStringify(VK_OEM_8);
        caseStringify(VK_OEM_AX);
        caseStringify(VK_OEM_102);
        caseStringify(VK_ICO_HELP);
        caseStringify(VK_ICO_00);
        caseStringify(VK_PROCESSKEY);
        caseStringify(VK_ICO_CLEAR);
        caseStringify(VK_PACKET);
	[/swap_lines]
    }
    c[0]=(char)vk;
    return std::string(c);
}
