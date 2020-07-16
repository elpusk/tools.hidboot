
// hidbootDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "hidboot.h"
#include "hidbootDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <ct_file.h>

// CAboutDlg dialog used for App About

const unsigned char ChidbootDlg::s_default_dev_name[16] = { 'g','a','n','y','m','e','d','e',' ',' ',' ',' ',' ',' ',' ',' ' };

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// ChidbootDlg dialog



ChidbootDlg::ChidbootDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HIDBOOT_DIALOG, pParent),
	m_Dll(_T(".tg_lpu237_fw.dll")),
	m_hDev(NULL),
	m_nIndex(0),
	m_hThread(INVALID_HANDLE_VALUE),
	m_nThreadID(0),
	m_hEvtKillThread(CreateEvent(NULL, TRUE, FALSE, NULL)),
	m_hRunThread(CreateEvent(NULL, TRUE, FALSE, NULL)),
	m_dw_fw_index(-1)
{
	m_vId.resize(LPU237_FW_SIZE_ID, 0);
	m_vName.resize(0);
	m_vVersion.resize(LPU237_FW_SIZE_VERSION, 0);

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void ChidbootDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INFO, m_listboxInfo);
	DDX_Control(pDX, IDC_COMBO_DEV_LIST, m_ComboDevList);
}

BEGIN_MESSAGE_MAP(ChidbootDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &ChidbootDlg::OnBnClickedButtonUpdate)
	ON_MESSAGE(WM_MSR, OnMSR)
END_MESSAGE_MAP()


// ChidbootDlg message handlers

BOOL ChidbootDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	SetTimer(TIMER_ID, TIMER_INTERVAL, NULL);
	pushInfoQ(_T("INI OK."));

	bool b_result(false);
	do {

		if (_setup_device_list() <= 0) {
			AfxMessageBox(L"Not found a device.");
			continue;
		}

		if (!_setup_target_device()) {
			AfxMessageBox(L"target device has a error.");
			continue;
		}

		if (!_setup_fw_file()) {
			AfxMessageBox(L"Not found a available rom file.");
			continue;
		}

		if (!_start_update()) {
			AfxMessageBox(L"Update-start has a error.");
			continue;
		}

		b_result = true;
	} while (false);

	if (!b_result) {
		exit(0);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void ChidbootDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void ChidbootDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR ChidbootDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT ChidbootDlg::OnMSR(WPARAM wParam, LPARAM lParam)
{

	DWORD dw_result(lParam);
	DWORD dw_wparam(wParam);

	if (dw_result == LPU237_FW_RESULT_SUCCESS) {
		switch (dw_wparam) {
		case LPU237_FW_WPARAM_FOUND_BL:
			pushInfoQ(_T(" MSG : found bootloader."));
			break;
		case LPU237_FW_WPARAM_SECTOR_ERASE:
			pushInfoQ(_T(" MSG : erase sector."));
			break;
		case LPU237_FW_WPARAM_SECTOR_WRITE:
			pushInfoQ(_T(" MSG : write firmware."));
			break;
		case LPU237_FW_WPARAM_COMPLETE:
			pushInfoQ(_T(" MSG : complete update."));
			SetTimer(TIMER_COMPLETE_SUCCESS_ID, 500, NULL);
			break;
		default:
			break;
		}//end switch
	}
	else if (dw_result == LPU237_FW_RESULT_CANCEL) {
		pushInfoQ(_T(" MSG : canceled."));
		SetTimer(TIMER_COMPLETE_FAIL_ID, 500, NULL);
	}
	else {
		pushInfoQ(_T(" MSG : error."));
		SetTimer(TIMER_COMPLETE_FAIL_ID, 500, NULL);
	}

	//
	return 0;
}



void ChidbootDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (TIMER_ID == nIDEvent) {
		// for displaying information.
		_type_info_item item = popInfoQ();

		std::wstring sinfo = std::get<0>(item);
		COLORREF color = std::get<1>(item);
		if (!sinfo.empty())
			AddInfo(sinfo.c_str());
	}
	else if (nIDEvent == TIMER_COMPLETE_SUCCESS_ID) {
		KillTimer(nIDEvent);
		m_Dll.LPU237_fw_close(m_hDev);
		AfxMessageBox(L"Update complete OK!");
		exit(0);
	}
	else if (nIDEvent == TIMER_COMPLETE_FAIL_ID) {
		KillTimer(nIDEvent);
		m_Dll.LPU237_fw_close(m_hDev);
		AfxMessageBox(L"-_- ;; Update Failure. Please request A/S.");
		exit(0);
	}


	CDialogEx::OnTimer(nIDEvent);
}


void ChidbootDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	m_Dll.LPU237_fw_close(m_hDev);
}


void ChidbootDlg::AddInfo(LPCTSTR lpInfo)
{
	m_listboxInfo.AddString(lpInfo);
	m_listboxInfo.SetCurSel(m_listboxInfo.GetCount() - 1);
}

int ChidbootDlg::_setup_device_list()
{
	int n_dev(0);

	m_ComboDevList.Clear();

	std::list<std::wstring> listPath;

	if (ChidbootDlg::fun_get_device_list(listPath)) {
		m_ComboDevList.ResetContent();

		std::for_each(std::begin(listPath), std::end(listPath), [&](std::wstring spath) {
			m_ComboDevList.AddString(spath.c_str());
			});

		m_ComboDevList.SetCurSel(0);
		n_dev = (int)listPath.size();
	}
	
	return n_dev;
}

bool ChidbootDlg::_setup_target_device()
{
	bool b_result(false);

	do {
		int nCur = m_ComboDevList.GetCurSel();

		if (nCur < -1) {
			continue;
		}

		CString sPath;

		m_ComboDevList.GetLBText(nCur, sPath);
		m_hDev = m_Dll.LPU237_fw_open(sPath);

		if (m_hDev == INVALID_HANDLE_VALUE) {
			pushInfoQ(_T(" open : error "));
			continue;
		}
		pushInfoQ(_T(" open : success "));

		CString sTemp, sMsg, sTemp1, sMsg1;
		//get id
		DWORD dw_result = m_Dll.LPU237_fw_msr_get_id(m_hDev, &m_vId[0]);
		if (LPU237_FW_RESULT_ERROR == dw_result) {
			pushInfoQ(_T(" get ID : error "));
			continue;
		}
		
		std::for_each(std::begin(m_vId), std::end(m_vId), [&](unsigned char c) {
			sTemp.Format(_T("%02X"), c);
			sMsg = sMsg + sTemp;
			});

		sMsg = _T(" get ID : success ") + sMsg;
		pushInfoQ(sMsg);

		//get name
		sTemp=L"", sMsg = L"", sTemp1 = L"", sMsg1 = L"";

		dw_result = m_Dll.LPU237_fw_msr_get_name(m_hDev, 0);
		if (dw_result == LPU237_FW_RESULT_ERROR) {
			pushInfoQ(_T(" get name : error "));
			continue;
		}
		//
		m_vName.resize(dw_result, 0);
		dw_result = m_Dll.LPU237_fw_msr_get_name(m_hDev, &m_vName[0]);
		if (LPU237_FW_RESULT_ERROR == dw_result) {
			pushInfoQ(_T(" get name : error "));
			continue;
		}
		
		std::for_each(std::begin(m_vName), std::end(m_vName), [&](unsigned char c) {
			sTemp.Format(_T("%02X"), c);
			sMsg = sMsg + sTemp;
			//
			sTemp1.Format(_T("%c"), c);
			sMsg1 = sMsg1 + sTemp1;
			});

		sMsg = _T(" get name : success ") + sMsg;
		sMsg1 = _T(" get name : success ") + sMsg1;
		pushInfoQ(sMsg);
		pushInfoQ(sMsg1);
		//
		// get version
		sTemp = L"", sMsg = L"", sTemp1 = L"", sMsg1 = L"";

		dw_result = m_Dll.LPU237_fw_msr_get_version(m_hDev, &m_vVersion[0]);
		if (LPU237_FW_RESULT_ERROR == dw_result) {
			pushInfoQ(_T(" get_version : error "));
			continue;
		}
		std::for_each(std::begin(m_vVersion), std::end(m_vVersion), [&](unsigned char c) {
			sTemp.Format(_T("%02X"), c);
			sMsg = sMsg + sTemp;
			});

		sMsg = _T(" get version : success ") + sMsg;
		pushInfoQ(sMsg);

		sMsg.Format(_T(" get version : success : V%d.%d"),
			m_Dll.LPU237_fw_msr_get_version_major(&m_vVersion[0]),
			m_Dll.LPU237_fw_msr_get_version_minor(&m_vVersion[0])
		);
		pushInfoQ(sMsg);


		//
		b_result = true;
	} while (false);
	return b_result;
}

