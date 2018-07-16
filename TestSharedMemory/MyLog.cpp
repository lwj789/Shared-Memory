#include "StdAfx.h"
#include "MyLog.h"

#include "resource.h"

#include <sys/timeb.h>

const wstring CMyLog::m_sFileName = L"SM";
CMyLog* CMyLog::m_pThis = NULL;
wofstream CMyLog::m_Logfile;


BOOL gOutput_List = TRUE;
BOOL gOutput_Debug = TRUE;
BOOL gOutput_File = TRUE;

CMyLog::CMyLog(void)
{
	// �⺻ ���� ��� ����(�α������� �����Ǵ� ��ġ�� �������ϰ� ���� �ϱ� ����)
	CString strPath(__wargv[0]);
	SetCurrentDirectory(strPath.Left(strPath.ReverseFind('\\')));

	// wofstream �ѱ� ��µǵ���.
	m_Logfile.imbue(std::locale("kor"));
}

CMyLog::~CMyLog(void)
{
	if (m_pThis != NULL){
		delete m_pThis;
	}	
}


CMyLog* CMyLog::GetLogger(){
	if (m_pThis == NULL){
		m_pThis = new CMyLog();
	}	
	return m_pThis;
}

void CMyLog::LogD(const WCHAR* format, ...)
{
	WCHAR szMsg[MAX_LOG_MESSAGE];
	va_list args;
	va_start(args, format);
	vswprintf(szMsg, format, args);
	va_end(args);

	Log(L"DEBUG", szMsg);
}

void CMyLog::LogI(const WCHAR* format, ...)
{
	WCHAR szMsg[MAX_LOG_MESSAGE];
	va_list args;
	va_start(args, format);
	vswprintf(szMsg, format, args);
	va_end(args);

	Log(L"INFO", szMsg);
}

void CMyLog::LogW(const WCHAR* format, ...)
{
	WCHAR szMsg[MAX_LOG_MESSAGE];
	va_list args;
	va_start(args, format);
	vswprintf(szMsg, format, args);
	va_end(args);

	Log(L"WARN", szMsg);
}

void CMyLog::LogE(const WCHAR* format, ...)
{
	WCHAR szMsg[MAX_LOG_MESSAGE];
	va_list args;
	va_start(args, format);
	vswprintf(szMsg, format, args);
	va_end(args);

	Log(L"ERROR", szMsg);
}

void CMyLog::LogF(const WCHAR* format, ...)
{
	WCHAR szMsg[MAX_LOG_MESSAGE];
	va_list args;
	va_start(args, format);
	vswprintf(szMsg, format, args);
	va_end(args);

	Log(L"FATAL", szMsg);
}

void CMyLog::LogT(std::string logMsg)
{
	// �ð����
	struct timeb timebuffer;
	struct tm Tm;
	ftime(&timebuffer);
	_localtime64_s(&Tm, &timebuffer.time);

	// ���ϰ�� ����
	WCHAR Path[MAX_PATH];
	swprintf( Path, L"%s_%.2d%.2d%.2d.log",
		m_sFileName.c_str(), Tm.tm_year+1900-2000, Tm.tm_mon+1, Tm.tm_mday );

	// string -> wstring
	int strOutSize = logMsg.length()+1;
	WCHAR* strOut = new WCHAR[strOutSize];
	ZeroMemory(strOut, strOutSize);
	wsprintf(strOut, L"%S", logMsg.c_str());

	///////////////////////
	// ���	

	// �����â �ƿ�
	if(gOutput_Debug)
	{
		OutputDebugStringW(strOut);
	}
	

	// ���Ϸ� �߰�
	if(gOutput_File)
	{
		m_Logfile.open(Path, ios::out | ios::app);
		if ( m_Logfile.is_open() == FALSE) {
			return ;
		}

		m_Logfile << strOut;
		m_Logfile.close();
	}

	delete [] strOut;
	strOut = NULL;
}

void CMyLog::LogT_HexToAscii(const char* recvBuff, int recvSize)
{
	char szStrHex[4] = {0, };

	string strHex = "";
	for( int i=0; i<recvSize; i++ )
	{
		sprintf_s(szStrHex, sizeof(szStrHex), "%02X ", recvBuff[i]);
		strHex += szStrHex;
		memset(szStrHex, 0x00, sizeof(szStrHex));
	}	
	strHex += "\r\n";

	LogT(strHex);
}

void CMyLog::Log(const WCHAR* logLevel, const WCHAR* logMsg)
{
	// �ð����
	struct timeb timebuffer;
	struct tm Tm;
	int milisec;

	ftime(&timebuffer);	
	// milisec�����Ѵ�
	milisec = timebuffer.millitm;	
	_localtime64_s(&Tm, &timebuffer.time);
	
	// ���ϰ�� ����
	WCHAR Path[MAX_PATH];
	swprintf( Path, L"%s_%.2d%.2d%.2d.log",
		m_sFileName.c_str(), Tm.tm_year+1900-2000, Tm.tm_mon+1, Tm.tm_mday );

	// �ð� + Log�����, Cariage Return + Line Feed
	//WCHAR strOut[1024];
	WCHAR* strOut = new WCHAR[1024];
	swprintf( strOut, L"[%.2i:%.2i:%.2i.%.3i] [%s] %s\r\n", Tm.tm_hour, Tm.tm_min, Tm.tm_sec, milisec, logLevel, logMsg);

	///////////////////////
	// ���
	
	// �����â �ƿ�
	if(gOutput_Debug)
	{
		OutputDebugStringW(strOut);
	}
#if 0
	// �ܼ����
	wprintf( strOut );
#endif

	// ���Ϸ� �߰�
	if(gOutput_File)
	{
		m_Logfile.open(Path, ios::out | ios::app);
		if ( m_Logfile.is_open() == FALSE) {
			return ;
		}

		m_Logfile << strOut;
		m_Logfile.close();
	}

	delete [] strOut;
	strOut = NULL;
}
