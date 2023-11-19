//-----------------------------------------------------------------------------
// This software is documented using Doxygen. Visit www.doxygen.org to
// get a free copy of Doxygen.
//-----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
/// \file	RichEditLog_DemoDlg.cpp
/// \brief	Contains the class implementation of CRichEditLog_DemoDlg.
///
///			This file contains the class implementations of
///			CRichEditLog_DemoDlg, which is the main dialog.
///
///			This code was written and compiled with Microsoft Visual Studio
///			.NET 2003. You should be able to compile this code with any
///			later version of Visual Studio or Visual C++.
/// 
/// \section LICENSE
///
/// This software is licensed under the terms and condfitions of the Code
/// Project Open License (CPOL), which you can find under
/// http://www.codeproject.com/info/cpol10.aspx (if not, go to 
/// http://www.codeproject.com and search for "The Code Project Open License").
///
/// This program is distributed WITHOUT ANY WARRANTY; without even the implied
/// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "stdafx.h"
#include "stdint.h"
#include <sys/timeb.h>  
#include <time.h>  
#include "resource.h"
#include "Color.h"
#include "Gest_Serial.h"
#include "SerialDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//-----------------------------------------------------------------------------
// Message map
//-----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(SerialDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_INSERT_BLACK, OnBnClickedInsertBlack)
	ON_BN_CLICKED(IDC_INSERT_RED, OnBnClickedInsertRed)
	ON_BN_CLICKED(IDC_INSERT_GREEN, OnBnClickedInsertGreen)
	ON_BN_CLICKED(IDC_INSERT_BLACK_EX, OnBnClickedInsertBlackEx)
	ON_BN_CLICKED(IDC_INSERT_RED_EX, OnBnClickedInsertRedEx)
	ON_BN_CLICKED(IDC_INSERT_GREEN_EX, OnBnClickedInsertGreenEx)
END_MESSAGE_MAP()


SerialDlg::SerialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SerialDlg::IDD, pParent)
	, Key_Serial(_T(""))
{
	// m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void SerialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT21, m_ctrlLog);
	DDX_Text(pDX, IDC_EDIT1, Key_Serial);
}


BOOL SerialDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

	// TODO: Hier zusätzliche Initialisierung einfügen
	
	return TRUE;  // Geben Sie TRUE zurück, außer ein Steuerelement soll den Fokus erhalten
}


void SerialDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext zum Zeichnen

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Symbol in Clientrechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR SerialDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void SerialDlg::OnBnClickedInsertBlack()
{
	AppendToLog("This is black text.\n", RGB(0, 0, 0));
}

void SerialDlg::OnBnClickedInsertRed()
{
	AppendToLog("This is red text.\r\nThis is the second line of red text without a line break.", RGB(255, 0, 0));
}


void SerialDlg::OnBnClickedInsertGreen()
{
	AppendToLog("This is dark green text.\nThis is the second line of green text with a line break.\r\n", RGB(0, 0x99, 0));
}


void SerialDlg::OnBnClickedInsertBlackEx()
{
	AppendToLogAndScroll("This is black text of improved version.\n", RGB(0, 0, 0));
}

void SerialDlg::idle()
{
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
		::PostQuitMessage(0);
		break;
		}

		if (!AfxGetApp()->PreTranslateMessage(&msg))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
}

void SerialDlg::OnBnClickedInsertRedEx()
{	
	UpdateData(TRUE);
	if (Key_Serial.GetLength() > MB_MAX_SECTION)
	{
		CString Temp;
		Temp.Format("Key length must be lower than %d", MB_MAX_SECTION);
		AfxMessageBox(Temp, MB_OK);
		Key_Serial = _T("");
		UpdateData(FALSE);
	} 
	else
	{
		if (Key_Serial.GetLength() < 5)
		{
			CString Temp;
			Temp.Format("Key length must be upper than %d", MB_MIN_SECTION);
			AfxMessageBox(Temp, MB_OK);
			Key_Serial = _T("");
			UpdateData(FALSE);
		}
		else
		{ 
		 Clear_Result();
		 UpdateData(FALSE);
		 Gest_Serial* pt_gest_serial = new Gest_Serial;
		 CString Result = pt_gest_serial->Generate_Serial(Key_Serial, Key_Serial.GetLength());
		 AppendToLogAndScroll(Result + _T("\n"), RED);
		 Copy_To();
		 Message();
		 UpdateData(FALSE);
		 bool tt = pt_gest_serial->Control_serial(Result);

		 if (pt_gest_serial->Control_serial(Result) == TRUE)
		 {

			 AfxMessageBox(pt_gest_serial->Gest_Serial_Key_Found, MB_OK);
		 }
		 delete pt_gest_serial;
		}
    }
}

