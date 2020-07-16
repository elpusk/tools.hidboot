
// hidbootDlg.h : header file
//

#pragma once

#include <string>
#include <vector>
#include <mutex>
#include <queue>
#include <list>

#include "Lpu237Fw.h"

#define	WM_MSR		WM_APP+100

// ChidbootDlg dialog
class ChidbootDlg : public CDialogEx
{
// Construction
public:
	ChidbootDlg(CWnd* pParent = nullptr);	// standard constructor

private:
	typedef	std::vector<unsigned char>	_type_v_buffer;
	typedef	std::tuple<std::wstring, COLORREF>	_type_info_item;
	typedef	std::queue<_type_info_item>			_type_info_queue;
	typedef	std::queue<DWORD>					_type_index_queue;

	enum {
		TIMER_ID = 10000,
		TIMER_INTERVAL = 30,
		TIMER_ID_CARD_CHK = 10001,
		TIMER_INTERVAL_CARD_CHK = 1000,
		TIMER_COMPLETE_SUCCESS_ID = 10002,
		TIMER_COMPLETE_FAIL_ID = 10002,
	};

	enum {
		UPDATE_WAIT_TIME = 180 * 1000	//unit mmsec
	};

	static const unsigned char s_default_dev_name[16];


private:
	CLpu237Fw m_Dll;
	HANDLE m_hDev;
	int m_nIndex;
	ChidbootDlg::_type_v_buffer m_vId;
	ChidbootDlg::_type_v_buffer m_vName;
	ChidbootDlg::_type_v_buffer m_vVersion;
	std::wstring m_sRom;
	DWORD m_dw_fw_index;
	//
	HANDLE m_hThread;
	unsigned int m_nThreadID;

	HANDLE m_hEvtKillThread;
	HANDLE m_hRunThread;

	ChidbootDlg::_type_info_queue m_InfoQ;
	std::mutex m_mutex;

	ChidbootDlg::_type_index_queue m_IndexQ;
	std::mutex m_mutexIndex;


private:
	int _setup_device_list();
	bool _setup_target_device();
	bool _setup_fw_file();
	bool _start_update();

	int GetStringFromMultiString(std::list<std::wstring>& listStr, LPCTSTR szMultiStr);

	void AddInfo(LPCTSTR lpInfo);

	bool fun_get_device_list(std::list<std::wstring>& listdev);

	bool is_all_zero_name()
	{
		bool b_is_all_zero(true);

		for_each(begin(m_vId), end(m_vId), [&](unsigned char c) {
			if (c != 0)
				b_is_all_zero = false;
			});

		return b_is_all_zero;
	}

	void pushInfoQ(LPCTSTR lpstrInfo, COLORREF color = RGB(0, 0, 0))
	{
		if (lpstrInfo == NULL)
			return;

		std::lock_guard<std::mutex> lock(m_mutex);
		m_InfoQ.push(std::make_tuple(std::wstring(lpstrInfo), color));
	}

	_type_info_item popInfoQ()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		_type_info_item item;

		if (!m_InfoQ.empty()) {
			item = m_InfoQ.front();
			m_InfoQ.pop();
		}
		return item;
	}

	void pushIndexQ(DWORD dwIndex)
	{
		std::lock_guard<std::mutex> lock(m_mutexIndex);
		m_IndexQ.push(dwIndex);
	}

	DWORD popIndexQ()
	{
		std::lock_guard<std::mutex> lock(m_mutexIndex);
		DWORD dwIndex(0);

		if (!m_IndexQ.empty()) {
			dwIndex = m_IndexQ.front();
			m_IndexQ.pop();
		}

		return dwIndex;
	}

	bool IsEmptyIndexQ()
	{
		std::lock_guard<std::mutex> lock(m_mutexIndex);

		if (!m_IndexQ.empty())
			return false;
		else
			return true;
	}
	//
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HIDBOOT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnMSR(WPARAM wParam, LPARAM lParam);
public:
	CListBox m_listboxInfo;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonUpdate();
	CComboBox m_ComboDevList;
};
