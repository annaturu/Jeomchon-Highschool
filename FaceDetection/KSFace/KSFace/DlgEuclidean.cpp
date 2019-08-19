// DlgEuclidean.cpp : implementation file
//

#include "stdafx.h"
#include "KSFace.h"
#include "DlgEuclidean.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEuclidean dialog


CDlgEuclidean::CDlgEuclidean(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEuclidean::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEuclidean)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgEuclidean::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEuclidean)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEuclidean, CDialog)
	//{{AFX_MSG_MAP(CDlgEuclidean)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEuclidean message handlers
