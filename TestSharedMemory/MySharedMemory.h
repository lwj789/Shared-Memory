#pragma once

#include <string>
using namespace std;

#define SM_NAME_DU_SD			L"HM_SHARED_MEMORY_DU_SD";
#define SM_NAME_DU_SDR			L"HM_SHARED_MEMORY_DU_SDR";
#define SM_NAME_SIMULATOR_SD	L"HM_SHARED_MEMORY_SIMULATOR_SD";
#define SM_NAME_SIMULATOR_SDR	L"HM_SHARED_MEMORY_SIMULATOR_SDR";

#define SM_NAME_MEMORY_TEST     L"HM_SHARED_MEMORY_MEMORY_TEST";


#define PD_DATASET_MAX_LENGTH	1432

typedef unsigned char BYTE;

typedef struct TSharedMemoryData {
	BYTE data[100][PD_DATASET_MAX_LENGTH];
} TSharedMemory;

class CMySharedMemory
{
public:
	CMySharedMemory(const wstring& sName);
	~CMySharedMemory(void);
		
	BOOL Create(size_t nMemSize);
	BOOL Attach(void);
	BOOL Detach(void);
	BOOL Lock(void);
	BOOL UnLock(void);
	HANDLE GetID(void);
	void* GetData(void);
	const void* GetData(void) const;

	void MemCopy(char *p_str);

private:
	void Clear();

private:
	wstring m_strName;
	HANDLE m_handle;
	size_t m_nMemSize;
	void* m_Ptr;

};

