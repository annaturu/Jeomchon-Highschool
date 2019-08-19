//////////////////////////////////////////////////////////////////////
// fileTGA.cpp: implementation of the fileTGA class.
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "fileTGA.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define TGA_Null	0
#define TGA_Map		1
#define TGA_RGB		2

typedef struct _TGAHEADER{
	BYTE IdLength;
	BYTE CmapType;
	BYTE ImageType;

	WORD CmapIndex;
	WORD CmapLength;
	BYTE CmapEntrySize;

	WORD X_Origin;
	WORD Y_Origin;
	WORD ImageWidth;
	WORD ImageHeight;
	BYTE PixelDepth;
	BYTE ImageDesc;
}TGAHEADER;

fileTGA::fileTGA()
{

}

fileTGA::~fileTGA()
{

}

BOOL fileTGA::Open(gfx_dc* pdib, CString pathName)
{
	CFile f;
	if(!f.Open(pathName, CFile::modeRead)){
		AfxMessageBox("Targa: could not open file");
		return FALSE;
	}

	TGAHEADER htga;
	ZeroMemory(&htga, sizeof(TGAHEADER));

	f.Read(&htga.IdLength, 1);
	f.Read(&htga.CmapType, 1);
	f.Read(&htga.ImageType, 1);
	f.Read(&htga.CmapIndex, 2);
	f.Read(&htga.CmapLength, 2);
	f.Read(&htga.CmapEntrySize, 1);
	f.Read(&htga.X_Origin, 2);
	f.Read(&htga.Y_Origin, 2);
	f.Read(&htga.ImageWidth, 2);
	f.Read(&htga.ImageHeight, 2);
	f.Read(&htga.PixelDepth, 1);
	f.Read(&htga.ImageDesc, 1);

	if(htga.ImageType!=TGA_RGB){
		AfxMessageBox("Taga: image type not yet supported");
		return FALSE;
	}

	int w=htga.ImageWidth;
	int h=htga.ImageHeight;

	if(!pdib->create(w, h)){
		AfxMessageBox("Taga: could not create dib");
		return FALSE;
	}

	if(htga.PixelDepth==COLOR_32){
	
		f.Read(pdib->get_lppixels(), w*h*4);
	}

	if(htga.PixelDepth==COLOR_24){
		BYTE* src= new BYTE[w*h*3];
		if(src==NULL){
			pdib->destroy();
			AfxMessageBox("Taga: not enough memory");
			return FALSE;
		}
		f.Read(src, w*h*3);
		
		for(long i=0; i<w*h; i++){
			pdib->m_img.pxs[i].r=src[i*3  ];
			pdib->m_img.pxs[i].g=src[i*3+1];
			pdib->m_img.pxs[i].b=src[i*3+2];
		}
		delete [] src;
	}

	else{
		AfxMessageBox("Taga: bit depth not yet supported");
		return FALSE;
	}
	return TRUE;
}

BOOL fileTGA::Save(gfx_dc* pdib, CString pathName)
{
	return FALSE;
}
