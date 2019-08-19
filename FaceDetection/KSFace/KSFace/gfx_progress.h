/////////////////////////////////////////////////////////////////////////////
// gfx_progress.h
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_GFX_PROGRESS_H__624351AD_902D_4FF4_86BB_841012B019B2__INCLUDED_)
#define AFX_GFX_PROGRESS_H__624351AD_902D_4FF4_86BB_841012B019B2__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
#include "KS_DIB/gfx_dc.h"
/////////////////////////////////////////////////////////////////////////////
typedef struct RGBA_{
	BYTE r;
	BYTE g;
	BYTE b;
	BYTE a;

	void set(BYTE _r, BYTE _g, BYTE _b, BYTE _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	void clear()
	{
		r = 0;
		g = 0;
		b = 0;
		a = 0;
	}
}RGBA;

class gfx_progress: public CProgressCtrl
{
public:
	gfx_progress();

	BOOL  Create(LPCTSTR text, int txtpos, int maxv, int nPane,
				 BYTE fr, BYTE fg, BYTE fb, BYTE fa, 
				 BYTE br, BYTE bg, BYTE bb, BYTE ba,
				 BYTE tr, BYTE tg, BYTE tb, BYTE ta);

	BOOL  Create(LPCTSTR text, int txtpos, int maxv, int nPane,
				 BYTE fr, BYTE fg, BYTE fb, BYTE fa, 
				 BYTE br, BYTE bg, BYTE bb, BYTE ba,
				 BYTE tr, BYTE tg, BYTE tb, BYTE ta, CStatusBar* pStatusBar);
	
	gfx_progress(LPCTSTR text , int txtpos, int maxv, int nPane,
				 BYTE fr, BYTE fg, BYTE fb, BYTE fa, 
				 BYTE br, BYTE bg, BYTE bb, BYTE ba,
				 BYTE tr, BYTE tg, BYTE tb, BYTE ta);

	virtual ~gfx_progress();

	void        init();
	void		run_progress(int _percent);
	void		set_fgcolor(BYTE r, BYTE g, BYTE b, BYTE a);
	void		set_bgcolor(BYTE r, BYTE g, BYTE b, BYTE a);
	void		set_text(LPCTSTR text, int txtpos, BYTE r, BYTE g, BYTE b);

	int         w;
	int         h;

private:
	BOOL		SetRange(int nLower, int nUpper, int nStep=1);
	int			SetPos(int nPos);
	int			SetStep(int nStep);
	int			StepIt();
	CStatusBar*	GetStatusBar();
	BOOL		Resize(CRect rc, CStatusBar* pBar);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(gfx_progress)
	//}}AFX_VIRTUAL

private:
	gfx_dc*		m_canvas;
	RGBA		m_fgColor;
	RGBA		m_bgColor;
	RGBA		m_txColor;
	int			m_txtpos;
	CString		m_text;
	int			m_percent;
	
protected:
	//{{AFX_MSG(gfx_progress)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_GFX_PROGRESS_H__624351AD_902D_4FF4_86BB_841012B019B2__INCLUDED_)
