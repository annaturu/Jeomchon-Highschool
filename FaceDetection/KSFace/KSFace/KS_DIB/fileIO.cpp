//////////////////////////////////////////////////////////////////////
// fileIO.cpp: implementation of the fileIO class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "fileIO.h"

#include "fileTGA.h"
#include "fileRAW.h"
#include "fileBMP.h"
#include "filePGM.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
fileIO::fileIO()
{

}

fileIO::~fileIO()
{

}

BOOL fileIO::Open(gfx_dc* pdib, CString pathName)
{
	CString extension=pathName.Right(3);

	if(extension.CompareNoCase("tga")==0){
		fileTGA f;
		
		if(!f.Open(pdib, pathName))
			return FALSE;
	}

	if(extension.CompareNoCase("raw")==0){
		fileRAW f;

		if(!f.Open(pdib, pathName))
			return FALSE;
	}

	if(extension.CompareNoCase("bmp")==0){
		fileBMP f;

		if(!f.Open(pdib, pathName))
			return FALSE;
	}

	if(extension.CompareNoCase("pgm")==0){
		filePGM f;
		
		if(!f.Open(pdib, pathName))
			return FALSE;
	}

	return TRUE;
}

BOOL fileIO::Save(gfx_dc* pdib, CString pathName)
{
	CString extension=pathName.Right(3);

	if(extension.CompareNoCase("tga")==0){
		AfxMessageBox("no support yet");
		return FALSE;
	}

	if(extension.CompareNoCase("raw")==0){
		AfxMessageBox("no support yet");
		return FALSE;
	}

	if(extension.CompareNoCase("bmp")==0){
		AfxMessageBox("no support yet");
		return FALSE;
	}

	if(extension.CompareNoCase("pgm")==0){
		filePGM f;
		
		if(!f.Save(pdib, pathName))
		return FALSE;
	}

	return TRUE;
}
