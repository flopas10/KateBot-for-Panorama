#pragma once

#define SAFE_DELETE_VECTOR(vec)	if (!vec.empty()) { \
									for (auto& slot : vec) { \
										delete slot; \
										slot = nullptr; \
									} \
								}

class Mem
{
public:
	Mem();
	~Mem();

	bool					Attach( const std::string& ExeName );
	void					Detach();
	bool					DumpModList();
	
	bool					Read( DWORD_PTR dwAddress, LPVOID lpBuffer, DWORD_PTR dwSize );
	bool					Write( DWORD_PTR dwAddress, LPCVOID lpBuffer, DWORD_PTR dwSize );

	Module*					GetModule( const std::string& ImageName );
	HMODULE					LoadRemote( const std::string& ImageName );

	void                    SetWindow( HWND window );
	HWND                    GetWindow();

	DWORD_PTR				Scan( DWORD_PTR dwStart, DWORD_PTR dwSize, const char* pSignature, const char* pMask );

	template<typename T>
	T inline ReadF(DWORD addr)
	{
		T mem;
		ReadProcessMemory(m_hProcess, (LPVOID)addr, &mem, sizeof(T), NULL);
		return mem;
	}
	
	template<typename T>
	void inline WriteF(DWORD addr, T data)
	{
		WriteProcessMemory(m_hProcess, (LPVOID)addr, &data, sizeof(T), NULL);
	}
	
	template<typename T>
	T Read( DWORD_PTR dwAddress, const T& tDefault = T() )
	{
		T tRet;

		if( !Read( dwAddress, &tRet, sizeof( T ) ) )
		{
			return tDefault;
		}

		return tRet;
	}

	template<typename T>
	bool Write( DWORD_PTR dwAddress, const T& tValue )
	{
		return Write( dwAddress, &tValue, sizeof( T ) );
	}

	template<typename T>
	bool WriteProtected( DWORD_PTR dwAddress, const T& tValue )
	{
		DWORD_PTR oldProtect;
		VirtualProtectEx( m_hProcess, ( LPVOID )dwAddress, sizeof( T ), PAGE_EXECUTE_READWRITE, &oldProtect );
		Write( dwAddress, &tValue, sizeof( T ) );
		VirtualProtectEx( m_hProcess, ( LPVOID )dwAddress, sizeof( T ), oldProtect, NULL );
		return true;
	}
	
	void debuglog(char* str) const
	{
		FILE * pFile;
		pFile = fopen ("debugLog.txt","w");
		fprintf (pFile, "Crash: %s", str);
		fclose (pFile);
		printf("Crash: %s", str);
		system("pause");
	}

private:
	DWORD					GetProcessIdByName( const std::string& name );

private:
	DWORD					m_dwProcessId = NULL;
	std::vector<Module*>	m_pModList;
	HWND					m_hWindow = NULL;
public:
	HANDLE					m_hProcess = NULL;
};

extern Mem*	mem;
extern Module*	client;
extern Module*	engine;
extern Module* csgobase;

