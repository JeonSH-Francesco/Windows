
// LibLoaderDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "LibLoader.h"
#include "LibLoaderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLibLoaderDlg 대화 상자



CLibLoaderDlg::CLibLoaderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LIBLOADER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLibLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLibLoaderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Btn_Load, &CLibLoaderDlg::OnBnClickedBtnLoad)
	ON_BN_CLICKED(IDC_Btn_Api_Call, &CLibLoaderDlg::OnBnClickedBtnApiCall)
	ON_BN_CLICKED(IDC_Btn_Free, &CLibLoaderDlg::OnBnClickedBtnFree)
	ON_BN_CLICKED(IDC_Btn_Thread, &CLibLoaderDlg::OnBnClickedBtnThread)
	ON_BN_CLICKED(IDC_Btn_StaticTest, &CLibLoaderDlg::OnBnClickedBtnStatictest)
	ON_BN_CLICKED(IDC_Btn_StaticRace, &CLibLoaderDlg::OnBnClickedBtnStaticrace)
	ON_BN_CLICKED(IDC_Btn_StopRace, &CLibLoaderDlg::OnBnClickedBtnStoprace)
	ON_BN_CLICKED(IDC_Btn_Dll_variable, &CLibLoaderDlg::OnBnClickedBtnDllvariable)
	ON_BN_CLICKED(IDC_Btn_Get_Tls_Data, &CLibLoaderDlg::OnBnClickedBtnGetTlsData)
	ON_BN_CLICKED(IDC_Btn_Get_Tls_Data_Thread1, &CLibLoaderDlg::OnBnClickedBtnGetTlsDataThread1)
	ON_BN_CLICKED(IDC_Btn_Get_Tls_Data_Thread2, &CLibLoaderDlg::OnBnClickedBtnGetTlsDataThread2)
END_MESSAGE_MAP()


// CLibLoaderDlg 메시지 처리기

BOOL CLibLoaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CLibLoaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CLibLoaderDlg::OnPaint()
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
HCURSOR CLibLoaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLibLoaderDlg::OnBnClickedBtnLoad()
{
	m_hDll = ::LoadLibrary(_T("DllSample.dll"));
	if (m_hDll == NULL)
	{
		AfxMessageBox(_T("Failed to load DLLSample.dll"));
		return;
	}
}


void CLibLoaderDlg::OnBnClickedBtnApiCall()
{
	if (m_hDll == NULL)
	{
		OutputDebugString(_T("m_hDll == NULL"));
		return;
	}

	DWORD(WINAPI * pfMyDllFunc)(void) = NULL;
	pfMyDllFunc = (DWORD (WINAPI*)(void))::GetProcAddress(
		m_hDll, "MyDllFunction");
	pfMyDllFunc();
}


void CLibLoaderDlg::OnBnClickedBtnFree()
{
	if (m_hDll != NULL)
	{
		::FreeLibrary(m_hDll);
		m_hDll = NULL;
	}
}


UINT WINAPI ThreadFunction(LPVOID)
{
	OutputDebugString(_T("ThreadFunction - Begin"));
	OutputDebugString(_T("ThreadFunction - End"));
	return 0;
}

void CLibLoaderDlg::OnBnClickedBtnThread()
{
	AfxBeginThread(ThreadFunction, NULL);
}


void CLibLoaderDlg::OnBnClickedBtnStatictest()
{
	int (WINAPI * pfGetTest)(void) = NULL;
	pfGetTest = (int (WINAPI*)(void))::GetProcAddress(
		m_hDll, "GetStaticTest");

	CString strTmp;
	strTmp.Format(_T("%d"), pfGetTest());
	AfxMessageBox(strTmp);
}


UINT ThreadSet10(LPVOID pParam)
{
	BOOL *pbFlag = (BOOL*)pParam;
	HMODULE hDll = ::LoadLibrary(_T("DllSample.dll"));

	void (WINAPI * pfSetTest)(int) = NULL;
	pfSetTest = (void (WINAPI*)(int))::GetProcAddress(
		hDll, "SetStaticTest");

	while (!*pbFlag)
	{
		pfSetTest(10);
		Sleep(1);
	}

	::FreeLibrary(hDll);
	AfxMessageBox(_T("ThreadSet10() - End"));
	return 0;
}

