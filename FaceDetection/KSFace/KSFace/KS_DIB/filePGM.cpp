// filePGM.cpp: implementation of the filePGM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "filePGM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

filePGM::filePGM()
{

}

filePGM::~filePGM()
{

}

BOOL filePGM::Open(gfx_dc* pdib, CString pathName)
{
	FILE* fp;
	
	if(!(fp = fopen(pathName, "rb"))){
		AfxMessageBox ( "PGM: could not open file" );
		return FALSE;
	}

	char    ver[20];
	int		depth;
	int		w;
	int		h;

	fscanf(fp, "%s", &ver);
	fscanf(fp, "%d", &w);
	fscanf(fp, "%d", &h);
	fscanf(fp, "%d", &depth);

	BYTE* buffer;
	buffer = new BYTE[w*h];

	fread(buffer, 1, w*h, fp);

	if(!pdib->create(w, h)){
		AfxMessageBox("PGM: could not create dib");
		return FALSE;
	}

	for(int y=0; y<h; y++){
		for(int x=0; x<w; x++){
				
			pdib->m_img.pxs[y*w+x].r=buffer[y*w+x];
			pdib->m_img.pxs[y*w+x].g=buffer[y*w+x];
			pdib->m_img.pxs[y*w+x].b=buffer[y*w+x];
		}
	}
	fclose(fp);

	delete []buffer;
	buffer = NULL;
	return TRUE;
}

BOOL filePGM::Save(gfx_dc* pdib, CString pathName)
{
	FILE* fp;
	
	if(!(fp = fopen(pathName, "wb"))){
		AfxMessageBox ( "PGM: could not write file" );
		return FALSE;
	}

	fprintf(fp, "%s\n", "P5");
	fprintf(fp, "%d\n", pdib->m_w);
	fprintf(fp, "%d\n", pdib->m_h);
	fprintf(fp, "%d\n", 255);

	BYTE* buffer;
	buffer = new BYTE[pdib->m_w * pdib->m_h];

	for(int y=0; y<pdib->m_h; y++){
		for(int x=0; x<pdib->m_w; x++){
				
			buffer[y*pdib->m_w+x]= (BYTE)(pdib->m_img.pxs[y*pdib->m_w+x].r+
								          pdib->m_img.pxs[y*pdib->m_w+x].g+
								          pdib->m_img.pxs[y*pdib->m_w+x].b)/3.f;
		}
	}

	fwrite(buffer, 1, pdib->m_w * pdib->m_h, fp);

	delete []buffer;
	buffer = NULL;
	fclose(fp);

	return TRUE;
}