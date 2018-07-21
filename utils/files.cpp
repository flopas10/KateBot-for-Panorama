#include "..\main.h"

Files* g_pFiles = new Files();
[junk_enable /]	
[enc_string_enable /]
void Files::OnSetup( const char* fileName, const char* fileDir )
{
	memset( m_fileName, 0x00, 255 );
	strcpy( m_fileName, fileDir );
	strcat( m_fileName, fileName );
	//printf(m_fileName);
}

int Files::ReadInt( char* element, char* item )
{
	return GetPrivateProfileIntA( element, item, NULL, m_fileName );
}

int Files::ReadIntParam( char* element, char* item, char* param )
{
	char m_ParamName[ 255 ];
	memset( m_ParamName, 0x00, 255 );
	strcpy( m_ParamName, item );
	strcat( m_ParamName, param );
	return GetPrivateProfileIntA( element, m_ParamName, NULL, m_fileName );
}

float Files::ReadFloat( char* element, char* item )
{
	char result[ 255 ];
	GetPrivateProfileStringA( element, item, NULL, result, 255, m_fileName );
	return atof( result );
}

float Files::ReadFloatParam( char* element, char* item, char* param )
{
	char m_ParamName[ 255 ];
	memset( m_ParamName, 0x00, 255 );
	strcpy( m_ParamName, item );
	strcat( m_ParamName, param );
	
	char result[ 255 ];
	GetPrivateProfileStringA( element, m_ParamName, NULL, result, 255, m_fileName );
	return atof( result );
}

bool Files::ReadBool( char* element, char* item )
{
	char result[ 255 ];
	GetPrivateProfileStringA( element, item, NULL, result, 255, m_fileName );
	return ( strcmp( result, "True" ) == 0 || strcmp( result, "true" ) == 0 ) ? true : false;
}

bool Files::ReadBoolParam( char* element, char* item, char* param )
{
	char m_ParamName[ 255 ];
	memset( m_ParamName, 0x00, 255 );
	strcpy( m_ParamName, item );
	strcat( m_ParamName, param );
	
	char result[ 255 ];
	GetPrivateProfileStringA( element, m_ParamName, NULL, result, 255, m_fileName );
	return ( strcmp( result, "True" ) == 0 || strcmp( result, "true" ) == 0 ) ? true : false;
}

char* Files::ReadChar( char* element, char* item )
{
	char result[ 255 ];
	GetPrivateProfileStringA( element, item, NULL, result, 255, m_fileName );
	return result;
}

char* Files::ReadCharParam( char* element, char* item, char* param )
{
	char m_ParamName[ 255 ];
	memset( m_ParamName, 0x00, 255 );
	strcpy( m_ParamName, item );
	strcat( m_ParamName, param );
	
	char result[ 255 ];
	GetPrivateProfileStringA( element, m_ParamName, NULL, result, 255, m_fileName );
	return result;
}

void Files::WriteInt( char* element, char* item, int value )
{
	char temp[ 255 ];
	sprintf_s( temp, "%d", value );
	WritePrivateProfileStringA( element, item, temp, m_fileName );
}

void Files::WriteIntParam( char* element, char* item, char* param, int value )
{
	char m_ParamName[ 255 ];
	memset( m_ParamName, 0x00, 255 );
	strcpy( m_ParamName, item );
	strcat( m_ParamName, param );
	
	char temp[ 255 ];
	sprintf_s( temp, "%d", value );
	WritePrivateProfileStringA( element, m_ParamName, temp, m_fileName );
}

void Files::WriteFloat( char* element, char* item, float value )
{
	char temp[ 255 ];
	sprintf_s( temp, "%f", value );
	WritePrivateProfileStringA( element, item, temp, m_fileName );
}

void Files::WriteFloatParam( char* element, char* item, char* param, float value )
{
	char m_ParamName[ 255 ];
	memset( m_ParamName, 0x00, 255 );
	strcpy( m_ParamName, item );
	strcat( m_ParamName, param );
	
	char temp[ 255 ];
	sprintf_s( temp, "%f", value );
	WritePrivateProfileStringA( element, m_ParamName, temp, m_fileName );
}

void Files::WriteBool( char* element, char* item, bool value )
{
	char temp[ 255 ];
	sprintf_s( temp, "%s", value ? "true" : "false" );
	WritePrivateProfileStringA( element, item, temp, m_fileName );
}

void Files::WriteBoolParam( char* element, char* item, char* param, bool value )
{
	char m_ParamName[ 255 ];
	memset( m_ParamName, 0x00, 255 );
	strcpy( m_ParamName, item );
	strcat( m_ParamName, param );
	
	char temp[ 255 ];
	sprintf_s( temp, "%s", value ? "true" : "false" );
	WritePrivateProfileStringA( element, m_ParamName, temp, m_fileName );
}

void Files::WriteChar( char* element, char* item, char* value )
{
	WritePrivateProfileStringA( element, item, value, m_fileName );
}

void Files::WriteCharParam( char* element, char* item, char* param, char* value )
{
	char m_ParamName[ 255 ];
	memset( m_ParamName, 0x00, 255 );
	strcpy( m_ParamName, item );
	strcat( m_ParamName, param );
	
	WritePrivateProfileStringA( element, m_ParamName, value, m_fileName );
}