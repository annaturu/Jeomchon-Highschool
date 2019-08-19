/////////////////////////////////////////////////////////////////////////////
// Graph.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "KSFace.h"
#include "Graph.h"

/////////////////////////////////////////////////////////////////////////////
// CGraph
/////////////////////////////////////////////////////////////////////////////
CGraph::CGraph(int _w, int _h, CWnd* pParent)
{
	m_pDC= new gfx_dc(_w, _h);
	
	CreateEx(NULL, 
	         NULL, 
		     NULL, 
			 WS_CHILD|
			 WS_VISIBLE,
			 CRect(0, 0, _w, _h),
			 pParent, 
			 100, 
			 NULL);
	
	m_w=_w;
	m_h=_h;

	m_minV = 0;
	m_maxV = 0;
	m_dataNum=0;
	m_data = NULL;	
}

CGraph::~CGraph()
{
	if(m_pDC){
		delete m_pDC;
		m_pDC=NULL;
	}

	if(m_data){
		delete []m_data;
		m_data=NULL;
	}
}

BEGIN_MESSAGE_MAP(CGraph, CStatic)
	//{{AFX_MSG_MAP(CGraph)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CGraph message handlers
/////////////////////////////////////////////////////////////////////////////
void CGraph::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	m_pDC->clear(255, 255, 255, 255);

	m_pDC->rect(15, 0, m_w-16, m_h-30, 0, 0, 0, 255);
	m_pDC->fill_rect(16, 1, m_w-17, m_h-31, 230, 230, 200);
	m_pDC->Text("Tahoma", &dc, "-face offset-", (m_w/2)-20, m_h-20, 0, 0, 0, 12, false, 0);
	
	int offset=0;

	if(m_data){
		int interval = (int)((m_w-5)/(m_dataNum-1));

		for(int i=0; i<m_dataNum; i++){
			int x = 20+(i*interval);
			int v = (int)((m_maxV-m_data[i])/(m_maxV-m_minV)*(m_h-40));

			m_pDC->line(x, m_h-30, x, v, 155, 155, 155, 255);
			m_pDC->line(x, m_h-30, x, m_h-28, 80, 80, 80, 255);

			m_pDC->fill_rect(x-3, v-3, 6, 6, 155, 155, 155);


			if(i%5==0){
				CString temp;
				temp.Format("%d", i);
				m_pDC->Text("Tahoma", &dc, temp, x+2, m_h-30, 0, 0, 0, 12, false, 0);

				m_pDC->line(x, m_h-30, x, m_h-25, 80, 80, 80, 255);
			}
			
			if(m_minV==m_data[i]){
				offset = i;
			}
		}
	}

	m_pDC->line(15, m_h-30, 12, m_h-30, 80, 80, 80, 255);
	m_pDC->line(15, 0, 12, 0, 80, 80, 80, 255);
	m_pDC->Text("Tahoma", &dc, "min", 1, m_h-33, 0, 0, 0, 12, false, 900);
	m_pDC->Text("Tahoma", &dc, "max", 1, 20, 0, 0, 0, 12, false, 900);
	m_pDC->Text("Tahoma", &dc, "-euclidean dis-", 1, 130, 0, 0, 0, 12, false, 900);

	CString msg;
	msg.Format("Max : %.0lf, Min : %.0lf  Recognition Face: %d", m_minV, m_maxV, offset);
	m_pDC->Text("Tahoma", &dc, msg, 15, m_h-10, 200, 30, 30, 12, false, 0);

	m_pDC->update(&dc);
}

void CGraph::SetData(int num, double minV, double maxV, double* pData)
{
	m_minV = minV;
	m_maxV = maxV;
	m_dataNum=num;
	
	if(m_data){
		delete []m_data;
		m_data=NULL;
	}

	m_data = new double[num];
	memcpy(m_data, 
		   pData, 
		   sizeof(double)*num);

	Invalidate(TRUE);
}
