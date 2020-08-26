// PL/SQL Developer Plug-In demo
// Copyright 1999 Allround Automations
// support@allroundautomations.nl
// http://www.allroundautomations.nl
// Demo Developed by David Maisonave (david@axter.com)

// DemoMfc.h : main header file for the DEMOMFC DLL
//

#if !defined(AFX_DEMOMFC_H__EAA2C3D3_2953_4B12_AB71_7847900F906F__INCLUDED_)
#define AFX_DEMOMFC_H__EAA2C3D3_2953_4B12_AB71_7847900F906F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

//Functions to expert to PL/SQL Dev
extern "C"
{
  __declspec(dllexport) char* IdentifyPlugIn(int);
  __declspec(dllexport) char* CreateMenuItem(int);
  __declspec(dllexport) void  RegisterCallback(int, void *);
  __declspec(dllexport) void  OnMenuClick(int);
  __declspec(dllexport) void  OnCreate();
  __declspec(dllexport) void  OnActivate();
  __declspec(dllexport) void  OnDestroy();
  __declspec(dllexport) void  OnBrowserChange();
  __declspec(dllexport) void  OnWindowChange();
  __declspec(dllexport) void  OnConnectionChange();
  __declspec(dllexport) int	  OnWindowClose(int WindowType, BOOL Changed);
  __declspec(dllexport) void  OnWindowCreate(int WindowType);
  __declspec(dllexport) void  Configure();
}

/////////////////////////////////////////////////////////////////////////////
// CDemoMfcApp
// See DemoMfc.cpp for the implementation of this class
//

class CDemoMfcApp : public CWinApp
{
public:
	CDemoMfcApp();
	~CDemoMfcApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoMfcApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CDemoMfcApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMOMFC_H__EAA2C3D3_2953_4B12_AB71_7847900F906F__INCLUDED_)
