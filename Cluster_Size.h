#pragma once
class Cluster_Size
{
public:

	Cluster_Size();
	~Cluster_Size();
	DWORD		Get(CString File);
	DWORD		Getx2(CString File);
	DWORD		Getx4(CString File);
	DWORD		Getx8(CString File);

private :

	CString		GetContainingFolder(CString &file);
	DWORD		Get_Current_Size_Disk_Cluster(CString File);
	CString		Current_Dir();
	CString		Get_Full_mame();
	CString		Get_Line_Command();
	CString		Get_label(int number);

};

