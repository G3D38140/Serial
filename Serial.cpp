//-----------------------------------------------------------------------------
// This software is documented using Doxygen. Visit www.doxygen.org to
// get a free copy of Doxygen.
//-----------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
/// \file	RichEditLog_Demo.cpp
/// \brief	Contains the class implementation of CRichEditLog_DemoApp.
///
///			This file contains the class implementations of
///			CRichEditLog_DemoApp, which is the main application.
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
#include "stdafx.h"
#include "serial.h"
#include "SerialDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//=============================================================================
// Global variables
//=============================================================================
/// The one and only application object.
Serial theApp;


///////////////////////////////////////////////////////////////////////////////
/// \class		CRichEditLog_DemoApp
/// \author		V. Typke
/// \date		2005-10-20 - 2011-05-26 (last modified)
/// \version	1.0
///
/// \section	DESCRIPTION
/// This is the dialog class implementation for the "RichEditLog_Demo"
/// application.
///////////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
// Message map
//-----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(Serial, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


//-----------------------------------------------------------------------------
//  CRichEditLog_DemoApp()
///
/// \brief	Constructor of the class.
//-----------------------------------------------------------------------------
Serial::Serial()
{
	// TODO: Hier Code zur Konstruktion einfügen
	// Alle wichtigen Initialisierungen in InitInstance positionieren
}


CString Serial::Current_Dir()
{
	return (Get_Line_Command());
}

CString Serial::Get_Line_Command()
{
	LPWSTR* szArglist;
	CString Result = _T("");
	int nArgs;
	int i;

	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	if (NULL == szArglist)
	{
		wprintf(L"CommandLineToArgvW failed\n");
		return (Result);
	}
	else
	{
		for (i = 0; i < nArgs; i++)
		{
			Result = szArglist[i];
		}
	}
	// Free memory allocated for CommandLineToArgvW arguments.
	LocalFree(szArglist);
	int index = Result.ReverseFind('\\');
	Result = Result.Left(index + 1);
	return(Result);
}

//-----------------------------------------------------------------------------
//  InitInstance()
///
/// \brief	Initialization of the application.
//-----------------------------------------------------------------------------
BOOL Serial::InitInstance()
{
	bool b = false;

	// Initialize common controls
	InitCommonControls();

	// Call parent class constructor
	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Initialize CRichEditCtrl. This function basically loads "RICHED20.dll",
	// which contains version 2.0 (and/or possibly 3.0) of CRichEditCtrl.
	// If this fails, you might have an old system not supporting version 2.0.
	// In that case, use AfxInitRichEdit() instead to loads RICHED32.DLL to
	// initialize version 1.0 of the rich edit control.
	// If you have dialog resources with the rich edit control created prior
	// to Visual C++ .NET, the rich edit controls are automatically version 1.0.
	// Rich edit controls inserted using the Visual C++ .NET Resource Editor
	// are version 2.0. 
	// So, if you roll back to Version 1.0, please open the .rc-file as text
	// and change the class name of each rich edit control from "RichEdit20a"
	// to "RICHEDIT".
	// This function also initializes the common controls library, if the
	// library hasn't already been initialized for the process. Here, this 
	// has happened already by the call to InitCommonControls().
	 AfxInitRichEdit2();

	
	SetRegistryKey(_T("SeialDemo"));

	SerialDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	return FALSE;
}
