// KSFaceDoc.h : interface of the CKSFaceDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_KSFACEDOC_H__0EBDEEA8_8EEF_4226_91FC_8E47535899B8__INCLUDED_)
#define AFX_KSFACEDOC_H__0EBDEEA8_8EEF_4226_91FC_8E47535899B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CKSFaceDoc : public CDocument
{
protected: // create from serialization only
	CKSFaceDoc();
	DECLARE_DYNCREATE(CKSFaceDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKSFaceDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKSFaceDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CKSFaceDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KSFACEDOC_H__0EBDEEA8_8EEF_4226_91FC_8E47535899B8__INCLUDED_)
