/////////////////////////////////////////////////////////////////////////////
// InputUserName.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "KSFace.h"
#include "InputUserName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputUserName dialog
/////////////////////////////////////////////////////////////////////////////
CInputUserName::CInputUserName(CWnd* pParent /*=NULL*/)
	: CDialog(CInputUserName::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputUserName)
	m_userName = _T("");
	//}}AFX_DATA_INIT
}


void CInputUserName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputUserName)
	DDX_Text(pDX, IDC_EDIT_USER_NAME, m_userName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputUserName, CDialog)
	//{{AFX_MSG_MAP(CInputUserName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputUserName message handlers
/////////////////////////////////////////////////////////////////////////////
BOOL CInputUserName::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowPos(&wndTopMost,
				 0, 0, 0, 0,
				 SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);
	
	return TRUE;
}

void CInputUserName::OnOK() 
{
	UpdateData(TRUE);	
	CDialog::OnOK();
}

CString CInputUserName::GetUserName()
{
	return m_userName;
}
