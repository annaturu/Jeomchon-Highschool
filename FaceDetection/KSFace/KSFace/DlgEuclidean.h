#if !defined(AFX_DLGEUCLIDEAN_H__4808345A_D976_456E_81F7_A33577E6F31A__INCLUDED_)
#define AFX_DLGEUCLIDEAN_H__4808345A_D976_456E_81F7_A33577E6F31A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEuclidean.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgEuclidean dialog

class CDlgEuclidean : public CDialog
{
// Construction
public:
	CDlgEuclidean(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgEuclidean)
	enum { IDD = IDD_DIALOG_EUCLIDEAN_GRAPH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEuclidean)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEuclidean)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEUCLIDEAN_H__4808345A_D976_456E_81F7_A33577E6F31A__INCLUDED_)
