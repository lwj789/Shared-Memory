
// TestSharedMemoryDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "TestSharedMemory.h"
#include "TestSharedMemoryDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestSharedMemoryDlg 대화 상자



CTestSharedMemoryDlg::CTestSharedMemoryDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TESTSHAREDMEMORY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pSharedMem = NULL;
	m_pSharedData = NULL;
}

void CTestSharedMemoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	//DDX_Control(pDX, IDC_STR_OUTPUT, m_editHex_sd);
}

BEGIN_MESSAGE_MAP(CTestSharedMemoryDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_STR_ENTER, &CTestSharedMemoryDlg::OnBnClickedStrEnter)
	ON_BN_CLICKED(IDC_CREATE_SM_BTN, &CTestSharedMemoryDlg::OnBnClickedCreateSmBtn)
	ON_BN_CLICKED(IDC_RELEASE_SM_BTN, &CTestSharedMemoryDlg::OnBnClickedReleaseSmBtn)
END_MESSAGE_MAP()


// CTestSharedMemoryDlg 메시지 처리기

BOOL CTestSharedMemoryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTestSharedMemoryDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTestSharedMemoryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestSharedMemoryDlg::OnBnClickedStrEnter()
{
	GetDlgItemText(IDC_EDIT_STR, str); // 입력할 내용
	SetDlgItemText(IDC_STR_OUTPUT, str); // 출력 할 내용
}


void CTestSharedMemoryDlg::OnBnClickedCreateSmBtn()
{
	CString strName = SM_NAME_MEMORY_TEST;
	
	char p_str[127];
	size_t CharactersConverted = 0;

	wcstombs_s(&CharactersConverted, p_str, str.GetLength() + 1, str, _TRUNCATE); //유니코드 스트링을 멀티바이트 스트링으로 변환

	m_nMemSize = sizeof(TSharedMemory);
	if (m_pSharedMem == NULL) {
		try {
			m_pSharedMem = new CMySharedMemory(strName.GetBuffer(0));
			m_pSharedMem->Create(m_nMemSize);
			m_pSharedMem->Attach();
			m_pSharedMem->MemCopy(p_str); 
		}
		catch (...) {
			LOGGER->LogE(L"CMySharedMemory Create Fail");
		}
	}
	
	
	//m_pSharedData = (TSharedMemory*)m_pSharedMem->GetData();
	//m_editHex_sd.SetData((LPBYTE)&m_pSharedData->data[0][0], m_nMemSize);

	GetDlgItem(IDC_CREATE_SM_BTN)->EnableWindow(FALSE);
}


void CTestSharedMemoryDlg::OnBnClickedReleaseSmBtn()
{
	if (m_pSharedMem != NULL) {
		try {
			delete m_pSharedMem;
			m_pSharedMem = NULL;

			GetDlgItem(IDC_CREATE_SM_BTN)->EnableWindow(TRUE);
		}
		catch (...) {
			LOGGER->LogE(L"CMySharedMemory Close Fail");
		}
	}
	
	
}
