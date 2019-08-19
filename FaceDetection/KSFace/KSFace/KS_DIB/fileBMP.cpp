//////////////////////////////////////////////////////////////////////
// fileBMP.cpp: implementation of the fileBMP class.
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "fileBMP.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define DIB_HEADER_MAKER ((WORD)('M'<<8)|'B')

fileBMP::fileBMP()
{

}

fileBMP::~fileBMP()
{

}

BOOL fileBMP::Open(gfx_dc* pdib, CString pathName)
{
	CFile f;
	BITMAPFILEHEADER bh;
	BITMAPINFOHEADER bi;
	DWORD	len;

	if(!f.Open(pathName, CFile::modeRead, NULL)){
		AfxMessageBox("Bmp: can not open file");
		return FALSE;
	}

	len=f.GetLength();

	if(f.Read((LPSTR)&bh, sizeof(bh))!=sizeof(bh)){
		AfxMessageBox("Bmp: file size error!");

		f.Close();
		return FALSE;
	}

	if(bh.bfType!=DIB_HEADER_MAKER){
		AfxMessageBox("Bmp: file maker error!");

		f.Close();
		return FALSE;
	}

	if(f.Read((LPSTR)&bi, sizeof(bi))!=sizeof(bi)){
		AfxMessageBox("Bmp: file info error!");
		f.Close();
		return FALSE;
	}

	if(bi.biBitCount!=COLOR_256&&
		bi.biBitCount!=COLOR_24){
		return FALSE;
	}

	int	size_bf=sizeof(BITMAPFILEHEADER);
	int	size_bi=sizeof(BITMAPINFOHEADER);

	if(bi.biBitCount==COLOR_256){
		int w=((bi.biWidth+3)/4)*4;
		int h=bi.biHeight;

		int ps=len-((sizeof(bh)+sizeof(bi))+(w*bi.biHeight));
		BYTE* palette=new BYTE[ps];
		BYTE* data   =new BYTE[w*h];

		f.Read((LPSTR)palette, ps);
		f.Read((LPSTR)data   , w*h);

		if(!pdib->create(bi.biWidth, bi.biHeight)){
			AfxMessageBox("Bmp: could not create dib");
			return FALSE;
		}

		long i=0;
		for(int y=0; y<bi.biHeight; y++){
			for(int x=0; x<bi.biWidth; x++){
				
				pdib->m_img.pxs[y*bi.biWidth+x].r=palette[data[i]*4+0];
				pdib->m_img.pxs[y*bi.biWidth+x].g=palette[data[i]*4+1];
				pdib->m_img.pxs[y*bi.biWidth+x].b=palette[data[i]*4+2];
				i++;
			}
			i+=w-bi.biWidth;
		}
	}

	if(bi.biBitCount==COLOR_24){
		int w=((bi.biWidth+3)/4)*4;
		int h=bi.biHeight;

		BYTE* buffer;
		buffer = new BYTE[w*h*3];
		
		f.Read(buffer, w*h*3);
		
		if(!pdib->create(bi.biWidth, bi.biHeight)){
			AfxMessageBox("Bmp: could not create dib");
			return FALSE;
		}
		
		BYTE* ptr=(BYTE*)buffer;

		for(int y=0; y<bi.biHeight; y++){
			for(int x=0; x<bi.biWidth; x++){

				pdib->m_img.pxs[y*bi.biWidth+x].r=(BYTE)ptr[0];
				pdib->m_img.pxs[y*bi.biWidth+x].g=(BYTE)ptr[1];
				pdib->m_img.pxs[y*bi.biWidth+x].b=(BYTE)ptr[2];
				
				ptr+=3;
			}
			ptr+=bi.biWidth%4;
		}
		delete []buffer;
		buffer =NULL;
	}
	f.Close();
	return TRUE;
}

BOOL fileBMP::Save(gfx_dc* pdib, CString pathName)
{
	return FALSE;
}