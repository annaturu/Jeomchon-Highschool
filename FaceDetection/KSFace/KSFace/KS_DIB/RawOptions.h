/////////////////////////////////////////////////////////////////////////////
// RawOptions.h : header file
/////////////////////////////////////////////////////////////////////////////
#ifndef __RAW_OPTIONS_H__
#define __RAW_OPTIONS_H__
/////////////////////////////////////////////////////////////////////////////
#include "resource.h"  //내가 추가.
/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CRawOptions : public CDialog
{
// Construction
public:
	CRawOptions(CWnd* pParent = NULL);

	void	SetData(DWORD size, CString fname);
	void	GetData(int* _w, 
		            int* _h, 
					int* _channel, 
					DWORD* _header);

// Dialog Data
	//{{AFX_DATA(CRawOptions)
	enum { IDD = IDD_DIALOG_OPEN_RAW };
	int		m_intH;
	int		m_intW;
	int		m_intHS;
	int		m_intCH;
	CString	m_strNA;
	CString	m_strSZ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRawOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRawOptions)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif
