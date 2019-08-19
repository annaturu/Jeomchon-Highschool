/////////////////////////////////////////////////////////////////////////////
// MainFrm.h : interface of the CMainFrame class
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_MAINFRM_H__08B65E78_8C7F_4AEB_91B4_0180BB494E79__INCLUDED_)
#define AFX_MAINFRM_H__08B65E78_8C7F_4AEB_91B4_0180BB494E79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
#include "gfx_progress.h"
/////////////////////////////////////////////////////////////////////////////
class CMainFrame : public CFrameWnd
{

public:
	afx_msg LRESULT OnProgress(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnClearProgress(WPARAM wparam, LPARAM lparam);

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

private:
	gfx_progress	m_progres_bar;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_MAINFRM_H__08B65E78_8C7F_4AEB_91B4_0180BB494E79__INCLUDED_)
