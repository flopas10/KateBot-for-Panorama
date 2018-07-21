#include "..\main.h"

#define BUFSIZE 1024
#define MD5LEN  16
[junk_enable /]	
[enc_string_enable /]
CMiscUtils* miscUtils = new CMiscUtils();

#define clamp(val, a, b) { if(val < a) val = a; if(val > b) val = b; }

std::string CMiscUtils::GenerateRandomString(int length)
{
[enc_string_disable /]
	static const char alphanum[] =
		"0123456789"
		"!@#$%^&*"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	std::string str;
	srand(time(0));
	for (int i = 0; i < length; i++) {
		str += alphanum[rand() % sizeof(alphanum) - 1];
	}

	return str;
}

void CMiscUtils::GenerateRandomWindowTitle(int stringLength)
{
	SetConsoleTitle(GenerateRandomString(stringLength).c_str());
}

void CMiscUtils::DeleteSelf(char* ProgramPath)
{
	// Windows keeps .exe files open during execution...
	char* batPath = new char[strlen(ProgramPath) + 5];
	strcpy_s(batPath, strlen(ProgramPath) + 5, ProgramPath);
	strcat_s(batPath, strlen(ProgramPath) + 5, ".bat");

	const char* batFormat =
		":Repeat\n"
		"del \"%s\"\n"
		"if exist \"%s\" goto Repeat\n"
		"del \"%s\"\n";

	char* batFile = new char[strlen(batPath) + strlen(batFormat) + strlen(ProgramPath) * 2];
	sprintf_s(batFile, strlen(batPath) + strlen(batFormat) + strlen(ProgramPath) * 2,
			  batFormat, ProgramPath, ProgramPath, batPath);

	FILE* f;
	fopen_s(&f, batPath, "w");
	if (f != NULL)
	{
		fwrite(batFile, strlen(batFile), 1, f);
		fclose(f);
	}

	STARTUPINFOA startupInfo;
	PROCESS_INFORMATION procInfo;
	memset(&startupInfo, 0, sizeof(startupInfo));

	startupInfo.cb = sizeof(startupInfo);

	CreateProcess(batPath, NULL, NULL, NULL, FALSE, 0, NULL, NULL,
				  &startupInfo, &procInfo);

	delete[] batFile;
	delete[] batPath;
}
[enc_string_enable /]
void CMiscUtils::AllowDebugging()
{
	HANDLE _HandleProcess = GetCurrentProcess();
	HANDLE _HandleToken;
	TOKEN_PRIVILEGES tkPrivileges;
	LUID _LUID;

	OpenProcessToken(_HandleProcess, TOKEN_ADJUST_PRIVILEGES, &_HandleToken);
	LookupPrivilegeValue(0, "seDebugPrivilege", &_LUID);

	tkPrivileges.PrivilegeCount = 1;
	tkPrivileges.Privileges[0].Luid = _LUID;
	tkPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(_HandleToken, false, &tkPrivileges, 0, 0, 0);
	CloseHandle(_HandleToken);
	CloseHandle(_HandleProcess);
}

void CMiscUtils::PrintLine(std::string s)
{
	printf("%s\n", s.c_str());
}

