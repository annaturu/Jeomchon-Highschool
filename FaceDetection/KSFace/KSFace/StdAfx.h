// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__FDB67317_4036_40EE_A421_87E9C4EFDD46__INCLUDED_)
#define AFX_STDAFX_H__FDB67317_4036_40EE_A421_87E9C4EFDD46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#define DEFAULT_CAMW	320
#define DEFAULT_CAMH	240
#define DEFAULT_GRAPHW  320
#define DEFAULT_GRAPHH  240

#define MAXIMUM_FACE	 10
#define CAPTURE_FACE	'C'

#define UM_PROGRESS        (WM_USER+1)
#define UM_CLEARPGS        (WM_USER+2)

#define UPDATE_PROGRESS(et, st)\
{\
	long nev=(long)((float)et/100.f);\
	if(nev==0) nev=1;\
		if(st%nev==0){\
			CFrameWnd *pFrame = (CFrameWnd*)AfxGetMainWnd();\
			::SendMessage(pFrame->m_hWnd, UM_PROGRESS, et, st);\
		}\
}

#define CLEAR_PROGRESS()\
{\
	CFrameWnd *pFrame = (CFrameWnd*)AfxGetMainWnd();\
	::SendMessage(pFrame->m_hWnd, UM_CLEARPGS, NULL, NULL);\
}

#define EXEC_ROOT(path)\
{\
	char szAppPath[1024];\
	UINT i, j = 0;\
	GetModuleFileName(AfxGetInstanceHandle(), szAppPath, 1024);\
	for(i=0;i<strlen(szAppPath);i++){\
        if(szAppPath[i]=='\\')\
		j=i;\
    }\
    if(j>0) szAppPath[j]='\0';\
	path.Format("%s", szAppPath);\
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__FDB67317_4036_40EE_A421_87E9C4EFDD46__INCLUDED_)
