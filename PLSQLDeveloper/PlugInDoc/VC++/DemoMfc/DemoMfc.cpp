// PL/SQL Developer Plug-In demo
// Copyright 1999 Allround Automations
// support@allroundautomations.nl
// http://www.allroundautomations.nl
// Demo Developed by David Maisonave (david@axter.com)

// This demo shows an elementary interface

// DemoMfc.cpp : Defines the initialization routines for the DLL.

// To debug the demo, change the project property settings in the LINK tab
// Change the [Output File Name] field from
// Debug/DemoMfc.dll
// To the following:
// C:\Program Files\PLSQL Developer\PlugIns/DemoMfc.dll
// Change the above directory to reflect the correct location of the PLSQL Developer directory.
// The above change will make the compiler compile the code to the PlugIns directory

// The project property settings in the debug tab should also be changed to reflect correct PLSQL Dev directory
// Check the following field: [Project]->[Settings]->[Debug](tab)->[Executable For Debug Session](field)

#include "stdafx.h"
#include "DemoMfc.h"
#include "PlSql_functions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDemoMfcApp

BEGIN_MESSAGE_MAP(CDemoMfcApp, CWinApp)
	//{{AFX_MSG_MAP(CDemoMfcApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

bool UseAutoSetCursorPosition = false;
/////////////////////////////////////////////////////////////////////////////
// CDemoMfcApp construction

CDemoMfcApp::CDemoMfcApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}
CDemoMfcApp::~CDemoMfcApp()
{
	WriteProfileInt("CursorPosOption","UseAutoSetCursorPosition",(UseAutoSetCursorPosition)?1:0);
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CDemoMfcApp object

CDemoMfcApp theApp;



/////////////////////////////////////////////////////////////////////////////
// CDemoMfcApp initialization

BOOL CDemoMfcApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

    UseAutoSetCursorPosition = (GetProfileInt("CursorPosOption","UseAutoSetCursorPosition",0))?true:false;
	return TRUE;
}

const char* IntToStr(int n)
{
	static CString s;
	s.Format("%i", n);
	return s;
}

void ShowMessage(const char* Data)
{
	MessageBox(NULL,Data, "Msg from AddIn",0);
}
char* MenuItemInterface(int Index, bool CreateMenuItem_);


//---------------------------------------------------------------------------
// Creating menus
char* CreateMenuItem(int Index)
{
	return MenuItemInterface(Index, true);
}

//---------------------------------------------------------------------------
// Show configuration dialog
void  Configure()
{
	ShowMessage("No configue menu");
}

//---------------------------------------------------------------------------
// Plug-In identification, a unique identifier is received and
// the description is returned
char* IdentifyPlugIn(int ID)
{
	PlugInID = ID;
	return Desc;
}
//---------------------------------------------------------------------------
void OnMenuClick(int Index)
{
	MenuItemInterface(Index, false);
}
//---------------------------------------------------------------------------
// Called when someone logs on or off
void OnConnectionChange()
{
}
//---------------------------------------------------------------------------
// Called when another item in the browser gets selected
void OnBrowserChange()
{
  char *Type, *Owner, *Name;

  IDE_GetBrowserInfo(&Type, &Owner, &Name);
}
//---------------------------------------------------------------------------
// Called when the Plug-In is created
void OnCreate()
{ 
}
//---------------------------------------------------------------------------
// Called when the Plug-In is activated
void OnActivate()
{
  OnBrowserChange();
  OnConnectionChange();
  OnWindowChange();
}
//---------------------------------------------------------------------------
// Called when the Plug-In is destroyed
void OnDestroy()
{
}
//---------------------------------------------------------------------------
void DoSQLForm()
{
}

