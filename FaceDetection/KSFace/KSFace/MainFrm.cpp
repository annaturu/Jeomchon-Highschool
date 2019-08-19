// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "KSFace.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP

	ON_MESSAGE(UM_PROGRESS, OnProgress)
	ON_MESSAGE(UM_CLEARPGS,OnClearProgress)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_PROGRESS,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	/*
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	*/

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);

	LONG style = ::GetWindowLong(m_hWnd, GWL_STYLE);
	style &= ~FWS_ADDTOTITLE;
	style &= ~WS_VISIBLE;
	style &= ~WS_POPUP;
	 
	::SetWindowLong(m_hWnd, GWL_STYLE, style);
	SetWindowPos(   &wndTop,
					GetSystemMetrics(SM_CXSCREEN)/2 - (DEFAULT_CAMW),
					GetSystemMetrics(SM_CYSCREEN)/2 - (DEFAULT_CAMH/2)-115,
					(DEFAULT_CAMW+50)*2,
					DEFAULT_CAMH+130,
					SWP_SHOWWINDOW );
	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
LRESULT CMainFrame::OnProgress(WPARAM wparam, LPARAM lparam)
{
	int percent = (float)lparam/(float)wparam*100.f;
	m_progres_bar.run_progress(percent);

	
	return NULL;
}

LRESULT CMainFrame::OnClearProgress(WPARAM wparam, LPARAM lparam)
{
	int w=m_progres_bar.w;
	
	m_progres_bar.set_text("Comeng Vision Lab", 
		                   int(w/2.f), 
						   255, 
						   255, 
						   255);
	
	m_progres_bar.Invalidate(true);
	return NULL;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	if(m_wndStatusBar){
		int status0_w = (int)(cx/100.f*37.f);
		int status1_w = (int)(cx/100.f*40.f);

		m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, ID_SEPARATOR, status0_w);
		m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_PROGRESS, ID_INDICATOR_PROGRESS, status1_w);
	
		m_progres_bar.Create("Comeng Vision Lab", 
			                 100,
			                 100, 1,
							 64, 64, 64,  255, 
							 155, 155, 155, 255, 
							 255, 255, 255, 255);
	}
}