int CMiscUtils::GetKeyFromString(std::string str)
{
	char data = str.c_str()[0];

	if ((data >= '0' && data <= '9') || (data >= 'A' && data <= 'Z'))
		return (int)data;
[junk_disable /]	
	if (str == "mouse1") return VK_LBUTTON;
	else if (str == "mouse2") return VK_RBUTTON;
	else if (str == "mouse3") return VK_MBUTTON;
	else if (str == "mouse4") return VK_XBUTTON1;
	else if (str == "mouse5") return VK_XBUTTON2;
	else if (str == "backspace") return VK_BACK;
	else if (str == "tab") return VK_TAB;
	else if (str == "enter") return VK_RETURN;
	else if (str == "shift") return VK_SHIFT;
	else if (str == "ctrl") return VK_CONTROL;
	else if (str == "alt") return VK_MENU;
	else if (str == "capslock") return VK_CAPITAL;
	else if (str == "escape") return VK_ESCAPE;
	else if (str == "space") return VK_SPACE;
	else if (str == "pgup") return VK_PRIOR;
	else if (str == "pgdn") return VK_NEXT;
	else if (str == "end") return VK_END;
	else if (str == "home") return VK_HOME;
	else if (str == "leftarrow") return VK_LEFT;
	else if (str == "rightarrow") return VK_RIGHT;
	else if (str == "uparrow") return VK_UP;
	else if (str == "downarrow") return VK_DOWN;
	else if (str == "ins") return VK_INSERT;
	else if (str == "del") return VK_DELETE;
	else if (str == "numpad_0") return VK_NUMPAD0;
	else if (str == "numpad_1") return VK_NUMPAD1;
	else if (str == "numpad_2") return VK_NUMPAD2;
	else if (str == "numpad_3") return VK_NUMPAD3;
	else if (str == "numpad_4") return VK_NUMPAD4;
	else if (str == "numpad_5") return VK_NUMPAD5;
	else if (str == "numpad_6") return VK_NUMPAD6;
	else if (str == "numpad_7") return VK_NUMPAD7;
	else if (str == "numpad_8") return VK_NUMPAD8;
	else if (str == "numpad_9") return VK_NUMPAD9;
	else if (str == "kp_multiply") return VK_MULTIPLY;
	else if (str == "kp_plus") return VK_ADD;
	else if (str == "kp_minus") return VK_SUBTRACT;
	else if (str == "kp_slash") return VK_DIVIDE;
	else if (str == "f1") return VK_F1;
	else if (str == "f2") return VK_F2;
	else if (str == "f3") return VK_F3;
	else if (str == "f4") return VK_F4;
	else if (str == "f5") return VK_F5;
	else if (str == "f6") return VK_F6;
	else if (str == "f7") return VK_F7;
	else if (str == "f8") return VK_F8;
	else if (str == "f9") return VK_F9;
	else if (str == "f10") return VK_F10;
	else if (str == "f11") return VK_F11;
	else if (str == "f12") return VK_F12;
	else if (str == ";") return VK_OEM_1;
	else if (str == "+") return VK_OEM_PLUS;
	else if (str == "-") return VK_OEM_MINUS;
	else if (str == ",") return VK_OEM_COMMA;
	else if (str == ".") return VK_OEM_PERIOD;
	else if (str == "/") return VK_OEM_2;
	else if (str == "~") return VK_OEM_3;
	else if (str == "[") return VK_OEM_4;
	else if (str == std::to_string(char(0x5C))) return VK_OEM_5;
	else if (str == "]") return VK_OEM_6;
	else if (str == std::to_string(char(0x22))) return VK_OEM_7;
	else return -1;

}
[junk_enable /]	

