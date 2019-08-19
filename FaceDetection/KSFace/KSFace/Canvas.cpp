/////////////////////////////////////////////////////////////////////////////
// Canvas.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "KSFace.h"
#include "Canvas.h"

/////////////////////////////////////////////////////////////////////////////
// CCanvas
/////////////////////////////////////////////////////////////////////////////
CCanvas::CCanvas(int _w, int _h, CWnd* pParent)
{
	m_subWindow = new gfx_dc(DEFAULT_FACE_W, 
		                     DEFAULT_FACE_H);

	m_subWindow->rect(0, 0, 
		              DEFAULT_FACE_W-1, 
		              DEFAULT_FACE_H-1,
					  190, 190, 190, 255);
	
	m_pDC= new gfx_dc(_w, _h);
	
	CreateEx(NULL, 
	         NULL, 
		     NULL, 
			 WS_CHILD|
			 WS_VISIBLE,
			 CRect(0, 0, _w, _h),
			 pParent, 
			 100, 
			 NULL);
	
	m_w=_w;
	m_h=_h;

	m_curfacePos=CPoint(0, 0);
}

CCanvas::~CCanvas()
{
	if(m_subWindow){
		delete m_subWindow;
		m_subWindow=NULL;
	}

	if(m_pDC){
		delete m_pDC;
		m_pDC=NULL;
	}
}

BEGIN_MESSAGE_MAP(CCanvas, CStatic)
	//{{AFX_MSG_MAP(CCanvas)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCanvas message handlers
/////////////////////////////////////////////////////////////////////////////
void CCanvas::SetMsgInfo(CString msg)
{
	m_msgInfo=msg;
	Invalidate(TRUE);
}

BOOL CCanvas::SetImage(int _w, int _h, BYTE* pImg, CString msg)
{
	if(m_w!=_w||m_h!=_h) return FALSE;
		
	m_pDC->copyFrom_image(pImg, _w, _h);
	m_overayMsg=(LPCSTR)msg;

	m_facePos=CPoint(0, 0);
	Invalidate(TRUE);
	return TRUE;
}

void CCanvas::SetFacePosition(int x, int y, int w, int h, int offset)
{
	m_facePos.x=x+(int)(w*0.5f);
	m_facePos.y=y+(int)(h*0.5f);
}

void CCanvas::OnPaint() 
{
	CPaintDC dc(this);
	
	BOOL bfindFace=FALSE;

	m_pDC->Text("Tahoma", &dc, "Capture face : 'C' Key", 5, 5, 0, 0, 0, 12, false, 0);
	m_pDC->Text("Tahoma", &dc, "Start Recognition : 'Space Bar' Key", 5, 15, 0, 0, 0, 12, false, 0);
	m_pDC->Text("Tahoma", &dc, "Stop Recognition : 'Space Bar' Key", 5, 25, 0, 0, 0, 12, false, 0);

	if(m_facePos!=CPoint(0,0)){
		m_pDC->rect(m_facePos.x-(DEFAULT_FACE_W*.5f), 
			        m_facePos.y-(DEFAULT_FACE_H*.5f), 
					DEFAULT_FACE_W,
					DEFAULT_FACE_H,
				    169, 169, 169, 255);

		m_curfacePos = m_facePos;
		bfindFace=TRUE;

		m_pDC->copy_from_this_rect(m_subWindow, 
			                       245, 
								   5);

		m_pDC->Text("Tahoma", &dc, ">>Found face", 245, DEFAULT_FACE_H+5, 180, 180, 180, 12, false, 0);
	}

	if(bfindFace==FALSE){
		m_curfacePos=CPoint(0, 0);
	}

	m_pDC->fill_glassRect_emboss(2, m_h-18, m_w-5, 15, 
		                         255, 255, 255, 
								 100);

	m_pDC->Text("Tahoma", &dc, "Console : ", 7, m_h-16, 255, 0, 0, 12, true, 0);
	m_pDC->Text("Tahoma", &dc, m_msgInfo, 55, m_h-16, 0, 0, 0, 12, false, 0);

	m_pDC->rect(0, 0, m_w-1, m_h-1, 0, 0, 0, 255);
	
	m_pDC->update(&dc);
}

///////////////////////////////////////////////////////
// subWindow size fixed.
///////////////////////////////////////////////////////
void CCanvas::SetSubWindow(BYTE* pData)
{
	m_subWindow->copyFrom_grayImage(pData, 
		                            DEFAULT_FACE_W, 
		                            DEFAULT_FACE_H);

	m_subWindow->rect(0, 0, 
		              DEFAULT_FACE_W-1, 
		              DEFAULT_FACE_H-1,
					  190, 190, 190, 255);
}


CSize CCanvas::GetFaceReginSize()
{
	return CSize(DEFAULT_FACE_W, 
		         DEFAULT_FACE_H);
}

BOOL CCanvas::GetFaceReginData(BYTE* buff, int w, int h)
{
	if(m_curfacePos==CPoint(0, 0)){
		return FALSE;
	}

	m_pDC->copy_to_image_rect_gray(buff, 
		                           m_curfacePos.x-(int)(w*0.5f),
							       m_curfacePos.y-(int)(h*0.5f),
								   w, h);

	for(int i=0; i<180; i++){
		gfx_dc* tempDC;
		tempDC = new gfx_dc(m_w, m_h);

		tempDC->copy_from_this(m_pDC);
		tempDC->glassRect_Target(m_curfacePos.x-(int)(w*0.5f),
			                    m_curfacePos.y-(int)(h*0.5f),
					            w, h,
					            255, 255, 255, 180-i);

		CDC* pDC = GetDC();
		tempDC->update(pDC);
		
		delete tempDC;
		tempDC=NULL;
		ReleaseDC(pDC);
	}
	
	return TRUE;
}