//////////////////////////////////////////////////////////////////////
// PGM.cpp: implementation of the CPGM class.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KSFace.h"
#include "PGM.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPGM::CPGM()
{
	m_img = NULL;
	m_w   = 0;
	m_h   = 0;
}

CPGM::~CPGM()
{
	if(m_img){
		delete []m_img;
		m_img=NULL;
	}
	m_w = 0;
	m_h = 0;
}

BOOL CPGM::Save(CString pathName)
{
	if(m_img==NULL){
		AfxMessageBox("PGM: data is null");
		return FALSE;
	}

	FILE* fp;
	if(!(fp = fopen(pathName, "wb"))){
		AfxMessageBox ( "PGM: could not write file" );
		return FALSE;
	}
	
	fprintf(fp, "%s\n", "P5");
	fprintf(fp, "%d\n", m_w);
	fprintf(fp, "%d\n", m_h);
	fprintf(fp, "%d\n", 255);
	
	fwrite(m_img, 1, m_w*m_h, fp);
	fclose(fp);
	
	return TRUE;
}

BOOL CPGM::Open(CString pathName)
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

	m_w = w;
	m_h = h;

	if(m_img){
		delete []m_img;
		m_img = NULL;
	}

	m_img = new BYTE[w*h];

	fread(m_img, 1, w*h, fp);
	fclose(fp);

	return TRUE;
}

BOOL CPGM::SetPGM(BYTE* buf, int w, int h)
{
	if(w==0||h==0){
		AfxMessageBox ( "PGM: size is zero" );
		return FALSE;
	}

	if(m_img){
		delete []m_img;
		m_img = NULL;
	}
	m_img = new BYTE[w*h];

	memcpy(m_img, buf, w*h);
	m_w = w;
	m_h = h;

	return TRUE;
}

CSize CPGM::GetSize()
{
	return CSize(m_w, m_h);
}

BYTE* CPGM::GetBuffer()
{
	return m_img;
}