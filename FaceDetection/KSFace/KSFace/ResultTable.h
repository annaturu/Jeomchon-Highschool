/////////////////////////////////////////////////////////////////////////////
// ResultTable.h : header file
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_RESULTTABLE_H__48D9CB8F_639D_49FA_9D69_1D847BACA6B5__INCLUDED_)
#define AFX_RESULTTABLE_H__48D9CB8F_639D_49FA_9D69_1D847BACA6B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// CResultTable dialog
/////////////////////////////////////////////////////////////////////////////
#include  <vector>
#include  <algorithm>
///////////////////////////////////////////////////////////////////////
//* allocate std templite.
using namespace std;
//////////////////////////////////////////////////////////////////////

typedef struct _INDEX_INFO{
	CString name;
	int		trainingN;
	int		hitN;
	int		failN;
	float	percent;
	std::vector<int>	index_array;
}INDEX_INFO;

class CResultTable : public CDialog
{
public:
	CResultTable(CWnd* pParent = NULL);   // standard constructor
	void	UpdateNode(CString nodeName, int offset);
	BOOL	SetResult(CString name, int offset);
	void	ClearAll();

private:
	void	UpdateListBox();

	std::vector<INDEX_INFO>	m_indexInfo;

public:
	//{{AFX_DATA(CResultTable)
	enum { IDD = IDD_DIALOG_RESULT_TABLE };
	CListCtrl	m_ctlListBox;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResultTable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CResultTable)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSaveResultTable();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_RESULTTABLE_H__48D9CB8F_639D_49FA_9D69_1D847BACA6B5__INCLUDED_)