std::string CMiscUtils::GetStringFromKey(int key)
{
	char c[2] = { 0 };

	if ((key >= '0' && key <= '9') || (key >= 'A' && key <= 'Z'))
	{
		c[0] = (char)key;
		std::string sweg(c);
		std::transform(sweg.begin(), sweg.end(), sweg.begin(), ::tolower);
		return sweg;
	}
[junk_disable /]	
	switch (key)
	{
		case VK_LBUTTON: return std::string("mouse1");
		case VK_RBUTTON: return std::string("mouse2");
		case VK_MBUTTON: return std::string("mouse3");
		case VK_XBUTTON1: return std::string("mouse4");
		case VK_XBUTTON2: return std::string("mouse5");
		case VK_BACK: return std::string("backspace");
		case VK_TAB: return std::string("tab");
		case VK_RETURN: return std::string("enter");
		case VK_SHIFT: return std::string("shift");
		case VK_CONTROL: return std::string("ctrl");
		case VK_MENU: return std::string("alt");
		case VK_CAPITAL: return std::string("capslock");
		case VK_ESCAPE: return std::string("escape");
		case VK_SPACE: return std::string("space");
		case VK_PRIOR: return std::string("pgup");
		case VK_NEXT: return std::string("pgdn");
		case VK_END: return std::string("end");
		case VK_HOME: return std::string("home");
		case VK_LEFT: return std::string("leftarrow");
		case VK_UP: return std::string("uparrow");
		case VK_DOWN: return std::string("downarrow");
		case VK_RIGHT: return std::string("rightarrow");
		case VK_INSERT: return std::string("ins");
		case VK_DELETE: return std::string("del");
		case VK_NUMPAD0: return std::string("numpad_0");
		case VK_NUMPAD1: return std::string("numpad_1");
		case VK_NUMPAD2: return std::string("numpad_2");
		case VK_NUMPAD3: return std::string("numpad_3");
		case VK_NUMPAD4: return std::string("numpad_4");
		case VK_NUMPAD5: return std::string("numpad_5");
		case VK_NUMPAD6: return std::string("numpad_6");
		case VK_NUMPAD7: return std::string("numpad_7");
		case VK_NUMPAD8: return std::string("numpad_8");
		case VK_NUMPAD9: return std::string("numpad_9");
		case VK_MULTIPLY: return std::string("kp_multiply");
		case VK_ADD: return std::string("kp_plus");
		case VK_SUBTRACT: return std::string("kp_minus");
		case VK_DIVIDE: return std::string("kp_slash");
		case VK_F1: return std::string("f1");
		case VK_F2: return std::string("f2");
		case VK_F3: return std::string("f3");
		case VK_F4: return std::string("f4");
		case VK_F5: return std::string("f5");
		case VK_F6: return std::string("f6");
		case VK_F7: return std::string("f7");
		case VK_F8: return std::string("f8");
		case VK_F9: return std::string("f9");
		case VK_F10: return std::string("f10");
		case VK_F11: return std::string("f11");
		case VK_F12: return std::string("f12");
		case VK_OEM_1: return std::string(";");
		case VK_OEM_PLUS: return std::string("+");
		case VK_OEM_MINUS: return std::string("-");
		case VK_OEM_COMMA: return std::string(",");
		case VK_OEM_PERIOD: return std::string(".");
		case VK_OEM_2: return std::string("/");
		case VK_OEM_3: return std::string("~");
		case VK_OEM_4: return std::string("[");
		case VK_OEM_5: return std::to_string(char(0x5C));
		case VK_OEM_6: return std::string("]");
		case VK_OEM_7: return std::to_string(char(0x22));
		default: return std::string("unknown key");
	}
}
[junk_enable /]	
void CMiscUtils::ParseColor(std::string str, Vector& clr)
{
	LPTSTR pStop;
	int color = _tcstol(str.c_str(), &pStop, 16);
	clr.x = ((color & 0xFF0000) >> 16) / 255.0f;
	clr.y = ((color & 0xFF00) >> 8) / 255.0f;
	clr.z = (color & 0xFF) / 255.0f;
}

std::string CMiscUtils::GetStringFromColor(Vector color)
{
	color *= 255.0f;
	int r = (((int)color.x & 0xFF) << 16);
	int g = (((int)color.y & 0xFF) << 8);
	int b = (((int)color.z & 0xFF));
	std::stringstream s;
	s << std::hex << std::uppercase << (r | g | b);
	return s.str();
}

bool CMiscUtils::IsActiveWindow()
{
	char windowTitle[256];
	char consoleTitle[256];
	HWND hwnd = GetForegroundWindow();
	GetWindowText(hwnd, windowTitle, sizeof(windowTitle));
	GetConsoleTitle(consoleTitle, sizeof(consoleTitle));

	return (!strcmp(windowTitle, consoleTitle));
}

