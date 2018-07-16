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
		LOGGER->LogD(L"�̹� ������ ���� �޸𸮰� �����մϴ�. ERR(%d)", GetLastError());
		return TRUE;
	}

	m_nMemSize = nSize;
	m_handle = OpenFileMapping(FILE_MAP_ALL_ACCESS, NULL, m_strName.c_str());
	if(m_handle != NULL) {
		LOGGER->LogD(L"������ ����ϴ� ���� �޸𸮰� ���� �մϴ�. ERR(%d), �����ϴ� �����޸� ������� ������� �ϴ� ������� �ٸ� �� �ֽ��ϴ�.", GetLastError());
		return TRUE;
	}

	//���� �޸𸮸� m_nSize ������� �����մϴ�.
	m_handle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, m_nMemSize, m_strName.c_str());

	//���� �޸𸮸� ���������� ���������� �޼��� �ڽ��� ��� �ش� ������ ����մϴ�.
	if(m_handle != NULL) {
		LOGGER->LogI(L"���� �޸𸮰� ���� �Ǿ����ϴ�.\r\n �̸�: %s, ũ��: %d byte , ERR(%d)", m_strName.c_str(), m_nMemSize, GetLastError());
	}	
	else {
		// �޸𸮰� ������ �ÿ� ���� �޸� ������ ���� �� �� �ְ�,
		// ������ �̸����� ������ ���� �޸��� ������ �ɷ� �ִ� ��쿡�� ������ �� �ֽ��ϴ�.
		LOGGER->LogE(L"���� �޸𸮸� �����ϴµ� �����߽��ϴ�. ERR(%d)", GetLastError());
		return FALSE;
	}

	return TRUE;
}

BOOL CMySharedMemory::Attach(void)
{
	if(m_Ptr != NULL) {
		return TRUE;
	}
	// ���� �޸𸮿� �����ϴ� �ּҰ��� ȹ���մϴ�.
	// ������ ����� �����մϴ�.
	m_Ptr = MapViewOfFile(m_handle, FILE_MAP_ALL_ACCESS, 0, 0, m_nMemSize);

	if(m_Ptr == NULL) {
		LOGGER->LogE(L"���� �޸� �����ּ� ȹ�� ���� ERR(%d)", GetLastError());
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

	//�ڵ��� �����Ѵٸ� �ڵ��� �����մϴ�.
	if(m_handle != NULL) 
	{
		BOOL bResult = CloseHandle(m_handle);
		LOGGER->LogI(L"CloseHandle %s, ERR(%d)", bResult != 0 ? L"SUCCESS" : L"FALSE", GetLastError());
		m_handle = NULL;
	}
}
