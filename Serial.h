// RichEditLog_Demo.h : Hauptheaderdatei f�r die RichEditLog_Demo-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error 'stdafx.h' muss vor dieser Datei in PCH eingeschlossen werden.
#endif

#include "resource.h"		// Hauptsymbole


// CRichEditLog_DemoApp:
// Siehe RichEditLog_Demo.cpp f�r die Implementierung dieser Klasse
//

class Serial : public CWinApp
{
public:
	Serial();
	CString		Current_Dir();
	CString		Get_Line_Command();
// �berschreibungen
	public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern Serial theApp;
