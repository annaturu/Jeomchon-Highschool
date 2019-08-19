/////////////////////////////////////////////////////////////////////////////
// KSFaceView.h : interface of the CKSFaceView class
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_KSFACEVIEW_H__9536BE84_16A7_473E_BD91_35031B5C7CC2__INCLUDED_)
#define AFX_KSFACEVIEW_H__9536BE84_16A7_473E_BD91_35031B5C7CC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
#include <cv.h>
#include <highgui.h>
#include <cvcam.h>

#include "Canvas.h"
#include "EigenFace/EGFace.h"
#include "PGM.h"
#include "Graph.h"
#include "InputUserName.h"
#include "ResultTable.h"
/////////////////////////////////////////////////////////////////////////////
class CKSFaceView : public CView
{
protected: // create from serialization only
	CKSFaceView();
	DECLARE_DYNCREATE(CKSFaceView)

private:
	CResultTable*	m_resultTable;
	CGraph*			m_pEuclidean;
	CEGFace			m_egFace;
	CCanvas*		m_pCanvas;
	IplImage*		m_pImage;
	CvCapture*		m_pCapture; //카메라 컨트롤 그리고 디바이스 제어 
	CvMemStorage*			 m_storage;
	CvHaarClassifierCascade* m_cascade;
	
	int			m_frameOffset;
	double		m_avMin;
	double		m_avMax;
	BOOL		m_busy;
	CString		m_userName;

public:
	CKSFaceDoc* GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKSFaceView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKSFaceView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CKSFaceView)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDestroy();
	afx_msg void OnReloadEigenface();
	afx_msg void OnStartRecognition();
	afx_msg void OnStopRecognition();
	afx_msg void OnPaint();
	afx_msg void OnStartNewUserCapture();
	afx_msg void OnOpenResultTable();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in KSFaceView.cpp
inline CKSFaceDoc* CKSFaceView::GetDocument()
   { return (CKSFaceDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KSFACEVIEW_H__9536BE84_16A7_473E_BD91_35031B5C7CC2__INCLUDED_)