//---------------------------------------------------------------------------
void ShowBrowserObject()
{
	char *Type, *Owner, *Name;
	char s[100];
	
	IDE_GetBrowserInfo(&Type, &Owner, &Name);
	s[0] = 0;
	strcat(s, Type);
	strcat(s, "\n");
	strcat(s, Owner);
	strcat(s, "\n");
	strcat(s, Name);
	ShowMessage(s);
}
//---------------------------------------------------------------------------
void ShowConnectionInfo()
{
	char *Username, *Password, *Database;
	char s[100];
	
	IDE_GetConnectionInfo(&Username, &Password, &Database);
	s[0] = 0;
	strcat(s, Username);
	strcat(s, "\n");
	strcat(s, Password);
	strcat(s, "\n");
	strcat(s, Database);
	ShowMessage(s);
}
//---------------------------------------------------------------------------
void SetSelection(char *s)
{
	int H = IDE_GetEditorHandle();
	if (H > 0)
	{
		SendMessage((HWND)H, EM_REPLACESEL, true, (int)s);
	}
}
//---------------------------------------------------------------------------
void MaximizeWindow()
{
	int H = IDE_GetChildHandle();
	int M = IDE_GetClientHandle();
	if (H > 0)
	{
		SendMessage((HWND)M, WM_MDIMAXIMIZE, H, (int)0);
	}
}
void SaveAndExecute()
{
	IDE_SaveFile();
	IDE_Perform(pcmd_Execute);
}

DWORD GetCurrentCusorPosition(int &x, int &y)
{
	HWND H = (HWND)IDE_GetEditorHandle();
	DWORD StartingPos;
	SendMessage(H, EM_GETSEL, (WPARAM) (LPDWORD) &StartingPos, NULL);
	DWORD StartingPos_x = StartingPos;
	y = SendMessage(H, EM_LINEFROMCHAR, (WPARAM) StartingPos, NULL);
	x = StartingPos - SendMessage(H, EM_LINEINDEX, y, NULL);
	return StartingPos_x;
}

int OnWindowClose(int WindowType, BOOL Changed)
{
	int x,y;
	DWORD StartingPos = GetCurrentCusorPosition(x,y);
	CString FileName = IDE_Filename();
	theApp.WriteProfileInt("CursorPos",FileName,y);
	return 0;
}

bool WindowJustOpened = false;

void OnWindowCreate(int WindowType)
{
	WindowJustOpened = true;
}
void OnWindowChange()
{
	if (UseAutoSetCursorPosition && WindowJustOpened)
	{
		WindowJustOpened = false;
		CString FileName = IDE_Filename();
		DWORD LastPos = theApp.GetProfileInt("CursorPos",FileName,1);
		HWND H = (HWND)IDE_GetEditorHandle();
		DWORD p = SendMessage(H, EM_LINEINDEX, LastPos, NULL);
		SendMessage(H, EM_SETSEL, p, p);
	}
}

void DisplayCurrentCusorLineNumber(void)
{
	int x,y;
	DWORD StartingPos = GetCurrentCusorPosition(x,y);
	CString Msg;
	Msg.Format("Current line number = %i %i %i", x, y, StartingPos);
	ShowMessage(Msg);
}

char* MenuItemInterface(int Index, bool CreateMenuItem_)
{
	switch (Index)
	{
    case 1 : 
		if (CreateMenuItem_) return "Session / Save and Execute";
		SaveAndExecute();
		return NULL;
    case 2 : 
		if (CreateMenuItem_) return "Edit / Test display Cursor position";
		DisplayCurrentCusorLineNumber();
		return NULL;
    case 3 :
		if (CreateMenuItem_) return "Tools / &Plug-In 2 Demo / &Maximize Window";
		MaximizeWindow();
		return NULL;
    case 4 :
		if (CreateMenuItem_) return "Tools / &Plug-In 2 Demo / &ReadOnly / &On";
		IDE_SetReadOnly(true);
		return NULL;
    case 5 :
		if (CreateMenuItem_) return "Tools / &Plug-In 2 Demo / &ReadOnly / O&ff";
		IDE_SetReadOnly(false);
		return NULL;
    case 6 :
		if (CreateMenuItem_) return "Tools / &Plug-In 2 Demo / &Editor/ ShowHandle...";
		ShowMessage(IntToStr(IDE_GetEditorHandle()));
		return NULL;
    case 7 :
		if (CreateMenuItem_) return "Tools / &Plug-In 2 Demo / &Editor/ SetSelection";
		SetSelection("New selected text");
		return NULL;
    case 8 :
		if (CreateMenuItem_) return "Tools / &Plug-In 2 Demo / &Auto Cursor / &On";
		UseAutoSetCursorPosition = true;
		return NULL;
    case 9 :
		if (CreateMenuItem_) return "Tools / &Plug-In 2 Demo / &Auto Cursor / O&ff";
		UseAutoSetCursorPosition = false;
		return NULL;
	}
	return "";
}