void SerialDlg::Message()
{
	::SetDlgItemTextA(GetSafeHwnd(), IDC_STATIC, _T("Value copied to the clipboard"));
}

void SerialDlg::Copy_To()
{
	m_ctrlLog.SetSel(0, -1);
	m_ctrlLog.Copy();

}


CString SerialDlg::Current_Dir()
{
	return(Sepcial_Folder(CSIDL_PERSONAL) + _T("\\"));
}

CString SerialDlg::Sepcial_Folder(int _directory_resquest)
{

	TCHAR szPath[MAX_PATH] = _T("");

	SHGetFolderPath(NULL, _directory_resquest, NULL, 0, szPath);

	CString chaine(szPath);
	return (chaine);

}

void SerialDlg::OnBnClickedInsertGreenEx()
{
	AppendToLogAndScroll("This is darker green text of improved version.\nThis is its second line\r\nand this the third line ending with a line break.\n", RGB(0, 128, 0));
}


int SerialDlg::AppendToLog(CString str, COLORREF color)
{
	int nOldLines = 0, nNewLines = 0, nScroll = 0;
	long nInsertionPoint = 0;
	CHARRANGE cr;
	CHARFORMAT cf;

	// Save number of lines before insertion of new text
	nOldLines		= m_ctrlLog.GetLineCount();

	// Initialize character format structure
	cf.cbSize		= sizeof(CHARFORMAT);
	cf.dwMask		= CFM_COLOR;
	cf.dwEffects	= 0;	// To disable CFE_AUTOCOLOR
	cf.crTextColor	= color;

	// Set insertion point to end of text
	nInsertionPoint = m_ctrlLog.GetWindowTextLength();
	nInsertionPoint = -1;
	m_ctrlLog.SetSel(nInsertionPoint, -1);

    //  Set the character format
    m_ctrlLog.SetSelectionCharFormat(cf);

	// Replace selection. Because we have nothing selected, this will simply insert
	// the string at the current caret position.
	m_ctrlLog.ReplaceSel(str);

	// Get new line count
	nNewLines		= m_ctrlLog.GetLineCount();

	// Scroll by the number of lines just inserted
	nScroll			= nNewLines - nOldLines;
	m_ctrlLog.LineScroll(nScroll);

	return 0;
}

void SerialDlg::Clear_Result()
{

	m_ctrlLog.SetSel(0, -1);
	m_ctrlLog.Clear();
	m_ctrlLog.ReplaceSel(_T(""));
}

int SerialDlg::AppendToLogAndScroll(CString str, COLORREF color)
{
	long nVisible = 0;
	long nInsertionPoint = 0;
	CHARFORMAT cf;
	
	// Initialize character format structure
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.dwEffects = 0; // To disable CFE_AUTOCOLOR

	cf.crTextColor = color;

	// Set insertion point to end of text
	
	nInsertionPoint = m_ctrlLog.GetWindowTextLength();
	m_ctrlLog.SetSel(nInsertionPoint, -1); // 
	
	// Set the character format
	m_ctrlLog.SetSelectionCharFormat(cf);

	// Replace selection. Because we have nothing 
	// selected, this will simply insert
	// the string at the current caret position.
	m_ctrlLog.ReplaceSel(str);

	// Get number of currently visible lines or maximum number of visible lines
	// (We must call GetNumVisibleLines() before the first call to LineScroll()!)
	nVisible   = GetNumVisibleLines(&m_ctrlLog);

	// Now this is the fix of CRichEditCtrl's abnormal behaviour when used
	// in an application not based on dialogs. Checking the focus prevents
	// us from scrolling when the CRichEditCtrl does so automatically,
	// even though ES_AUTOxSCROLL style is NOT set.
	if (&m_ctrlLog != m_ctrlLog.GetFocus())
	{
		m_ctrlLog.LineScroll(INT_MAX);
		m_ctrlLog.LineScroll(1 - nVisible);
	}

	return 0;
}

int SerialDlg::GetNumVisibleLines(CRichEditCtrl* pCtrl)
{
	CRect rect;
	long nFirstChar, nLastChar;
	long nFirstLine, nLastLine;

	// Get client rect of rich edit control
	pCtrl->GetClientRect(rect);

	// Get character index close to upper left corner
	nFirstChar = pCtrl->CharFromPos(CPoint(0, 0));

	// Get character index close to lower right corner
	nLastChar = pCtrl->CharFromPos(CPoint(rect.right, rect.bottom));
	if (nLastChar < 0)
	{
		nLastChar = pCtrl->GetTextLength();
	}

	// Convert to lines
	nFirstLine = pCtrl->LineFromChar(nFirstChar);
	nLastLine  = pCtrl->LineFromChar(nLastChar);

	return (nLastLine - nFirstLine);
}
