/////////////////////////////////////////////////////////////////////////////
// gfx_progress.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "gfx_progress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL PeekAndPump();

/////////////////////////////////////////////////////////////////////////////
// gfx_progress
/////////////////////////////////////////////////////////////////////////////
gfx_progress::gfx_progress()
{
	m_canvas   = NULL;	
	init();
}

gfx_progress::gfx_progress(LPCTSTR text, int txtpos, int maxv, int nPane,
						   BYTE fr, BYTE fg, BYTE fb, BYTE fa, 
						   BYTE br, BYTE bg, BYTE bb, BYTE ba,
						   BYTE tr, BYTE tg, BYTE tb, BYTE ta)
{
	m_canvas   = NULL;
	
	init();

	Create(text, txtpos, maxv, nPane, 
		   fr, fg, fb, fa, 
	       br, bg, bb, ba,
		   tr, tg, tb, ta);
}

void gfx_progress::init()
{
	w = 0;
	h = 0;
	
	m_txtpos =0;
	m_percent=0;

	m_text  ="";
    m_fgColor.clear();
	m_bgColor.clear();
	m_txColor.clear();
}

gfx_progress::~gfx_progress()
{
	if(m_canvas){
		delete m_canvas;
		m_canvas = NULL;
	}
}

BEGIN_MESSAGE_MAP(gfx_progress, CProgressCtrl)
	//{{AFX_MSG_MAP(gfx_progress)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////
// gfx_progress message handlers
/////////////////////////////////////////////////////////////
void gfx_progress::set_fgcolor(BYTE r, BYTE g, BYTE b, BYTE a)
{
	m_fgColor.set(r, g, b, a);
}

void gfx_progress::set_bgcolor(BYTE r, BYTE g, BYTE b, BYTE a)
{
	m_bgColor.set(r, g, b, a);
}

BOOL gfx_progress::Create(LPCTSTR text, int txtpos,int maxv, int nPane,
						   BYTE fr, BYTE fg, BYTE fb, BYTE fa, 
						   BYTE br, BYTE bg, BYTE bb, BYTE ba,
						   BYTE tr, BYTE tg, BYTE tb, BYTE ta)
{
	BOOL bSuccess = FALSE;

	CStatusBar *pStatusBar = GetStatusBar();
	if (!pStatusBar)
		return FALSE;

	DWORD dwStyle = WS_CHILD|WS_VISIBLE|PBS_SMOOTH;

	CRect rc;
	pStatusBar->GetItemRect(nPane, &rc);

	if (!IsWindow(GetSafeHwnd())){
		bSuccess = CProgressCtrl::Create(dwStyle, rc, pStatusBar, NULL);
		ASSERT(bSuccess);
		if (!bSuccess)	return FALSE;
	}
	
	SetRange(0, maxv);
	SetStep(1);

	w = rc.Width();
	h = rc.Height();
	
	m_text   = text;
	m_txtpos = txtpos;

	m_fgColor.set(fr, fg, fb, fa);
	m_bgColor.set(br, bg, bb, ba);
	m_txColor.set(tr, tg, tb, ta);
	
	Resize(rc, pStatusBar);
	Invalidate(true);
	return TRUE;
}


BOOL gfx_progress::Create(LPCTSTR text, int txtpos,int maxv, int nPane,
						   BYTE fr, BYTE fg, BYTE fb, BYTE fa, 
						   BYTE br, BYTE bg, BYTE bb, BYTE ba,
						   BYTE tr, BYTE tg, BYTE tb, BYTE ta, CStatusBar* pStatusBar)
{
	BOOL bSuccess = FALSE;

	if (!pStatusBar)
		return FALSE;

	DWORD dwStyle = WS_CHILD|WS_VISIBLE|PBS_SMOOTH;

	CRect rc;
	pStatusBar->GetItemRect(nPane, &rc);

	if (!IsWindow(GetSafeHwnd())){
		bSuccess = CProgressCtrl::Create(dwStyle, rc, pStatusBar, NULL);
		ASSERT(bSuccess);
		if (!bSuccess)	return FALSE;
	}
	
	SetRange(0, maxv);
	SetStep(1);

	w = rc.Width();
	h = rc.Height();
	
	m_text   = text;
	m_txtpos = txtpos;

	m_fgColor.set(fr, fg, fb, fa);
	m_bgColor.set(br, bg, bb, ba);
	m_txColor.set(tr, tg, tb, ta);
	
	Resize(rc, pStatusBar);
	Invalidate(true);
	return TRUE;
}

