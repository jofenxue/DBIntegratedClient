// PL/SQL Developer Plug-In demo
// Copyright 1999 Allround Automations
// support@allroundautomations.nl
// http://www.allroundautomations.nl
// Demo Developed by David Maisonave (david@axter.com)

#if !defined(DEMOWIN32_H_INCLUDED_)
#define DEMOWIN32_H_INCLUDED_

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



#endif // !defined(DEMOWIN32_H_INCLUDED_)
