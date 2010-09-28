// midiKernelDlg.h : header file
//

#include <mmsystem.h>

#include "..\include\pKernel.hpp"

/////////////////////////////////////////////////////////////////////////////
// CMidiKernelDlg dialog

class CMidiKernelDlg : public CDialog
{
// Construction
public:
	//CListBox * clist;
	void CALLBACK onTimer (UINT timerId, UINT uMsg, DWORD dwUser, DWORD dwParam1, DWORD dwParam2);
	void __cdecl event(pEvent *);
	void __cdecl out(pNote *);
	CMidiKernelDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMidiKernelDlg)
	enum { IDD = IDD_MIDIKERNEL_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMidiKernelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	pKernel *pk;
	short beats;

	// Generated message map functions
	//{{AFX_MSG(CMidiKernelDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPlay();
	afx_msg void OnStop();
	afx_msg LRESULT OnExpire(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