UINT ThreadSet20(LPVOID pParam)
{
	BOOL* pbFlag = (BOOL*)pParam;
	HMODULE hDll = ::LoadLibrary(_T("DllSample.dll"));

	void (WINAPI * pfSetTest)(int) = NULL;
	pfSetTest = (void (WINAPI*)(int))::GetProcAddress(
		hDll, "SetStaticTest");

	while (!*pbFlag)
	{
		pfSetTest(20);
		Sleep(1);
	}

	::FreeLibrary(hDll);
	AfxMessageBox(_T("ThreadSet20() - End"));
	return 0;
}


void CLibLoaderDlg::OnBnClickedBtnStaticrace()
{
	m_bRaceExit = FALSE;
	AfxBeginThread(ThreadSet10, &m_bRaceExit);
	AfxBeginThread(ThreadSet20, &m_bRaceExit);
}


void CLibLoaderDlg::OnBnClickedBtnStoprace()
{
	m_bRaceExit = TRUE;
}


void CLibLoaderDlg::OnBnClickedBtnDllvariable()
{
	int* pDllVariable = NULL;
	pDllVariable = (int*)::GetProcAddress(
		m_hDll, "g_nTest");

	*pDllVariable = 100;
}


void CLibLoaderDlg::OnBnClickedBtnGetTlsData()
{
	int (WINAPI * pfGetTlsData)(void) = NULL;
	pfGetTlsData = (int (WINAPI*)(void))::GetProcAddress(
		m_hDll, "GetTlsData");

	void (WINAPI * pfSetTlsData)(int) = NULL;
	pfSetTlsData = (void (WINAPI*)(int))::GetProcAddress(
		m_hDll, "SetTlsData");

	pfSetTlsData(200);
	::Sleep(2000);

	CString strTmp;
	strTmp.Format(_T("TLS: %d"), pfGetTlsData());
	AfxMessageBox(strTmp);
}


UINT ThreadSetTls10(LPVOID pParam)
{
	BOOL* pbFlag = (BOOL*)pParam;
	HMODULE hDll = ::LoadLibrary(_T("DllSample.dll"));

	void (WINAPI * pfSetTlsData)(int) = NULL;
	pfSetTlsData = (void (WINAPI*)(int))::GetProcAddress(
		hDll, "SetTlsData");

	while (!*pbFlag)
	{
		pfSetTlsData(10);
		Sleep(1);
	}

	int (WINAPI * pfGetTlsData)(void) = NULL;
	pfGetTlsData = (int (WINAPI*)(void))::GetProcAddress(
		hDll, "GetTlsData");

	CString strTmp;
	strTmp.Format(_T("ThreadSetTls10(): %d"), pfGetTlsData());
	AfxMessageBox(strTmp);

	::FreeLibrary(hDll);
	return 0;
}

UINT ThreadSetTls20(LPVOID pParam)
{
	BOOL* pbFlag = (BOOL*)pParam;
	HMODULE hDll = ::LoadLibrary(_T("DllSample.dll"));

	void (WINAPI * pfSetTlsData)(int) = NULL;
	pfSetTlsData = (void (WINAPI*)(int))::GetProcAddress(
		hDll, "SetTlsData");

	while (!*pbFlag)
	{
		pfSetTlsData(20);
		Sleep(1);
	}

	int (WINAPI * pfGetTlsData)(void) = NULL;
	pfGetTlsData = (int (WINAPI*)(void))::GetProcAddress(
		hDll, "GetTlsData");

	CString strTmp;
	strTmp.Format(_T("ThreadSetTls20(): %d"), pfGetTlsData());
	AfxMessageBox(strTmp);

	::FreeLibrary(hDll);
	return 0;
}

void CLibLoaderDlg::OnBnClickedBtnGetTlsDataThread1()
{
	m_bRaceExit = FALSE;
	AfxBeginThread(ThreadSetTls10, &m_bRaceExit);
	AfxBeginThread(ThreadSetTls20, &m_bRaceExit);
}


void CLibLoaderDlg::OnBnClickedBtnGetTlsDataThread2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
