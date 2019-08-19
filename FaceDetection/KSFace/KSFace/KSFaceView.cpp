/////////////////////////////////////////////////////////////////////////////
// KSFaceView.cpp : implementation of the CKSFaceView class
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "KSFace.h"

#include "KSFaceDoc.h"
#include "KSFaceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKSFaceView
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CKSFaceView, CView)

BEGIN_MESSAGE_MAP(CKSFaceView, CView)
	//{{AFX_MSG_MAP(CKSFaceView)
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_DESTROY()
	ON_COMMAND(ID_RELOAD_EIGENFACE, OnReloadEigenface)
	ON_COMMAND(ID_START_RECOGNITION, OnStartRecognition)
	ON_COMMAND(ID_STOP_RECOGNITION, OnStopRecognition)
	ON_WM_PAINT()
	ON_COMMAND(ID_START_NEW_USER_CAPTURE, OnStartNewUserCapture)
	ON_COMMAND(ID_OPEN_RESULT_TABLE, OnOpenResultTable)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CKSFaceView construction/destruction
/////////////////////////////////////////////////////////////////////////////

CKSFaceView::CKSFaceView()
{
	m_pCanvas=NULL;

	m_pImage  =NULL;
    m_pCapture=NULL;
	m_storage =NULL;
	m_cascade =NULL;
	m_pEuclidean =NULL;
	m_resultTable=NULL;
	m_busy=FALSE;
}

CKSFaceView::~CKSFaceView()
{
	if(m_pEuclidean){
		delete m_pEuclidean;
		m_pEuclidean=NULL;
	}

	if(m_pCanvas){
		delete m_pCanvas;
		m_pCanvas=NULL;
	}

	if(m_pCapture){
		cvReleaseCapture(&m_pCapture);
		m_pCapture=NULL;
	}

	if(m_resultTable){
		delete m_resultTable;
		m_resultTable=NULL;
	}

	cvReleaseHaarClassifierCascade(&m_cascade);
	cvReleaseMemStorage(&m_storage);
}

BOOL CKSFaceView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CKSFaceView drawing

void CKSFaceView::OnDraw(CDC* pDC)
{
	CKSFaceDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CKSFaceView diagnostics
/////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CKSFaceView::AssertValid() const
{
	CView::AssertValid();
}

void CKSFaceView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKSFaceDoc* CKSFaceView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKSFaceDoc)));
	return (CKSFaceDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CKSFaceView message handlers
/////////////////////////////////////////////////////////////////////////////

void CKSFaceView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

	///////////////////////////////////////////////////////
	// canvas static box set size. and set postion.
	///////////////////////////////////////////////////////
	m_pCanvas=new CCanvas(DEFAULT_CAMW,   // width
						  DEFAULT_CAMH,   // height,
						  this);

	///////////////////////////////////////////////////////
	// canvas static box set size. and set postion.
	///////////////////////////////////////////////////////
	m_pCanvas->MoveWindow(20, 
						  40, 
						  DEFAULT_CAMW, 
						  DEFAULT_CAMH, 
						  TRUE);

	m_pEuclidean = new CGraph(DEFAULT_GRAPHW,
		                      DEFAULT_GRAPHH,
							  this);

	m_pEuclidean->MoveWindow(DEFAULT_CAMW+60, 
						     40, 
						     DEFAULT_GRAPHW, 
						     DEFAULT_GRAPHH, 
						     TRUE);

	CvSize size;
	size.width = DEFAULT_CAMW;
	size.height= DEFAULT_CAMH;

	///////////////////////////////////////////////////////
	// default camera num 0
	///////////////////////////////////////////////////////
	m_pCapture=cvCaptureFromCAM(0);       // 카메라로부터 들어오는 부분
	m_pImage  =cvCreateImage(size, 8, 3); // Zigbee로부터 들어 오는 데이터를 받는 부분으로 활요하는 부분임

	m_storage= cvCreateMemStorage(0);
	m_cascade= (CvHaarClassifierCascade*)cvLoad("haarcascade_frontalface_alt.xml");
	
	if(m_cascade==NULL){
		AfxMessageBox("Can not Load cascade");
	}
	
	///////////////////////////////////////////////////////////
	// 결과값을 보여 주는 위도우 생성..
	///////////////////////////////////////////////////////////
	m_resultTable = new CResultTable(this);
	m_resultTable->Create(IDD_DIALOG_RESULT_TABLE, this);
	m_resultTable->ShowWindow(SW_HIDE);

	SetTimer(ID_CAMERA_TRIGGER, 0, NULL);
}

void CKSFaceView::OnOpenResultTable() 
{
	if(m_resultTable){
		m_resultTable->ShowWindow(SW_SHOW);
	}
}


