; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CKSFaceView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "KSFace.h"
LastPage=0

ClassCount=10
Class1=CKSFaceApp
Class2=CKSFaceDoc
Class3=CKSFaceView
Class4=CMainFrame

ResourceCount=4
Resource1=IDD_ABOUTBOX
Class5=CAboutDlg
Class6=CCanvas
Resource2=IDD_DIALOG_INPUT_USERNAME
Class7=CDlgEuclidean
Class8=CGraph
Class9=CInputUserName
Resource3=IDR_MAINFRAME
Class10=CResultTable
Resource4=IDD_DIALOG_RESULT_TABLE

[CLS:CKSFaceApp]
Type=0
HeaderFile=KSFace.h
ImplementationFile=KSFace.cpp
Filter=N

[CLS:CKSFaceDoc]
Type=0
HeaderFile=KSFaceDoc.h
ImplementationFile=KSFaceDoc.cpp
Filter=N

[CLS:CKSFaceView]
Type=0
HeaderFile=KSFaceView.h
ImplementationFile=KSFaceView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=ID_OPEN_RESULT_TABLE


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=CMainFrame
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=KSFace.cpp
ImplementationFile=KSFace.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_START_NEW_USER_CAPTURE
Command11=ID_RELOAD_EIGENFACE
Command12=ID_START_RECOGNITION
Command13=ID_STOP_RECOGNITION
Command14=ID_OPEN_RESULT_TABLE
Command15=ID_APP_ABOUT
CommandCount=15

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[CLS:CCanvas]
Type=0
HeaderFile=Canvas.h
ImplementationFile=Canvas.cpp
BaseClass=CStatic
Filter=W
VirtualFilter=WC
LastObject=CCanvas

[CLS:CDlgEuclidean]
Type=0
HeaderFile=DlgEuclidean.h
ImplementationFile=DlgEuclidean.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgEuclidean

[CLS:CGraph]
Type=0
HeaderFile=Graph.h
ImplementationFile=Graph.cpp
BaseClass=CStatic
Filter=W
VirtualFilter=WC
LastObject=CGraph

[DLG:IDD_DIALOG_INPUT_USERNAME]
Type=1
Class=CInputUserName
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_EDIT_USER_NAME,edit,1342242817
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,button,1342177287
Control7=IDC_STATIC,static,1342308352

[CLS:CInputUserName]
Type=0
HeaderFile=InputUserName.h
ImplementationFile=InputUserName.cpp
BaseClass=CDialog
Filter=D
LastObject=CInputUserName
VirtualFilter=dWC

[DLG:IDD_DIALOG_RESULT_TABLE]
Type=1
Class=CResultTable
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDC_LIST_RESULT_TABLE,SysListView32,1350631425
Control3=IDC_STATIC,button,1342210055
Control4=IDC_BUTTON_SAVE_RESULT_TABLE,button,1342242816

[CLS:CResultTable]
Type=0
HeaderFile=ResultTable.h
ImplementationFile=ResultTable.cpp
BaseClass=CDialog
Filter=D
LastObject=CResultTable
VirtualFilter=dWC

