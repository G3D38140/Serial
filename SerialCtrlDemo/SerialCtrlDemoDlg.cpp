// SerialCtrlDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SerialCtrlDemo.h"
#include "SerialCtrlDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern FILE *pt_fichier;
HINSTANCE hinst;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSerialCtrlDemoDlg dialog




CSerialCtrlDemoDlg::CSerialCtrlDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSerialCtrlDemoDlg::IDD, pParent)
	, bPortOpened(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialCtrlDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SN, m_comboSN);
	DDX_Control(pDX, IDC_COMBO_BR, m_comboBR);
	DDX_Control(pDX, IDC_BUTTON_OPEN, m_btnOpen);
	DDX_Control(pDX, IDC_EDIT_WRITE, m_editWrite);
	DDX_Control(pDX, IDC_LIST_READ, m_listboxRead);
	DDX_Control(pDX, IDC_STATIC_INFO, m_staticInfo);
}

BEGIN_MESSAGE_MAP(CSerialCtrlDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CSerialCtrlDemoDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_WR, &CSerialCtrlDemoDlg::OnBnClickedButtonWr)
	ON_LBN_SELCHANGE(IDC_LIST_READ, &CSerialCtrlDemoDlg::OnLbnSelchangeListRead)
END_MESSAGE_MAP()


// CSerialCtrlDemoDlg message handlers

BOOL CSerialCtrlDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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

	m_comboSN.InsertString(0,_T("COM1"));
	m_comboSN.InsertString(1,_T("COM2"));
	m_comboSN.InsertString(2,_T("COM3"));
	m_comboSN.InsertString(3,_T("COM4"));
	m_comboSN.InsertString(4,_T("COM5"));
	m_comboSN.InsertString(5,_T("COM6"));
	m_comboSN.InsertString(6,_T("COM7"));
	m_comboSN.InsertString(7,_T("COM8"));
	m_comboSN.InsertString(8,_T("COM9"));
	m_comboSN.InsertString(9,_T("COM10"));
	m_comboSN.SetCurSel(0);

	m_comboBR.InsertString(0,_T("1200"));
	m_comboBR.InsertString(1,_T("2400"));
	m_comboBR.InsertString(2,_T("4800"));
	m_comboBR.InsertString(3,_T("9600"));
	m_comboBR.InsertString(4,_T("19200"));
	m_comboBR.InsertString(5,_T("115200"));
	m_comboBR.SetCurSel(0);

	hinst = AfxGetInstanceHandle();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSerialCtrlDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSerialCtrlDemoDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSerialCtrlDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSerialCtrlDemoDlg::OnEventOpen(BOOL bSuccess)
{
	CString str;
	if(bSuccess)
	{
		str=m_strPortName + _T(" Port Ouvert");

		bPortOpened=TRUE;
		m_btnOpen.SetWindowText(_T("Close"));
		
	}
	else
	{
		str=m_strPortName +  _T("Erreur lors de l'ouverture");
	}
	m_staticInfo.SetWindowText(str);
}

void CSerialCtrlDemoDlg::OnEventClose(BOOL bSuccess)
{
	CString str;
	if(bSuccess)
	{
		str=m_strPortName + _T(" Fermeture OK");
		bPortOpened=FALSE;
		m_btnOpen.SetWindowText(_T("Open"));

	}
	else
	{
		str=m_strPortName + _T("Erreur lors de la fermeture");
	}
	m_staticInfo.SetWindowText(str);
}

void CSerialCtrlDemoDlg::OnEventRead(char *inPacket,int inLength)
{
	char buffer[250];

	fwrite(inPacket,sizeof(char),inLength,pt_fichier);
	m_listboxRead.AddString( (LPCTSTR) inPacket);

	sprintf(&buffer[0],"%d bytes lus",inLength);
	m_staticInfo.SetWindowText((LPCTSTR ) &buffer[0]);
	
}
void CSerialCtrlDemoDlg::OnEventWrite(int nWritten)
{
	char buffer[250];

	if(nWritten>0)
	{
		sprintf(&buffer[0],"%d bytes écris",nWritten);
		m_staticInfo.SetWindowText((LPCTSTR ) &buffer[0]);
	}
	else
	{
		m_staticInfo.SetWindowText(_T("Erreur d'écriture"));
	}

}

void CSerialCtrlDemoDlg::OnBnClickedButtonOpen()
{
	// TODO: Add your control notification handler code here
    if(bPortOpened==FALSE)
	{
		CString strPortName;
		CString strBaudRate;
		m_comboSN.GetLBText(m_comboSN.GetCurSel(),strPortName);
		m_comboBR.GetLBText(m_comboBR.GetCurSel(),strBaudRate);
		OpenPort(strPortName,strBaudRate);

	}
	else
	{
		// IDC_BUTTON_OPEN
		ClosePort();
		m_staticInfo.SetWindowText(_T("Fermeture du port"));

	}
	
}

void CSerialCtrlDemoDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//__super::OnOK();
}

void CSerialCtrlDemoDlg::OnBnClickedButtonWr()
{
	// TODO: Add your control notification handler code here

	CString strW;
	char buffer[250];

	m_editWrite.GetWindowText(strW);
	sprintf(&buffer[0],"%s",strW);
	Write(&buffer[0], strW.GetLength());
}


void CSerialCtrlDemoDlg::OnLbnSelchangeListRead()
{
	// TODO: Add your control notification handler code here
}