void CKSFaceView::OnTimer(UINT nIDEvent) 
{//현재 최고 속도로 되어 있음 - 초당 30 프레임이 들어오도록 되어 있음
	if(nIDEvent==ID_CAMERA_TRIGGER){
		if(m_pCapture){
			
			//
			//
			//

			cvGrabFrame(m_pCapture);
			m_pImage=cvRetrieveFrame(m_pCapture);

			// Changing Color to Gray Level Image
			IplImage* fcopy;
			fcopy=cvCreateImage(cvSize(m_pImage->width, m_pImage->height),
								IPL_DEPTH_8U, 
								m_pImage->nChannels);

			cvFlip(m_pImage, fcopy); // 상하로 뒤집힌 이미지를 정상적으로 되돌려 주는 작업
			m_pCanvas->SetImage(fcopy->width,
								fcopy->height,
								(BYTE*)fcopy->imageData,
								 "RGB color space");
			
			
			if(m_cascade){
				CvSeq* faces = cvHaarDetectObjects(fcopy, 
												   m_cascade, 
												   m_storage,
												   1.2, 2, 
												   CV_HAAR_DO_CANNY_PRUNING,
												   cvSize(40, 40));
				
				//////////////////////////////////////////////////////
				// 다수의 얼굴을 잡을때... 
				//for(int i=0; i<(faces?faces->total:0); i++){
				//////////////////////////////////////////////////////
				if(faces->total>0){
					CvRect* r = (CvRect*)cvGetSeqElem(faces, 0);
					
					m_pCanvas->SetFacePosition(r->x,
											   r->y, 
											   r->width, 
											   r->height,
											   0);
				}
			}
			cvReleaseImage(&fcopy);
			m_pCanvas->Invalidate(TRUE);
		}
	}

	if(nIDEvent==ID_RECOGNITION_TRIGGER){
		CSize s=m_pCanvas->GetFaceReginSize();

		BYTE* temp;
		temp = new BYTE[s.cx*s.cy];

		CString msg;

		if((m_pCanvas->GetFaceReginData(temp, s.cx, s.cy))==FALSE){
			msg.Format("Get Face Regin Data : False");
			m_pCanvas->SetMsgInfo(msg);

			delete []temp;
			temp=NULL;
			return;
		}

		if((m_egFace.InCommingInputImage(temp, s.cx, s.cy))==FALSE){
			msg.Format("Training face set not loaded");
			m_pCanvas->SetMsgInfo(msg);

			delete []temp;
			temp=NULL;
			return;
		}
		delete []temp;
		temp=NULL;

		/////////////////////////////////////////////////
		// Get euclidean matrix data of input face.
		/////////////////////////////////////////////////
		CSize ms = m_egFace.m_euclidean.Size();

		double* mtData;
		mtData = new double[ms.cx*ms.cy];

		m_egFace.m_euclidean.GetMatrix(mtData);
		m_pEuclidean->SetData(ms.cx, 
			                  m_egFace.m_distMin, 
							  m_egFace.m_distMax, 
							  mtData);

		/////////////////////////////////////////////////////
		// Training image 들로 부터 가장 가까운 거리를 찾음.
		/////////////////////////////////////////////////////
		int offset=0;

		for(int i=0; i<ms.cx; i++){
			if(m_egFace.m_distMin==mtData[i]){
				offset=i;
			}
		}

		///////////////////////////////////////////////////////////
		// 현재 설정된 사용자의 이름에 대한 인식 성공 여부를 기록.
		///////////////////////////////////////////////////////////
		m_resultTable->SetResult(m_userName, offset);


		BYTE* tImage;
		tImage = new BYTE[s.cx*s.cy];

		m_egFace.GetTrainingImg(tImage, offset);
		m_pCanvas->SetSubWindow(tImage);

		delete []tImage;
		tImage=NULL;

		delete []mtData;
		mtData=NULL;
	}

	CView::OnTimer(nIDEvent);
}

