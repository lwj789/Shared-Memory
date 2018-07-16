#pragma once

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <string>
using namespace std;

extern wstring _FormatW(wchar_t* pstrFormat, ... );
extern string _FormatA(char* pstrFormat, ... );

extern string _TrimA(std::string& s,const std::string& drop);
extern string _RTrimA(std::string s,const std::string& drop);
extern string _LTrimA(std::string s,const std::string& drop);

inline wstring _FormatW(wchar_t* pstrFormat, ... )
{
	va_list args;
	int     len;
	wchar_t    *buffer;

	va_start( args, pstrFormat );

	len = _vscwprintf ( pstrFormat, args ) + 1; // terminating '\0'
	buffer = (wchar_t*)malloc( len * sizeof(wchar_t) );

	vswprintf( buffer, pstrFormat, args );
	wstring strArgv(buffer);
	free( buffer );

	return strArgv;
}

inline string _FormatA(char* pstrFormat, ... )
{
	va_list args;
	int     len;
	char    *buffer;

	va_start( args, pstrFormat );

	len = _vscprintf ( pstrFormat, args ) + 1; // terminating '\0'
	buffer = (char*)malloc( len * sizeof(char) );

	vsprintf( buffer, pstrFormat, args );
	string strArgv(buffer);
	free( buffer );
	va_end(args);

	return strArgv;
}

#define TRIM_SPACE " \t\n\v"
inline std::string _TrimA(std::string& s,const std::string& drop = TRIM_SPACE)
{
	std::string r=s.erase(s.find_last_not_of(drop)+1);
	return r.erase(0,r.find_first_not_of(drop));
}

inline std::string _RTrimA(std::string s,const std::string& drop = TRIM_SPACE)
{
	return s.erase(s.find_last_not_of(drop)+1);
}

inline std::string _LTrimA(std::string s,const std::string& drop = TRIM_SPACE)
{
	return s.erase(0,s.find_first_not_of(drop));
}
