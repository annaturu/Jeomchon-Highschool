/////////////////////////////////////////////////////////////////////////////
// ResultTable.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "KSFace.h"
#include "ResultTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResultTable dialog
/////////////////////////////////////////////////////////////////////////////
CResultTable::CResultTable(CWnd* pParent /*=NULL*/)
	: CDialog(CResultTable::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResultTable)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_indexInfo.clear();
}


void CResultTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResultTable)
	DDX_Control(pDX, IDC_LIST_RESULT_TABLE, m_ctlListBox);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CResultTable, CDialog)
	//{{AFX_MSG_MAP(CResultTable)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_RESULT_TABLE, OnButtonSaveResultTable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResultTable message handlers
/////////////////////////////////////////////////////////////////////////////
BOOL CResultTable::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	m_ctlListBox.SetExtendedStyle(LVS_EX_FULLROWSELECT|
								  LVS_EX_GRIDLINES|
								  LVS_EX_ONECLICKACTIVATE|
								  LVS_EX_FLATSB);

	LV_COLUMN lv;
	lv.mask=LVCF_FMT|
		    LVCF_SUBITEM|
			LVCF_TEXT|
			LVCF_WIDTH;

	lv.fmt= LVCFMT_CENTER;
	
	lv.cx      = 85;
	lv.iSubItem= 0;
	lv.pszText = "NAME";
	m_ctlListBox.InsertColumn(0, &lv);	

	lv.cx      = 100;
	lv.iSubItem= 1;
	lv.pszText = "Training Image N";
	m_ctlListBox.InsertColumn(1, &lv);	


	lv.cx      = 50;
	lv.iSubItem= 2;
	lv.pszText = "Hit";
	m_ctlListBox.InsertColumn(2, &lv);	

	lv.cx      = 50;
	lv.iSubItem= 3;
	lv.pszText = "Fail";
	m_ctlListBox.InsertColumn(3, &lv);	

	lv.cx      = 50;
	lv.iSubItem= 4;
	lv.pszText = "%";
	m_ctlListBox.InsertColumn(4, &lv);	
	
	return TRUE;
}

void CResultTable::UpdateNode(CString nodeName, int offset)
{
	int i = nodeName.Find("_");
	nodeName = nodeName.Left(i);

	//AfxMessageBox(nodeName);
	
	BOOL bNewNode = TRUE;
	std::vector<INDEX_INFO>::iterator sp=m_indexInfo.begin();
	std::vector<INDEX_INFO>::iterator ep=m_indexInfo.end();
	for(; sp!=ep; sp++){
		/////////////////////////////////////////////////////////////////
		// 만약에 기존에 있는 사람이라면 훈련영상의 카운트를 증가 시키고.
		// 영상의 오브셋 번호를 추가한다.
		/////////////////////////////////////////////////////////////////
		if(sp->name==nodeName){
			sp->trainingN++;
			sp->index_array.push_back(offset);

			bNewNode = FALSE;
		}
	}

	/////////////////////////////////////////////////////////////////
	// 아니라면 새로운 노드 생성..
	/////////////////////////////////////////////////////////////////
	if(bNewNode==TRUE){
		INDEX_INFO n;
		n.name  = nodeName;
		n.trainingN = 1;
		n.index_array.push_back(offset);

		n.failN = 0;
		n.hitN  = 0;
		n.percent= 0.f;

		m_indexInfo.push_back(n);
	}
	
	UpdateListBox();
}

void CResultTable::UpdateListBox()
{
	m_ctlListBox.DeleteAllItems();
	
	std::vector<INDEX_INFO>::iterator sp=m_indexInfo.begin();
	std::vector<INDEX_INFO>::iterator ep=m_indexInfo.end();
	
	int offset=0;

	for(; sp!=ep; sp++){
		LV_ITEM lvitem;
		lvitem.iItem    =offset;
		lvitem.mask     =LVIF_TEXT|LVIF_STATE;
		lvitem.iSubItem =0;
		lvitem.stateMask=LVIS_STATEIMAGEMASK;
		lvitem.state    =INDEXTOSTATEIMAGEMASK(1);
		lvitem.pszText  =(LPSTR)(LPCTSTR)sp->name;
		
		m_ctlListBox.InsertItem(&lvitem);

		CString temp;
		temp.Format("%d", sp->trainingN);
		m_ctlListBox.SetItemText(lvitem.iItem, 1, temp);

		temp.Format("%d", sp->hitN);
		m_ctlListBox.SetItemText(lvitem.iItem, 2, temp);

		temp.Format("%d", sp->failN);
		m_ctlListBox.SetItemText(lvitem.iItem, 3, temp);
		
		if((sp->hitN+sp->failN)==0){
			sp->percent=0.f;
		}
		
		else{
			sp->percent = (float)sp->hitN/(float)(sp->hitN+sp->failN)*100;
		}

		temp.Format("%.1f", sp->percent);
		m_ctlListBox.SetItemText(lvitem.iItem, 4, temp);
		
		offset++;
	}
}

BOOL CResultTable::SetResult(CString name, int offset)
{
	std::vector<INDEX_INFO>::iterator sp=m_indexInfo.begin();
	std::vector<INDEX_INFO>::iterator ep=m_indexInfo.end();
	
	BOOL bfind=FALSE;

	for(; sp!=ep; sp++){
		if(sp->name==name){
			bfind=TRUE;
			break;
		}
	}

	if(bfind==FALSE) return FALSE;

	else{
		BOOL succuss=FALSE;
		int s = sp->index_array.size();
	
		for(int i=0; i<s; i++){
			
			if(sp->index_array[i]==offset){
				succuss=TRUE;
				break;
			}
		}
		
		if(succuss==TRUE )	sp->hitN++;
		if(succuss==FALSE)	sp->failN++;
		UpdateListBox();
	}

	return TRUE;
}

void CResultTable::ClearAll()
{
	m_indexInfo.clear();
	m_ctlListBox.DeleteAllItems();
}

void CResultTable::OnButtonSaveResultTable() 
{
	FILE* fp;

	CString rootPath;
	EXEC_ROOT(rootPath);
	rootPath+="\\resultTable.txt";

	if(!(fp=fopen(rootPath, "a"))){
		AfxMessageBox("resultTable.txt file not exist\n Make empty file [resultTable.txt]");
		return;
	}

	fprintf(fp, "\n");
	fprintf(fp, "################################################################\n");
	fprintf(fp, "#                Face Recognition Result                       #\n");
	fprintf(fp, "################################################################\n");
	fprintf(fp, "NAME\t TRAINING COUNT\t HIT COUNT\t FAIL COUNT\t SUCCESS\n");

	std::vector<INDEX_INFO>::iterator sp=m_indexInfo.begin();
	std::vector<INDEX_INFO>::iterator ep=m_indexInfo.end();

	for(; sp!=ep; sp++){
		fprintf(fp, "%s\t\t", sp->name);
		fprintf(fp, "%d\t\t", sp->trainingN);
		fprintf(fp, "%d\t\t", sp->hitN);
		fprintf(fp, "%d\t", sp->failN);
		fprintf(fp, "%.2f", sp->percent);
		fprintf(fp, "\n");
	}
	fclose(fp);
	AfxMessageBox("Save complete");
}