void CKSFaceView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	///////////////////////////////////////////////////////////
	// capture face.
	///////////////////////////////////////////////////////////
	if(nChar==CAPTURE_FACE){
		if(m_userName.IsEmpty()){
			AfxMessageBox("Training User Name is empty.");
			return;
		}

		CSize s=m_pCanvas->GetFaceReginSize();

		BYTE* temp;
		temp = new BYTE[s.cx*s.cy];

		if((m_pCanvas->GetFaceReginData(temp, s.cx, s.cy))==FALSE){
			delete []temp;
			temp=NULL;

			return;
		}

		//////////////////////////////////////////////////////////////////
		// get new file pathname.
		//////////////////////////////////////////////////////////////////
		CString rootPath;
		EXEC_ROOT(rootPath);
		rootPath+="\\Face Source\\";
		
		CFileFind find;
		BOOL      bfind;
		CString   pathName;
		CString   nextFname;

		if(find.FindFile(rootPath+m_userName+"_0.pgm")==FALSE){
			pathName=rootPath+m_userName+"_0.pgm";
		}
		
		else{
			int offset=1;
			while(bfind){
				nextFname.Format("_%d.pgm", offset);

				bfind=find.FindFile(rootPath+m_userName+nextFname);
				offset++;
			}
			pathName=rootPath+m_userName+nextFname;
		}

		//////////////////////////////////////////////////////////////////
		// save pgm file to gray color.
		//////////////////////////////////////////////////////////////////
		CPGM pgm;
		pgm.SetPGM(temp, s.cx, s.cy);
		pgm.Save(pathName);

		delete []temp;
		temp=NULL;

		CString msg;
		msg.Format("Captured face Source - ");
		m_pCanvas->SetMsgInfo(msg+nextFname);
		
		m_pCanvas->Invalidate(TRUE);
	}
	
	///////////////////////////////////////////////////////////
	// 인식 시작.
	///////////////////////////////////////////////////////////
	if(nChar==VK_SPACE){
		if(m_busy==FALSE){
			OnStartRecognition();
		}
		
		else OnStopRecognition();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CKSFaceView::OnDestroy() 
{
	CView::OnDestroy();

	KillTimer(ID_CAMERA_TRIGGER);
	KillTimer(ID_RECOGNITION_TRIGGER);
}

void CKSFaceView::OnReloadEigenface() 
{
	//////////////////////////////////////////////////////////////////
	// get new file pathname.
	//////////////////////////////////////////////////////////////////
	m_egFace.ResetAll();

	CString rootPath;
	EXEC_ROOT(rootPath);
	rootPath+="\\Face Source\\";

	CFileFind find;
	BOOL      bfind=TRUE;
	int		  offset=0;

	///////////////////////////////////////////////
	// 현재 폴더에 트레이닝 영상이 있는지 확인하고..
	///////////////////////////////////////////////
	bfind=find.FindFile(rootPath+"*.pgm");
	
	if(bfind==FALSE) return;

	///////////////////////////////////////////////
	// 있다면 하나씩 트레이닝 버퍼에 저장.
	///////////////////////////////////////////////
	m_resultTable->ClearAll();

	while(bfind){
		bfind=find.FindNextFile();
		if(find.IsDots()) continue;
		if(find.IsDirectory()) continue;

		else{
			CPGM pgm;
			pgm.Open(find.GetFilePath());

			CSize s = pgm.GetSize();
			m_egFace.PushTrainingImg(pgm.GetBuffer(),
				                     s.cx, 
									 s.cy);

			////////////////////////////////////////////////////////////////////
			// 리스트 박스의 노드를 추가 한다. 만약 이미 존재 하는 사람이라면..
			// 트레이닝 이미지 카운트를 증가 시킨다.
			////////////////////////////////////////////////////////////////////
			m_resultTable->UpdateNode(find.GetFileTitle(),
				                      offset);
			offset++;
		}
	}

	m_egFace.CalculateEigenFace();
	
	CString msg;
	msg.Format("Face Source training complete : %d", offset);
	m_pCanvas->SetMsgInfo(msg);
}

void CKSFaceView::OnStartRecognition() 
{
	SetTimer(ID_RECOGNITION_TRIGGER, 0, NULL);

	m_pCanvas->SetMsgInfo("Recognition Start - System busy");
	m_busy=TRUE;
}

void CKSFaceView::OnStopRecognition() 
{
	KillTimer(ID_RECOGNITION_TRIGGER);

	m_pCanvas->SetMsgInfo("Recognition Stop - System idle");
	m_busy=FALSE;
}

void CKSFaceView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	HFONT font;
	HFONT oldfont;

	font=CreateFont(12, 0, 0, 0, 0, 0, 0, 0, 
		            HANGEUL_CHARSET, 
					0, 0, 0, 0, 
					"Tahoma");

	oldfont = (HFONT)SelectObject(dc.m_hDC, font);

	dc.TextOut(20, 25, ">>Face Recognition");
	dc.TextOut(DEFAULT_CAMW+70, 25, ">>Euclidean Distance");

	dc.TextOut(DEFAULT_CAMW-115, 25, "Training User Name :"+m_userName);
	SelectObject(dc.m_hDC, oldfont);
}

void CKSFaceView::OnStartNewUserCapture() 
{
	CInputUserName dlg;
	
	if(dlg.DoModal()==IDOK){
		m_userName=dlg.GetUserName();
		Invalidate(TRUE);
	}
}