bool ChidbootDlg::_setup_fw_file()
{
	bool b_result(false);
	do {
		_ns_tools::type_list_wstring list_found;
		_ns_tools::ct_file::type_find_folder_area area(_ns_tools::ct_file::folder_area_current_folder);

		//find only files.
		std::wstring s_module_abs_path = _ns_tools::ct_file::get_module_abs_path();
		if (s_module_abs_path.empty())
			continue;
		s_module_abs_path.erase(s_module_abs_path.size()-1);//remove backslash

		_ns_tools::ct_file::get_find_file_list(
			list_found
			, s_module_abs_path
			, L"*.rom"
			, area
			, false
		);
		if (list_found.empty())
			continue;
		
		DWORD dw_result = m_Dll.LPU237_fw_rom_load(std::begin(list_found)->c_str());
		if (LPU237_FW_RESULT_SUCCESS != dw_result) {
			pushInfoQ(_T(" load rom : error "));
			continue;
		}
		pushInfoQ(_T(" load rom : success "));
		//
		m_sRom = *std::begin(list_found);
		m_dw_fw_index = 0;
		//
		b_result = true;
	} while (false);

	return b_result;
}

bool ChidbootDlg::_start_update()
{
	bool b_result(false);

	do {
		DWORD dw_result;
		if (is_all_zero_name())
			dw_result = m_Dll.LPU237_fw_msr_update_wnd(NULL, this->m_hWnd, WM_MSR, m_sRom.c_str(), m_dw_fw_index);
		else
			dw_result = m_Dll.LPU237_fw_msr_update_wnd(&m_vId[0], this->m_hWnd, WM_MSR, m_sRom.c_str(), m_dw_fw_index);
		if (dw_result == LPU237_FW_RESULT_ERROR) {
			pushInfoQ(_T("update starting : error "));
			continue;
		}
		else if (dw_result == LPU237_FW_RESULT_NO_MSR) {
			pushInfoQ(_T("update starting : not found your MSR with ID. restart a connected bootloader. "));
			fill(begin(m_vId), end(m_vId), 0);
			continue;
		}
		pushInfoQ(_T("update starting : success "));

		b_result = true;
	} while (false);
	return b_result;
}

int ChidbootDlg::GetStringFromMultiString(std::list<std::wstring>& listStr, LPCTSTR szMultiStr)
{
	LPCTSTR pDest;
	std::wstring stemp;
	INT nCount = 0;
	INT nOffset = 0;

	if (szMultiStr == NULL)
		return 0;
	else
		listStr.clear();
	//

	while (szMultiStr[nOffset] != NULL) {

		pDest = &(szMultiStr[nOffset]);
		stemp = pDest;
		listStr.push_back(stemp);

		nOffset += stemp.length() + 1;//for passing null termination
		nCount++;
	}//while

	return nCount;
}

bool ChidbootDlg::fun_get_device_list(std::list<std::wstring>& listdev)
{
	listdev.resize(0);

	std::vector<wchar_t> vBuf;

	DWORD dwSize = m_Dll.LPU237_fw_get_list(NULL);

	if (dwSize > 0 && dwSize != LPU237_FW_RESULT_ERROR) {

		// dwSize : need memory size( unit : byte )
		vBuf.resize(dwSize, 0);

		UINT nDev = m_Dll.LPU237_fw_get_list(reinterpret_cast<LPTSTR>(&vBuf[0]));
		// the number of found device. = nDev

		GetStringFromMultiString(listdev, reinterpret_cast<LPCTSTR>(&vBuf[0]));

		if (nDev > 0) {
			return true;
		}
	}

	return false;
}


void ChidbootDlg::OnBnClickedButtonUpdate()
{
	DWORD dw_result;
	if (is_all_zero_name())
		dw_result = m_Dll.LPU237_fw_msr_update_wnd(NULL, this->m_hWnd, WM_MSR, m_sRom.c_str(), m_dw_fw_index);
	else
		dw_result = m_Dll.LPU237_fw_msr_update_wnd(&m_vId[0], this->m_hWnd, WM_MSR, m_sRom.c_str(), m_dw_fw_index);
	if (dw_result == LPU237_FW_RESULT_ERROR) {
		pushInfoQ(_T("update starting : error "));
	}
	else if (dw_result == LPU237_FW_RESULT_NO_MSR) {
		pushInfoQ(_T("update starting : not found your MSR with ID. restart a connected bootloader. "));
		fill(begin(m_vId), end(m_vId), 0);
		//OnBnClickedButtonUpdateMsg();
	}
	else {
		pushInfoQ(_T("update starting : success "));
	}

}
