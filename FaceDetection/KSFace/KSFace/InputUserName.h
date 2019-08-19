/////////////////////////////////////////////////////////////////////////////
// InputUserName.h : header file
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_INPUTUSERNAME_H__EFA27288_895D_4EBE_B7CE_B117E7253E11__INCLUDED_)
#define AFX_INPUTUSERNAME_H__EFA27288_895D_4EBE_B7CE_B117E7253E11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// CInputUserName dialog
/////////////////////////////////////////////////////////////////////////////
class CInputUserName : public CDialog
{
// Construction
public:
	CInputUserName(CWnd* pParent = NULL);   // standard constructor
	CString	GetUserName();

// Dialog Data
	//{{AFX_DATA(CInputUserName)
	enum { IDD = IDD_DIALOG_INPUT_USERNAME };
	CString	m_userName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputUserName)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputUserName)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_INPUTUSERNAME_H__EFA27288_895D_4EBE_B7CE_B117E7253E11__INCLUDED_)