bool CMiscUtils::IsCSGOActiveWindow()
{
	return csgoActiveWindow;
}

bool CMiscUtils::CheckCSGOWindowState()
{
	char windowTitle[256];
	HWND hwnd = GetForegroundWindow();
	GetWindowText(hwnd, windowTitle, sizeof(windowTitle));
	
	csgoActiveWindow = !strcmp(windowTitle, "Counter-Strike: Global Offensive");
	return csgoActiveWindow;
}

bool CMiscUtils::DoesCSGOExist()
{
	HWND hwnd = FindWindow(NULL, "Counter-Strike: Global Offensive");
	return (hwnd != NULL);
}

HWND CMiscUtils::GetGameWindow()
{
	return FindWindow(NULL, "Counter-Strike: Global Offensive");
}

void CMiscUtils::DisableQuickEdit()
{
	HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);
	DWORD consoleMode;
	GetConsoleMode(hConsole, &consoleMode);
	consoleMode &= ~ENABLE_QUICK_EDIT_MODE;
	SetConsoleMode(hConsole, consoleMode);
}

std::string CMiscUtils::GetHash(std::string fileName)
{
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	HCRYPTPROV hProv;
	HCRYPTHASH hHash;

	DWORD cbHash = MD5LEN;
	BYTE rgbHash[MD5LEN];
	[enc_string_disable /]
	CHAR rgbDigits[] = "0123456789abcdef";
[enc_string_enable /]
	if (hFile == INVALID_HANDLE_VALUE)
	{
		int dwStatus = GetLastError();
		CMiscUtils::PrintLine(std::string("Error reading in .ini"));
		CloseHandle(hFile);
		return std::string("ERROR");
	}

	// Get handle to the crypto provider
	if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
	{
		int dwStatus = GetLastError();
		CMiscUtils::PrintLine(std::string("Error acquiring CryptAcquireContext"));
		CloseHandle(hFile);
		return std::string("ERROR");
	}

	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
	{
		int dwStatus = GetLastError();
		CMiscUtils::PrintLine(std::string("Error creating hash"));
		CloseHandle(hFile);
		CryptReleaseContext(hProv, 0);
		return std::string("ERROR");
	}

	BOOL bResult = FALSE;
	DWORD cbRead;
	BYTE rgbFile[BUFSIZE];

	while (bResult = ReadFile(hFile, rgbFile, BUFSIZE, &cbRead, NULL))
	{
		if (!cbRead)
		{
			break;
		}

		if (!CryptHashData(hHash, rgbFile, cbRead, 0))
		{
			int dwStatus = GetLastError();
			CMiscUtils::PrintLine(std::string("Error with CryptHashData"));
			CryptReleaseContext(hProv, 0);
			CryptDestroyHash(hHash);
			CloseHandle(hFile);
			return std::string("ERROR");
		}
	}

	if (!bResult)
	{
		int dwStatus = GetLastError();
		CMiscUtils::PrintLine(std::string("ReadFile failed"));
		CryptReleaseContext(hProv, 0);
		CryptDestroyHash(hHash);
		CloseHandle(hFile);
		return std::string("ERROR");
	}

	std::string hashOutput;

	if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0))
	{
		//printf("MD5 hash of file %s is: ", fileName.c_str());

		for (DWORD i = 0; i < cbHash; i++) {
			hashOutput += std::to_string(rgbDigits[rgbHash[i] >> 4]);
			hashOutput += std::to_string(rgbDigits[rgbHash[i] & 0xf]);

			//printf("%c%c", rgbDigits[rgbHash[i] >> 4], rgbDigits[rgbHash[i] & 0xf]);
		}
		//printf("\n");
	}


	CryptReleaseContext(hProv, 0);
	CryptDestroyHash(hHash);
	CloseHandle(hFile);

	return hashOutput;
}
