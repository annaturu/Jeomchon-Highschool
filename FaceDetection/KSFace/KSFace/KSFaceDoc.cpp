// KSFaceDoc.cpp : implementation of the CKSFaceDoc class
//

#include "stdafx.h"
#include "KSFace.h"

#include "KSFaceDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKSFaceDoc

IMPLEMENT_DYNCREATE(CKSFaceDoc, CDocument)

BEGIN_MESSAGE_MAP(CKSFaceDoc, CDocument)
	//{{AFX_MSG_MAP(CKSFaceDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKSFaceDoc construction/destruction

CKSFaceDoc::CKSFaceDoc()
{
	// TODO: add one-time construction code here

}

CKSFaceDoc::~CKSFaceDoc()
{
}

BOOL CKSFaceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CKSFaceDoc serialization

void CKSFaceDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CKSFaceDoc diagnostics

#ifdef _DEBUG
void CKSFaceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CKSFaceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CKSFaceDoc commands
