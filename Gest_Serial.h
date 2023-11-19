#pragma once
class Gest_Serial
{
public:

	~Gest_Serial();
	Gest_Serial();
	CString     Generate_Serial(CString Key, int lg);
	BOOL		Control_serial(CString Valuer);
	CString     Gest_Serial_Key_Found = _T("");

private:

	char     Buffer[MAX_PATH] = "";
    CString  Generate_Serial_one(int lg,int index);
	CString  Control_Valeur(CString Valeur);
	int		 Count_Separate(CString Valeur);
	void	 Init_Table();

	CString  Separate = _T("-");
	CString  Gest_Serial_Key = _T("");
	CString	 Sum_Control = _T("";);

};

