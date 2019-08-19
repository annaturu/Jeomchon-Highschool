/////////////////////////////////////////////////////////////////////////////
// Canvas.h : header file
/////////////////////////////////////////////////////////////////////////////
#ifndef __CANVAS_H__
#define __CANVAS_H__
/////////////////////////////////////////////////////////////////////////////
// CCanvas window
/////////////////////////////////////////////////////////////////////////////
#include "KS_DIB/gfx_dc.h"

#define DEFAULT_FACE_W 70
#define DEFAULT_FACE_H 80
/////////////////////////////////////////////////////////////////////////////
class CCanvas : public CStatic
{
// Construction
public:
	CCanvas(int _w, int _h, CWnd* pParent);
	virtual ~CCanvas();

	void	SetSubWindow(BYTE* pData);
	void	SetMsgInfo(CString msg);
	BOOL	SetImage(int _w, int _h, BYTE* pImg, CString msg);
	void	SetFacePosition(int x, int y, int w, int h, int offset);
	CSize	GetFaceReginSize();
	BOOL	GetFaceReginData(BYTE* buff, int w, int h);

private:
	gfx_dc*		m_subWindow;
	gfx_dc*		m_pDC;
	int			m_w;
	int			m_h;
	CString		m_overayMsg;
	CString		m_msgInfo;
	CPoint		m_facePos;
	CPoint		m_curfacePos;
	
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCanvas)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCanvas)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_CANVAS_H__021887C2_508B_4949_8399_AA17B4B0A376__INCLUDED_)
