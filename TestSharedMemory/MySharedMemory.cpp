#include "StdAfx.h"
#include "MySharedMemory.h"

#include "MyLog.h"
#include "CommonFunction.h"
#include <Windows.h>


CMySharedMemory::CMySharedMemory(const wstring& sName)
	: m_strName(sName), m_Ptr(NULL), m_handle(NULL), m_nMemSize(0)
{
}

CMySharedMemory::~CMySharedMemory(void)
{
	Clear();
}

////////////////////////////////////////////////////////////////////////////////////////////
// 
BOOL CMySharedMemory::Create(size_t nSize)
{
	if(m_handle != NULL) {
		LOGGER->LogD(L"이미 생성된 공유 메모리가 존재합니다. ERR(%d)", GetLastError());
		return TRUE;
	}

	m_nMemSize = nSize;
	m_handle = OpenFileMapping(FILE_MAP_ALL_ACCESS, NULL, m_strName.c_str());
	if(m_handle != NULL) {
		LOGGER->LogD(L"기존에 사용하는 공유 메모리가 존재 합니다. ERR(%d), 존재하는 공유메모리 사이즈는 만들고자 하는 사이즈와 다를 수 있습니다.", GetLastError());
		return TRUE;
	}

	//공유 메모리를 m_nSize 사이즈로 생성합니다.
	m_handle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, m_nMemSize, m_strName.c_str());

	//공유 메모리를 성공적으로 생성했으면 메세지 박스를 띄워 해당 내용을 출력합니다.
	if(m_handle != NULL) {
		LOGGER->LogI(L"공유 메모리가 생성 되었습니다.\r\n 이름: %s, 크기: %d byte , ERR(%d)", m_strName.c_str(), m_nMemSize, GetLastError());
	}	
	else {
		// 메모리가 부족할 시에 공유 메모리 생성에 실패 할 수 있고,
		// 동일한 이름으로 생성된 공유 메모리의 보안이 걸려 있는 경우에도 실패할 수 있습니다.
		LOGGER->LogE(L"공유 메모리를 생성하는데 실패했습니다. ERR(%d)", GetLastError());
		return FALSE;
	}

	return TRUE;
}

BOOL CMySharedMemory::Attach(void)
{
	if(m_Ptr != NULL) {
		return TRUE;
	}
	// 공유 메모리에 접근하는 주소값을 획득합니다.
	// 데이터 사이즈를 지정합니다.
	m_Ptr = MapViewOfFile(m_handle, FILE_MAP_ALL_ACCESS, 0, 0, m_nMemSize);

	if(m_Ptr == NULL) {
		LOGGER->LogE(L"공유 메모리 접근주소 획득 실패 ERR(%d)", GetLastError());
		return FALSE;
	}
	return TRUE;
}

BOOL CMySharedMemory::Detach()
{
	if(UnmapViewOfFile(m_Ptr) == 0) {
		LOGGER->LogE(L"UnmapViewOfFile %s, ERR(%d)", L"FALSE", GetLastError());
		return FALSE;
	} 
	else {		
		LOGGER->LogI(L"UnmapViewOfFile %s, ERR(%d)", L"SUCCESS", GetLastError());
		m_Ptr = NULL;
	}

	return TRUE;
}

BOOL CMySharedMemory::Lock()
{
	return TRUE;
}

BOOL CMySharedMemory::UnLock()
{
	return TRUE;
}

HANDLE CMySharedMemory::GetID(void) 
{ 
	return m_handle; 
}

void* CMySharedMemory::GetData(void) 
{ 
	return m_Ptr; 
};

const void* CMySharedMemory::GetData(void) const 
{ 
	return m_Ptr; 
}

void CMySharedMemory::MemCopy(char * p_str)
{
	if (m_Ptr != NULL)
	{
		strcpy((char *)m_Ptr,p_str);
	}
}

void CMySharedMemory::Clear()
{
	Detach();

	//핸들이 존재한다면 핸들을 제거합니다.
	if(m_handle != NULL) 
	{
		BOOL bResult = CloseHandle(m_handle);
		LOGGER->LogI(L"CloseHandle %s, ERR(%d)", bResult != 0 ? L"SUCCESS" : L"FALSE", GetLastError());
		m_handle = NULL;
	}
}
