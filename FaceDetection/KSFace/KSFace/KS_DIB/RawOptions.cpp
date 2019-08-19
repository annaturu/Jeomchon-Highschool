/////////////////////////////////////////////////////////////////////////////
// RawOptions.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
//add additional includes here"
#include "RawOptions.h"

/////////////////////////////////////////////////////////////////////////////
// CRawOptions dialog
/////////////////////////////////////////////////////////////////////////////
CRawOptions::CRawOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CRawOptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRawOptions)
	m_intH = 0;
	m_intW = 0;
	m_intHS = 0;
	m_intCH = 0;
	m_strNA = _T("");
	m_strSZ = _T("");
	//}}AFX_DATA_INIT
}


void CRawOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRawOptions)
	DDX_Text(pDX, IDC_EDIT_RAW_H, m_intH);
	DDX_Text(pDX, IDC_EDIT_RAW_W, m_intW);
	DDX_Text(pDX, IDC_EDIT_RAW_HEADER, m_intHS);
	DDX_Text(pDX, IDC_EDIT_RAW_CHANNEL, m_intCH);
	DDX_Text(pDX, IDC_STATIC_RAW_NAME, m_strNA);
	DDX_Text(pDX, IDC_STATIC_RAW_SIZE, m_strSZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRawOptions, CDialog)
	//{{AFX_MSG_MAP(CRawOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRawOptions message handlers
/////////////////////////////////////////////////////////////////////////////
void CRawOptions::SetData(DWORD size, CString fname)
{
	m_strNA=fname;
	m_strSZ.Format("%ld", size);
}

void CRawOptions::GetData(int* _w, int* _h, int* _channel, DWORD* _header)
{
	*_w=m_intW;
	*_h=m_intH;
	*_channel=m_intCH;
	*_header =m_intHS;
}

BOOL CRawOptions::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_intCH=1;
	
	DWORD size = atoi(m_strSZ);

	if((640*480  )==size) {m_intW=640; m_intH=480;}
	if((512*512  )==size) {m_intW=512; m_intH=512;}
	if((256*256  )==size) {m_intW=256; m_intH=256;}
	if((1024*768 )==size) {m_intW=1024; m_intH=768;}
	if((1280*1024)==size) {m_intW=1280; m_intH=1024;}
	
	UpdateData(FALSE);
	return TRUE;
}
