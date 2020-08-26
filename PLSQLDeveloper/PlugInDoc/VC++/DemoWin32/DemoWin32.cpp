// PL/SQL Developer Plug-In demo
// Copyright 1999 Allround Automations
// support@allroundautomations.nl
// http://www.allroundautomations.nl
// Demo Developed by David Maisonave (david@axter.com)

// This demo shows an elementary interface

// To debug the demo, change the project property settings in the LINK tab
// Change the [Output File Name] field from
// Debug/DemoWin32.dll
// To the following:
// C:\Program Files\PLSQL Developer\PlugIns/DemoWin32.dll
// Change the above directory to reflect the correct location of the PLSQL Developer directory.
// The above change will make the compiler compile the code to the PlugIns directory

// The project property settings in the debug tab should also be changed to reflect correct PLSQL Dev directory
// Check the following field: [Project]->[Settings]->[Debug](tab)->[Executable For Debug Session](field)

// DemoWin32.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "DemoWin32.h"
#include "PlSql_functions.h"
#include <string>
#include <sstream>

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

bool UseAutoSetCursorPosition = false;

const char* IntToStr(int n)
{
	static std::string s;
	std::stringstream ss;
	ss << n;
	s = ss.str();
	return s.c_str();
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
	char Buff[32]="";
	GetProfileString("CursorPosOption","UseAutoSetCursorPosition","0",Buff,sizeof(Buff));
	UseAutoSetCursorPosition = (atoi(Buff))?true:false;
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
	std::string FileName = IDE_Filename();
	WriteProfileString("CursorPos",FileName.c_str(),IntToStr(y));
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
		std::string FileName = IDE_Filename();
		char Buff[32]="";
		GetProfileString("CursorPos",FileName.c_str(),"1",Buff,sizeof(Buff));
		DWORD LastPos = atoi(Buff);
		HWND H = (HWND)IDE_GetEditorHandle();
		DWORD p = SendMessage(H, EM_LINEINDEX, LastPos, NULL);
		SendMessage(H, EM_SETSEL, p, p);
	}
}

void DisplayCurrentCusorLineNumber(void)
{
	int x,y;
	DWORD StartingPos = GetCurrentCusorPosition(x,y);
	std::stringstream ss;
	ss << "Current line number = " << x << " " <<  y << " " << StartingPos ;
	ShowMessage(ss.str().c_str());
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
		WriteProfileString("CursorPosOption","UseAutoSetCursorPosition",IntToStr(UseAutoSetCursorPosition));
		return NULL;
    case 9 :
		if (CreateMenuItem_) return "Tools / &Plug-In 2 Demo / &Auto Cursor / O&ff";
		UseAutoSetCursorPosition = false;
		WriteProfileString("CursorPosOption","UseAutoSetCursorPosition",IntToStr(UseAutoSetCursorPosition));
		return NULL;
	}
	return "";
}
