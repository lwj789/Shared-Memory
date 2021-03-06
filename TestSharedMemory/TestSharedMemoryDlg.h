
// TestSharedMemoryDlg.h: 헤더 파일
//

#pragma once

#include "MySharedMemory.h"
#include "MyLog.h"

// CTestSharedMemoryDlg 대화 상자
class CTestSharedMemoryDlg : public CDialogEx
{
// 생성입니다.
public:
	CTestSharedMemoryDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTSHAREDMEMORY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


	// Shared Memory	
public:
	CMySharedMemory * m_pSharedMem;
	TSharedMemory* m_pSharedData;
	int m_nMemSize;
	CString str; // IDC_EDIT_STR에서 문자열을 입력받음.

public:
	afx_msg void OnBnClickedStrEnter();
	afx_msg void OnBnClickedCreateSmBtn();
	afx_msg void OnBnClickedReleaseSmBtn();
};
