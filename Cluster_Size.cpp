#include "stdafx.h"
#include "resource.h"
#include "Cluster_Size.h"
#include "C_Password.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Cluster_Size::Cluster_Size()
{
	CString Ref = "Compiled on " __DATE__ " at " __TIME__;
	CString File_Source = "File: " __FILE__;
}


Cluster_Size::~Cluster_Size()
{
}

CString Cluster_Size::Get_label(int number)
{
	C_Password* pt_Pass = new C_Password;
	CString Mess;
	Mess = pt_Pass->Get_Ressource_huff(number);
	delete pt_Pass;
	return (Mess);
}

CString Cluster_Size::Current_Dir()
{
	CString Chaine = Get_Line_Command();

	if (Chaine.IsEmpty())
	{
		// The Current Directory could'nt found !!!
		AfxMessageBox(_T("MESSAGE_NO_DIECTORY_FOUND"), MB_OK);
	}
	return (Chaine);
}

CString Cluster_Size::Get_Line_Command()
{
    LPWSTR* szArglist;
    CString Result = _T("");
    int nArgs;
    int i;

    szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);

	if (nArgs > 1) { AfxMessageBox(_T("MESSAGE_No_Parametres"), MB_OK); return(Result); }

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

CString Cluster_Size::Get_Full_mame()
{
	CString Full_name = Current_Dir() + AfxGetAppName() + _T(".EXE");
	return (Full_name );

}

CString Cluster_Size::GetContainingFolder(CString &file)
{
	CFileFind fileFind;
	fileFind.FindFile(file);
	fileFind.FindNextFile();
	return (fileFind.GetRoot());
}

DWORD Cluster_Size::Get_Current_Size_Disk_Cluster(CString File)
{
	DWORD lpSectorsPerCluster;
	DWORD lpBytesPerSector;
	DWORD lpNumberOfFreeClusters;
	DWORD lpTotalNumberOfClusters;
	CString Chaine;
	
	Chaine = GetContainingFolder(File);
	Chaine.SetAt(3, 0x00);
	GetDiskFreeSpace(Chaine, &lpSectorsPerCluster, &lpBytesPerSector, &lpNumberOfFreeClusters, &lpTotalNumberOfClusters);
	return (lpSectorsPerCluster * lpBytesPerSector);
}


DWORD	Cluster_Size::Getx2( CString File)
{
	return (2 * Get_Current_Size_Disk_Cluster(File));
}

DWORD	Cluster_Size::Getx4(CString File)
{
	return (4 * Get_Current_Size_Disk_Cluster(File));
}
DWORD	Cluster_Size::Getx8(CString File)
{
	return (8 * Get_Current_Size_Disk_Cluster(File));
}

DWORD	Cluster_Size::Get(CString File)
{
	return (Get_Current_Size_Disk_Cluster(File));
}