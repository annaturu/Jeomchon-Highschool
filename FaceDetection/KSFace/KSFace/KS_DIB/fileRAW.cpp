//////////////////////////////////////////////////////////////////////
// fileRAW.cpp: implementation of the fileRAW class.
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "fileRAW.h"
#include "RawOptions.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

fileRAW::fileRAW()
{

}

fileRAW::~fileRAW()
{

}

BOOL fileRAW::Open(gfx_dc* pdib, CString pathName)
{
	
	CFile f;
	
	if(!f.Open(pathName, CFile::modeRead, NULL)){
		AfxMessageBox("Raw: can not open file");
		return FALSE;
	}

	CRawOptions* dlgOption;
	dlgOption = new CRawOptions();

	dlgOption->SetData(f.GetLength(), 
		               f.GetFileName());
	
	int		w=0;
	int		h=0;
	int		channel=0;
	DWORD	headersize=0;

	if(dlgOption->DoModal()==IDOK){
		dlgOption->GetData(&w, &h, 
			               &channel, 
						   &headersize);	
	}

	delete dlgOption;
	dlgOption=NULL;

	if((w*h*channel+headersize)>f.GetLength()){
		AfxMessageBox("Specified image is larger than file.");
		return FALSE;
	}
	
	//////////////////////////////////////////////////////////////
	// read header..
	//////////////////////////////////////////////////////////////
	if(headersize){
		BYTE* temp;
		temp=new BYTE[headersize];

		if(!f.Read(temp, headersize)){
			AfxMessageBox("file Raw: can not read file");

			delete [] temp;
			temp=NULL;
			f.Close();
			return FALSE;
		}
		delete [] temp;
		temp=NULL;
	}

	//////////////////////////////////////////////////////////////
	// read body..
	//////////////////////////////////////////////////////////////
	BYTE* buff;
	buff = new BYTE[w*h*channel];

	if(!f.Read(buff, w*h*channel)){
		AfxMessageBox("file Raw: can not read file");

		delete [] buff;
		buff=NULL;
		f.Close();
		return FALSE;
	}
	
	if(!pdib->create(w, h)){
		AfxMessageBox("file Raw: create dib fail");
		delete [] buff;
		buff=NULL;

		f.Close();
		return FALSE;
	}
	
	//////////////////////////////////////////////////////////////
	// set image by channel.  무식하다. 수정할것.
	//////////////////////////////////////////////////////////////
	if(channel==1){
		long offset=(w*h);

		for(int i=0; i<w*h; i++){
			pdib->m_img.pxs[i].r=buff[w*h-i];
			pdib->m_img.pxs[i].g=buff[w*h-i];
			pdib->m_img.pxs[i].b=buff[w*h-i];
		}
	}

	if(channel>=3){
		long offset=0;

		for(int i=0; i<w*h*channel; i+=channel){
			pdib->m_img.pxs[offset].r=buff[i  ];
			pdib->m_img.pxs[offset].g=buff[i+1];
			pdib->m_img.pxs[offset].b=buff[i+2];
			offset++;
		}
	}

	f.Close();
	delete [] buff;
	buff=NULL;

	return TRUE;
}

BOOL fileRAW::Save(gfx_dc* pdib, CString pathName)
{
	return FALSE;
}