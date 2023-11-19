// RichEditLog_DemoDlg.h : Headerdatei
//

#pragma once
#include "afxcmn.h"
#include "resource.h"


// CRichEditLog_DemoDlg Dialogfeld
class SerialDlg : public CDialog
{
// Konstruktion
public:
	SerialDlg(CWnd* pParent = NULL);	// Standardkonstruktor

// Dialogfelddaten
	enum { IDD = IDD_RICHEDITLOG_DEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung


// Implementierung
protected:
	HICON m_hIcon;

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	CRichEditCtrl m_ctrlLog;
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedInsertBlack();
	afx_msg void OnBnClickedInsertRed();
	afx_msg void OnBnClickedInsertGreen();
	afx_msg void OnBnClickedInsertBlackEx();
	afx_msg void OnBnClickedInsertRedEx();
	afx_msg void OnBnClickedInsertGreenEx();
	void		 idle();
	void		 Clear_Result();
	void		 Copy_To();
	void		 Message();
	CString		Sepcial_Folder(int _directory_resquest);
	CString		Current_Dir();

	DECLARE_MESSAGE_MAP()
public:
	int AppendToLog(CString str, COLORREF color);
	int AppendToLogAndScroll(CString str, COLORREF color);
	int GetNumVisibleLines(CRichEditCtrl* pCtrl);
	CString Key_Serial;
};
