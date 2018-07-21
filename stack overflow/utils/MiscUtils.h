#pragma once

class CMiscUtils
{
public:

	CMiscUtils() = default;
	~CMiscUtils() = default;

	std::string GenerateRandomString(int length = 15);
	void GenerateRandomWindowTitle(int length = 15);
	void DeleteSelf(char* szProgramPath);
	void AllowDebugging();
	void PrintLine(std::string szStr);

	int GetKeyFromString(std::string str);
	std::string GetStringFromKey(int key);

	void ParseColor(std::string str, Vector& clr);
	std::string GetStringFromColor(Vector color);

	bool IsActiveWindow();
	bool DoesCSGOExist();
	bool IsCSGOActiveWindow();
	bool CheckCSGOWindowState();
	HWND GetGameWindow();

	void DisableQuickEdit();

	std::string GetHash(std::string fileName);

private:
	bool csgoActiveWindow = false;

};

extern CMiscUtils* miscUtils;

