// midiKernelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "midiKernel.h"
#include "midiKernelDlg.h"

#include "pMidiEvent.h"

#define IDC_TIMER1 100

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//----------------------------------------
// some variables global to this file
//----------------------------------------
float onsetTimes[MAXTHREADS];
extern CListBox *clist;
char msg[1025];
extern MMRESULT rc;
extern UINT nDevId;
extern HMIDISTRM hms;
MIDIHDR *pmh[2];
MIDIEVENT *midiBuffers[2];
extern short currentBuffer;
MIDIPROPTEMPO mpt;
MIDIPROPTIMEDIV mptd;

int WM_EXPIRE = RegisterWindowMessage ("EXPIREMESSAGE");

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMidiKernelDlg dialog

CMidiKernelDlg::CMidiKernelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMidiKernelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMidiKernelDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMidiKernelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMidiKernelDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMidiKernelDlg, CDialog)
	//{{AFX_MSG_MAP(CMidiKernelDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_REGISTERED_MESSAGE(WM_EXPIRE, OnExpire)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMidiKernelDlg message handlers

BOOL CMidiKernelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	CString strAboutMenu;
	strAboutMenu.LoadString(IDS_ABOUTBOX);
	if (!strAboutMenu.IsEmpty())
	{
		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	pk = NULL;
	//clist = (CListBox *) GetDlgItem(IDC_MESSAGES);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMidiKernelDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMidiKernelDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMidiKernelDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMidiKernelDlg::OnPlay() 
{
	// TODO: Add your control notification handler code here
	
	// hard code this shit for now

	int i;

	for (i=0; i<MAXTHREADS; i++)
	{
		onsetTimes[i] = 0.0;
	}

	pk = new pKernel();
	pk->load("c:\\users\\kyle\\thesis\\scores\\twinkle.pk");
	beats = 4;
	
	// initialise the new kernel and connect the
	// input and output channels

	pk->init(beats, (this->out), this->event);

	// Set a timer:

	clist = (CListBox *) GetDlgItem(IDC_MESSAGES);
	clist->AddString("Starting pKernel...");

	//timeSetEvent(1000, 1, this->onTimer, NULL, TIME_ONESHOT);

	// initialise the MIDI device:
	// open the MIDI stream

	rc = midiStreamOpen(&hms, &nDevId, 1, (DWORD)m_hWnd, (DWORD)NULL, CALLBACK_WINDOW);

	if (rc != MMSYSERR_NOERROR)
	{
		midiOutGetErrorText(rc, msg, 1025);
		MessageBox(msg, NULL, MB_OK);
	}

	// display MIDI stream default properties

	mpt.cbStruct = sizeof(MIDIPROPTEMPO);
	mptd.cbStruct = sizeof(MIDIPROPTIMEDIV);

	midiStreamProperty(hms, (LPBYTE)&mpt, MIDIPROP_GET | MIDIPROP_TEMPO);
	midiStreamProperty(hms, (LPBYTE)&mptd, MIDIPROP_GET | MIDIPROP_TIMEDIV);

	sprintf(msg, "Tempo:%ld Time Div:%ld", mpt.dwTempo, mptd.dwTimeDiv);
	clist->InsertString(-1, msg);

	currentBuffer = 0;

	// allocate the MIDIHDR structures

	pmh[0] = (MIDIHDR *) HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MIDIHDR));
	pmh[1] = (MIDIHDR *) HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MIDIHDR));

	SetTimer(IDC_TIMER1, 1000, NULL);
}

void CMidiKernelDlg::OnStop() 
{
	// TODO: Add your control notification handler code here
	
	KillTimer(IDC_TIMER1);
	
	if (pk != NULL)
	{
		delete pk;
	}

	clist->ResetContent();
}