void gfx_progress::set_text(LPCTSTR text, int txtpos, BYTE r, BYTE g, BYTE b)
{ 
	m_text   = text; 
	m_txtpos = txtpos;

	m_txColor.r = r;
	m_txColor.g = g;
	m_txColor.b = b;
	
	m_canvas->clear(m_bgColor.r, 
		            m_bgColor.g, 
					m_bgColor.b, 
					255);
	m_percent=0;
}

BOOL gfx_progress::Resize(CRect rc, CStatusBar* pBar) 
{
	if (!IsWindow(GetSafeHwnd()))
		return FALSE;

	CStatusBar *pStatusBar = pBar;
	if (!pStatusBar)
		return FALSE;

	MoveWindow(&rc);

	if(w<1||h<1) return false;

	else{
		if(m_canvas){
			delete m_canvas;
			m_canvas=NULL;
		}
		
		m_canvas = new gfx_dc(w, h);

		m_canvas->clear( m_bgColor.r, 
			             m_bgColor.g, 
						 m_bgColor.b, 
						 m_bgColor.a);
	}
	return TRUE;
}

void gfx_progress::OnPaint() 
{
	CPaintDC dc(this);

	if(m_canvas){
		float curPos = ((float)m_percent/100.f)*w;

		m_canvas->clear( m_bgColor.r, 
		                 m_bgColor.g, 
					     m_bgColor.b, 
					     m_bgColor.a);

		if(curPos){
			m_canvas->fill_glass_rect( 0, 0,
									 (int)curPos, h-3,
									 m_fgColor.r, 
									 m_fgColor.g, 
									 m_fgColor.b, 
									 50);
		}

		m_canvas->Text("Tahoma", 
			           &dc, 
					   m_text, 
					   m_txtpos+1, 1, 
					   255, 255, 255, 
					   12,
					   false, 0);
	}
	m_canvas->update(&dc);
}

void gfx_progress::run_progress(int _percent)
{
	m_percent=_percent;

	m_text.Format("%d", m_percent);
	m_text+="%";

	PeekAndPump();
	StepIt();
}

BOOL PeekAndPump()
{
	static MSG msg;

	while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)) {
		if (!AfxGetApp()->PumpMessage()) {
			::PostQuitMessage(0);
			return FALSE;
		}	
	}
	return TRUE;
}

CStatusBar* gfx_progress::GetStatusBar()
{
	CWnd *pMainWnd = AfxGetMainWnd();
	if(!pMainWnd)	 return NULL;
	
    if(pMainWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd))){
        CWnd* pMessageBar=((CFrameWnd*)pMainWnd)->GetMessageBar();
        return DYNAMIC_DOWNCAST(CStatusBar, pMessageBar);
    }
    
    else{
	    return DYNAMIC_DOWNCAST(CStatusBar, pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR));
	}
}

BOOL gfx_progress::SetRange(int nLower, int nUpper, int nStep /* = 1 */)	
{	 
	if (!IsWindow(GetSafeHwnd()))
		return FALSE;

	#ifdef PBM_SETRANGE32
		ASSERT(-0x7FFFFFFF <= nLower && nLower <= 0x7FFFFFFF);
		ASSERT(-0x7FFFFFFF <= nUpper && nUpper <= 0x7FFFFFFF);
		SendMessage(PBM_SETRANGE32, (WPARAM) nLower, (LPARAM) nUpper);

	#else
		ASSERT(0 <= nLower && nLower <= 1165535);
		ASSERT(0 <= nUpper && nUpper <= 1165535);
		CProgressCtrl::SetRange(nLower, nUpper);
	#endif

	CProgressCtrl::SetStep(nStep);
	return TRUE;
}

int gfx_progress::SetPos(int nPos)	   
{
	if (!IsWindow(GetSafeHwnd()))
		return 0;

	#ifdef PBM_SETRANGE32
		ASSERT(-0x7FFFFFFF <= nPos && nPos <= 0x7FFFFFFF);
	
	#else
		ASSERT(0 <= nPos && nPos <= 65535);
	#endif

	ModifyStyle(0,WS_VISIBLE);
	return CProgressCtrl::SetPos(nPos);
}

int gfx_progress::SetStep(int nStep)
{ 
	if (!IsWindow(GetSafeHwnd()))
		return 0;

	ModifyStyle(0,WS_VISIBLE);
	return CProgressCtrl::SetStep(nStep);	 
}

int gfx_progress::StepIt() 			
{ 
	if (!IsWindow(GetSafeHwnd()))
		return 0;

	ModifyStyle(0,WS_VISIBLE);
	return CProgressCtrl::StepIt();    
}

BOOL gfx_progress::OnEraseBkgnd(CDC* pDC) 
{
	return true;
}
