// PL/SQL Developer Plug-In demo
// Copyright 1999 Allround Automations
// support@allroundautomations.nl
// http://www.allroundautomations.nl

// This demo shows an elementary interface

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
    return 1;
}
//---------------------------------------------------------------------------
// The three basic export functions
extern "C"
{
  __declspec(dllexport) char* IdentifyPlugIn(int);
  __declspec(dllexport) char* CreateMenuItem(int);
  __declspec(dllexport) void  OnMenuClick(int);
}

// Description of this Plug-In (as displayed in Plug-In configuration dialog)
char *const Desc = "C++Builder Plug-In demo 1";
int PlugInID;


//---------------------------------------------------------------------------
// Plug-In identification, a unique identifier is received and
// the description is returned
char* IdentifyPlugIn(int ID)
{
  PlugInID = ID;
  return Desc;
}
//---------------------------------------------------------------------------
// Creating a menu item
char* CreateMenuItem(int Index)
{
  switch (Index)
  {
    case 1 : return "Tools / &Plug-In 1 Demo...";
  }
  return "";
}
//---------------------------------------------------------------------------
// The menu item got selected
void OnMenuClick(int Index)
{
  switch (Index)
  {
    case 1 :
      ShowMessage("Hello");
      break;
  }
}
//---------------------------------------------------------------------------