void __cdecl CMidiKernelDlg::out(pNote *outList)
{
	char message[80];
	static pMidiEvent midiList;
	pMidiEvent *tempME;
	pMidiEvent *tempME2;
	long midiListLen, midiIndex;
	float lastAbs;
	float relative;

	int i;

	outList = (pNote *)this;	// why the fuck?  bug? or just
								// the weird calling convention?

	if (outList != NULL)
	{
		pNote *pList;
		pList = outList;

		while (pList)
		{
			if (pList->pitchClass != REST)
			{
				sprintf(message, "i%d %f %f %f %d.%02d",
				  pList->thread+1, onsetTimes[pList->thread], pList->fdur, (float)(pList->baggage[0]), pList->octave+5, pList->pitchClass);

				clist->InsertString(-1, message);
			}
			else
			{
				sprintf(message, ";i%d rest: duration %2.4f", pList->thread+1, pList->fdur);

				clist->InsertString(-1, message);
			}

			// add start and finish midi events to the list

			tempME = new pMidiEvent(pList, onsetTimes[pList->thread], PME_START);
			tempME2 = new pMidiEvent(pList, onsetTimes[pList->thread] + pList->fdur, PME_FINISH);

			midiList.insert(tempME);
			midiList.insert(tempME2);

			// increment the onset time for this thread
			onsetTimes[pList->thread]+= pList->fdur;

			// get the next note in the list
			pList = pList->nextNote;
		}
	}
	else
	{
		// we have done all the gathering for this call
		// so now send the midi list to the output
		// device

		if (midiList.next != NULL)
		{
			// output the midi

			// 1. get the length of the list

			tempME = midiList.next;
			midiListLen = 0;
			lastAbs = midiList.absoluteTime;

			while (tempME)
			{
				midiListLen += 1;

				// set the stream ID

				tempME->midiEvent.dwStreamID = 0;

				// set the delta time

				relative = tempME->absoluteTime-lastAbs;
				lastAbs = tempME->absoluteTime;

				// this a temporary hack

				tempME->midiEvent.dwDeltaTime = mptd.dwTimeDiv * 4 * relative;

				// and zero the parameter block

				tempME->midiEvent.dwParms[0] = 0x00;

				// get the next one

				tempME = tempME->next;
			}

			// now we know exactly how big our block is

			midiBuffers[currentBuffer] = (MIDIEVENT *) HeapAlloc(GetProcessHeap(), 
						HEAP_ZERO_MEMORY, midiListLen * sizeof(MIDIEVENT));
			
			// fill the new memory

			midiIndex = 0;

			tempME = midiList.next;

			while (tempME)
			{
				memcpy( &(midiBuffers[currentBuffer][midiIndex]), &(tempME->midiEvent), sizeof(MIDIEVENT));

				midiIndex++;

				tempME = tempME->next;
			}

			// set up the header

			memset(pmh[currentBuffer], 0, sizeof(MIDIHDR));

			pmh[currentBuffer]->dwBytesRecorded = midiListLen * sizeof(MIDIEVENT);
			pmh[currentBuffer]->lpData = (char *) midiBuffers[currentBuffer];
			pmh[currentBuffer]->dwBufferLength = midiListLen * sizeof(MIDIEVENT);
			pmh[currentBuffer]->dwFlags = MHDR_ISSTRM;

			// send the stream

			rc = midiOutPrepareHeader((HMIDIOUT)hms, pmh[currentBuffer], sizeof(MIDIHDR));

			if (rc == MMSYSERR_NOERROR)
			{
				rc = midiStreamRestart(hms);
			}

			if (rc == MMSYSERR_NOERROR)
			{
				rc = midiStreamOut(hms, pmh[currentBuffer], sizeof(MIDIHDR));
			}

			if (rc != MMSYSERR_NOERROR)
			{
				midiOutGetErrorText(rc, msg, 1025);
				::MessageBox(NULL, msg, NULL, MB_OK);

			}

			// change buffers for the next time

			currentBuffer = (currentBuffer + 1)%2;

			// destroy the midi list

			delete midiList.next;
			midiList.next == NULL;
		}
	}
}

void CMidiKernelDlg::event(pEvent *eventList)
{

}

void CALLBACK CMidiKernelDlg::onTimer (UINT timerId, UINT uMsg, DWORD dwUser, DWORD dwParam1, DWORD dwParam2)
{
	//PostMessage(WM_EXPIRE, 0, 0);
}

LRESULT CMidiKernelDlg::OnExpire(WPARAM wParam, LPARAM lParam)
{
	// do something useful
	CListBox *clist = (CListBox *) GetDlgItem(IDC_MESSAGES);
	clist->AddString("Timer Expired...");

	return (LRESULT) TRUE;
};

void CMidiKernelDlg::OnTimer(UINT nIDEvent) 
{
	long milliseconds;
	// TODO: Add your message handler code here and/or call default

	midiOutUnprepareHeader((HMIDIOUT)hms, pmh[currentBuffer], sizeof(MIDIHDR));
	midiStreamStop(hms);

	KillTimer(IDC_TIMER1);

	milliseconds = pk->go(beats) / 1000;

	SetTimer(IDC_TIMER1, milliseconds, NULL);

	//CListBox *clist = (CListBox *) GetDlgItem(IDC_MESSAGES);
	//clist->AddString("Timer Expired...");

	CDialog::OnTimer(nIDEvent);
}

BOOL CMidiKernelDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	switch ( LOWORD (wParam) )
	{
	case MM_STREAM_DONE:
		// unprepare the data block
		break;
	default:
		break;
	}

	return CDialog::OnCommand(wParam, lParam);
}
