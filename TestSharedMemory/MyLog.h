#pragma once

#include <fstream>
#include <iostream>
#include <cstdarg>
#include <string>
#include <time.h>


using namespace std;
#define LOGGER CMyLog::GetLogger()
#define MAX_LOG_MESSAGE 1024

extern BOOL gOutput_List;
extern BOOL gOutput_Debug;
extern BOOL gOutput_File;

/**
*   Singleton Logger Class.
*	@Usage
*	CMyLog::GetLogger()->LogD(L"message to be logged");
*	LOGGER->LogD(L"한글로그 (%d)", 100);
*/
class CTRDP_StartDlg;
class CMyLog
{
public:

	void LogD(const WCHAR* format, ...);    	
	void LogI(const WCHAR* format, ...);    
	void LogW(const WCHAR* format, ...);    
	void LogE(const WCHAR* format, ...);
	void LogF(const WCHAR* format, ...);		

	// TRDP용 로그 함수.
	void LogT(std::string logMsg);
	void LogT_HexToAscii(const char* recvBuff, int recvSize);

    /**
    *   Funtion to create the instance of logger class.
    *   @return singleton object of Clogger class..
    */
    static CMyLog* GetLogger();	
private:
    /**
    *    Default constructor for the Logger class.
    */
    CMyLog();
	~CMyLog();
    /**
    *   copy constructor for the Logger class.
    */
    CMyLog(const CMyLog&){};             // copy constructor is private
    /**
    *   assignment operator for the Logger class.
    */
    CMyLog& operator=(const CMyLog&){ return *this; };  // assignment operator is private
	/**
    *   Variable Length Logger function
    *   @param format string for the message to be logged.
    */
    void Log(const WCHAR* logLevel, const WCHAR* logMsg);    
		
    /**
    *   Log file name.
    */
    static const std::wstring m_sFileName;
    /**
    *   Singleton logger class object pointer.
    */
    static CMyLog* m_pThis;
    /**
    *   Log file stream object.
    */
    static wofstream m_Logfile;	
};

