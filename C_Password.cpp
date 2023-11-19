#include "stdafx.h"
#include "resource.h"
#include "C_Password.h"
#include "code_huffman.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(linker,"/merge:.rdata=.text")

C_Password::C_Password()
{
	CString Ref = "Compiled on " __DATE__ " at " __TIME__;
	CString File_Source = "File: " __FILE__;
}

C_Password::~C_Password()
{
}

CString  C_Password::Get_Ressource_huff(int ID)
{
	CString Result = _T("");
	DWORD dwSize = 0;
	int lg = 0;

	HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(ID), RT_RCDATA); // IDR_MYFILE

	if (hRes != NULL)
	{

		// Load the resource and save its total size.
		dwSize = SizeofResource(NULL, hRes);
		HGLOBAL MemoryHandle = LoadResource(NULL, hRes);
		if (MemoryHandle != NULL)
		{
			char* resText = (char *)LockResource(MemoryHandle);
			char* text = (char*)malloc(dwSize + 1);

			if (text != NULL)
			{
				memcpy(text, resText, dwSize);
			}
			text[dwSize] = 0x00;
			simple_Huffman *pt_huff = new simple_Huffman;
			lg = pt_huff->Decompress((BYTE *)text, dwSize);
			CString temp( pt_huff->getOutput());
			Result = temp;
			Result.SetAt(lg, 0x00);
			pt_huff->Finalize();
			delete pt_huff;

			FreeResource(MemoryHandle);
			free(text);
		}
		FreeResource((HANDLE)hRes);
	}
	return (Result);
}


