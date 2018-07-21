#pragma once

class Files
{
public:
	void	OnSetup( const char* fileName, const char* fileDir );

	int		ReadInt( char* element, char* item );
	int		ReadIntParam( char* element, char* item, char* param );
	float	ReadFloat( char* element, char* item );
	float	ReadFloatParam( char* element, char* item, char* param );
	bool	ReadBool( char* element, char* item );
	bool	ReadBoolParam( char* element, char* item, char* param );
	char*	ReadChar( char* element, char* item );
	char*	ReadCharParam( char* element, char* item, char* param );

	void	WriteInt( char* element, char* item, int value );
	void	WriteIntParam( char* element, char* item, char* param, int value );
	void	WriteFloat( char* element, char* item, float value );
	void	WriteFloatParam( char* element, char* item, char* param, float value );
	void	WriteBool( char* element, char* item, bool value );
	void	WriteBoolParam( char* element, char* item, char* param, bool value );
	void	WriteChar( char* element, char* item, char* value );
	void	WriteCharParam( char* element, char* item, char* param, char* value );

private:
	char m_fileName[ 255 ];
};

extern Files* g_pFiles;