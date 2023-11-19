// SerialCtrlDemoDlg.h : header file
//

#pragma once
#include "SerialCtrl.h"
#include "afxwin.h"

// CSerialCtrlDemoDlg dialog
class CSerialCtrlDemoDlg : public CDialog,public CSerialIO
{
// Construction
public:
	CSerialCtrlDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SERIALCTRLDEMO_DIALOG };

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
public:
	virtual void OnEventOpen(BOOL bSuccess);
	virtual void OnEventClose(BOOL bSuccess);
	virtual void OnEventRead(char *inPacket,int inLength);
	virtual void OnEventWrite(int nWritten);
	CComboBox m_comboSN;
	CComboBox m_comboBR;
	CButton m_btnOpen;
	CEdit m_editWrite;
	CListBox m_listboxRead;
	CStatic m_staticInfo;
	BOOL bPortOpened;
	afx_msg void OnBnClickedButtonOpen();
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedButtonWr();
	afx_msg void OnLbnSelchangeListRead();
};
