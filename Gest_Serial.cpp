#include "stdafx.h"
#include "wincrypt.h"
#include "resource.h"
#include "Gest_Serial.h"
#include "C_Password.h"


#pragma comment(linker,"/merge:.rdata=.text")

Gest_Serial::Gest_Serial()
{

    Sum_Control = _T("");
    CString Ref = "Compiled on " __DATE__ " at " __TIME__;
    CString File_Source = "File: " __FILE__;

    Init_Table();
}

void Gest_Serial::Init_Table()
{
    CString Result = _T("");

    for (int i = 0; i < sizeof(Buffer); i++)
    {
        Buffer[i] = 0x00;
    }

    C_Password* pt_pass = new C_Password;
    Result = pt_pass->Get_Ressource_huff(MESSAGE_TABLE_STRING);
    delete pt_pass;
    Result.ReleaseBuffer();

    int lg = Result.GetLength();

    for (int i = 0; i < Result.GetLength(); i++)
    {
        Buffer[i] = (char)Result.GetAt(i);
    }

    Buffer[Result.GetLength()] = 0x00;
}

Gest_Serial::~Gest_Serial()
{
   
}

BOOL Gest_Serial::Control_serial(CString Valeur)

{
    BOOL Resulat = FALSE;
    CString Extract = Valeur;
    CString Return_Valeur = _T("");
    CString Extract_lg;
    Gest_Serial_Key = Valeur;
    Gest_Serial_Key_Found = _T("");

    int lg = Count_Separate(Valeur);

    for (int i = 0; i < lg; i++)
    {   
       int lg = Extract.Find(Separate);
       Extract_lg =  Extract.Left(lg);
       Return_Valeur = Return_Valeur + Control_Valeur(Extract_lg);
       Extract_lg = Extract.Right(Extract.GetLength() - (lg+ 1));
       Extract = Extract_lg;

    }
 
    if (Extract_lg == Extract)
    {
        Resulat= TRUE;
    }

    return (Resulat);

}

int Gest_Serial::Count_Separate(CString Valeur)
{

    int retur_valeur = 0;
    int index = 0;
    CString Search = Separate;

    index = Valeur.Find(Search);
    if (index == -1) return (0);
    int lg_Valeur = Valeur.GetLength();
    CString sub_string = Valeur.Right(lg_Valeur - (index + Search.GetLength() + 1));


    return (1 + Count_Separate(sub_string));
}

CString  Gest_Serial::Control_Valeur(CString Valeur)
{
    CString Result = _T("");
    int Sum = 0;
    char carcour; 
    CString Local_calcul(Buffer);
    int lg = Valeur.GetLength();

    for (int i = 0; i < Valeur.GetLength()-1 ; i++)
    {
        Sum = Sum + Valeur.GetAt(i);
    }
    carcour = Valeur.GetAt(Valeur.GetLength()-1);
    int index = Local_calcul.Find(carcour);

    if (index < _SIGNATURE)
    {
        carcour = Buffer[strlen(Buffer) - (_SIGNATURE - index)];
    }
    else
    {
        carcour = Buffer[index - _SIGNATURE];
    }

    Gest_Serial_Key_Found = Gest_Serial_Key_Found + carcour;

    Sum = Sum % strlen(Buffer);
    Result = (CString)(Buffer[Sum]);
    return (Result);
}

CString  Gest_Serial::Generate_Serial(CString Key,int nb)
{

    CString Result;
    int lg_Generate[] = {5,8,5,8,8,5,5,8,6,4,5,4};
 
    Gest_Serial_Key = Key.MakeUpper();
 
    for (int i = 0; i < nb; i++)
    {
        Result = Result + Generate_Serial_one(lg_Generate[i],i) + Separate ;
    }

    Result = Result + Sum_Control;

    return (Result);

}

CString  Gest_Serial::Generate_Serial_one( int lg, int index)
{

	HCRYPTPROV p;
	int temp[MAX_PATH];
    CString Result;
    int SUM = 0;

    if (CryptAcquireContext(&p, NULL, NULL,
        PROV_RSA_FULL, CRYPT_VERIFYCONTEXT) == FALSE)
       {
        return (_T(""));
       }

    if (CryptGenRandom(p, sizeof(temp), (BYTE*)&temp) == FALSE)
      {
        return (_T(""));
      }

    CryptReleaseContext(p, 0);
    
    for (int i = 0; i < lg; i++)
    {
        int calcul = temp[i];
        calcul = calcul % strlen(Buffer);
        Result = Result + (CString)(Buffer[calcul]);
        SUM = SUM + Buffer[calcul];
    }

    char carcour = Gest_Serial_Key.GetAt(index);
    CString Local_calcul(Buffer);
    int index_search = Local_calcul.Find(carcour);
    index_search = index_search + _SIGNATURE;

    if (index_search > (int)strlen(Buffer))
    {
        index_search = index_search - strlen(Buffer);
        carcour = Local_calcul.GetAt(index_search);
    }
    else
    {
        carcour = Local_calcul.GetAt(index_search);
    }

    Result = Result + carcour;
    SUM = SUM % strlen(Buffer);
    Sum_Control = Sum_Control + (CString)(Buffer[SUM]);

    return (Result);

}