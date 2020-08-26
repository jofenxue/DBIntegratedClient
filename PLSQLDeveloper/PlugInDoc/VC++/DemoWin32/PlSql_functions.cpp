// PL/SQL Developer Plug-In demo
// Copyright 1999 Allround Automations
// support@allroundautomations.nl
// http://www.allroundautomations.nl
// Demo Developed by David Maisonave (david@axter.com)

// Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PlSql_functions.h"
#include "DemoWin32.h"
#include <string>
#include <sstream>


char *const Desc = "PL SQL MFC Demo";
int PlugInID;

// Declaration of PL/SQL Developer callback functions
int   (*funcptr_SYS_Version)() = NULL;
char* (*funcptr_SYS_Registry)() = NULL;
char* (*funcptr_SYS_RootDir)() = NULL;
char* (*funcptr_SYS_OracleHome)() = NULL;
void  (*funcptr_IDE_MenuState)(int ID, int Index, BOOL Enabled) = NULL;
BOOL  (*funcptr_IDE_Connected)() = NULL;
void  (*funcptr_IDE_GetConnectionInfo)(char **Username, char **Password, char **Database) = NULL;
void  (*funcptr_IDE_GetBrowserInfo)(char **ObjectType, char **ObjectOwner, char **ObjectName) = NULL;
int   (*funcptr_IDE_GetWindowType)() = NULL;
int   (*funcptr_IDE_GetAppHandle)() = NULL;
int   (*funcptr_IDE_GetWindowHandle)() = NULL;
int   (*funcptr_IDE_GetClientHandle)() = NULL;
int   (*funcptr_IDE_GetChildHandle)() = NULL;
void  (*funcptr_IDE_CreateWindow)(int WindowType, char *Text, BOOL Execute) = NULL;
BOOL  (*funcptr_IDE_OpenFile)(int WindowType, char *Filename) = NULL;
BOOL  (*funcptr_IDE_SaveFile)() = NULL;
char* (*funcptr_IDE_Filename)() = NULL;
void  (*funcptr_IDE_CloseFile)() = NULL;
void  (*funcptr_IDE_SetReadOnly)(BOOL ReadOnly) = NULL;
char* (*funcptr_IDE_GetText)() = NULL;
char* (*funcptr_IDE_GetSelectedText)() = NULL;
char* (*funcptr_IDE_GetCursorWord)() = NULL;
int   (*funcptr_IDE_GetEditorHandle)() = NULL;
BOOL  (*funcptr_IDE_SetText)(char *Text) = NULL;//34*******************
BOOL  (*funcptr_IDE_SetStatusMessage)(char *Text) = NULL;//35*******************
BOOL  (*funcptr_IDE_SetErrorPosition)(int Line, int Col) = NULL;//36*******************
void  (*funcptr_IDE_ClearErrorPositions)() = NULL;//37*******************
int   (*funcptr_IDE_GetCursorWordPosition)() = NULL;//38*******************
BOOL  (*funcptr_IDE_Perform)(int Param) = NULL;//
int   (*funcptr_SQL_Execute)(char *SQL) = NULL;
int   (*funcptr_SQL_FieldCount)() = NULL;
BOOL  (*funcptr_SQL_Eof)() = NULL;
int   (*funcptr_SQL_Next)() = NULL;
char* (*funcptr_SQL_Field)(int Field) = NULL;
char* (*funcptr_SQL_FieldName)(int Field) = NULL;
int   (*funcptr_SQL_FieldIndex)(char *Name) = NULL;
int   (*funcptr_SQL_FieldType)(int Field) = NULL;

void RegisterCallback(int Index, void *Addr)
{
	switch (Index)
	{
    case 1 :
		memcpy(&funcptr_SYS_Version, &Addr, sizeof(Addr));
		break;
    case 2 :
		memcpy(&funcptr_SYS_Registry, &Addr, sizeof(Addr));
		break;
    case 3 :
		memcpy(&funcptr_SYS_RootDir, &Addr, sizeof(Addr));
		break;
    case 4 :
		memcpy(&funcptr_SYS_OracleHome, &Addr, sizeof(Addr));
		break;
    case 10 :
		memcpy(&funcptr_IDE_MenuState, &Addr, sizeof(Addr));
		break;
    case 11 :
		memcpy(&funcptr_IDE_Connected, &Addr, sizeof(Addr));
		break;
    case 12 :
		memcpy(&funcptr_IDE_GetConnectionInfo, &Addr, sizeof(Addr));
		break;
    case 13 :
		memcpy(&funcptr_IDE_GetBrowserInfo, &Addr, sizeof(Addr));
		break;
    case 14 :
		memcpy(&funcptr_IDE_GetWindowType, &Addr, sizeof(Addr));
		break;
    case 15 :
		memcpy(&funcptr_IDE_GetAppHandle, &Addr, sizeof(Addr));
		break;
    case 16 :
		memcpy(&funcptr_IDE_GetWindowHandle, &Addr, sizeof(Addr));
		break;
    case 17 :
		memcpy(&funcptr_IDE_GetClientHandle, &Addr, sizeof(Addr));
		break;
    case 18 :
		memcpy(&funcptr_IDE_GetChildHandle, &Addr, sizeof(Addr));
		break;
    case 20 :
		memcpy(&funcptr_IDE_CreateWindow, &Addr, sizeof(Addr));
		break;
    case 21 :
		memcpy(&funcptr_IDE_OpenFile, &Addr, sizeof(Addr));
		break;
    case 22 :
		memcpy(&funcptr_IDE_SaveFile, &Addr, sizeof(Addr));
		break;
    case 23 :
		memcpy(&funcptr_IDE_Filename, &Addr, sizeof(Addr));
		break;
    case 24 :
		memcpy(&funcptr_IDE_CloseFile, &Addr, sizeof(Addr));
		break;
    case 25 :
		memcpy(&funcptr_IDE_SetReadOnly, &Addr, sizeof(Addr));
		break;
    case 30 :
		memcpy(&funcptr_IDE_GetText, &Addr, sizeof(Addr));
		break;
    case 31 :
		memcpy(&funcptr_IDE_GetSelectedText, &Addr, sizeof(Addr));
		break;
    case 32 :
		memcpy(&funcptr_IDE_GetCursorWord, &Addr, sizeof(Addr));
		break;
    case 33 :
		memcpy(&funcptr_IDE_GetEditorHandle, &Addr, sizeof(Addr));
		break;
    case 34 :
		memcpy(&funcptr_IDE_SetText, &Addr, sizeof(Addr));
		break;
    case 35 :
		memcpy(&funcptr_IDE_SetStatusMessage, &Addr, sizeof(Addr));
		break;
    case 36 :
		memcpy(&funcptr_IDE_SetErrorPosition, &Addr, sizeof(Addr));
		break;
    case 37 :
		memcpy(&funcptr_IDE_ClearErrorPositions, &Addr, sizeof(Addr));
		break;
    case 38 :
		memcpy(&funcptr_IDE_GetCursorWordPosition, &Addr, sizeof(Addr));
		break;
    case 39 : //????
		memcpy(&funcptr_IDE_Perform, &Addr, sizeof(Addr));
		break;
    case 40 :
		memcpy(&funcptr_SQL_Execute, &Addr, sizeof(Addr));
		break;
    case 41 :
		memcpy(&funcptr_SQL_FieldCount, &Addr, sizeof(Addr));
		break;
    case 42 :
		memcpy(&funcptr_SQL_Eof, &Addr, sizeof(Addr));
		break;
    case 43 :
		memcpy(&funcptr_SQL_Next, &Addr, sizeof(Addr));
		break;
    case 44 :
		memcpy(&funcptr_SQL_Field, &Addr, sizeof(Addr));
		break;
    case 45 :
		memcpy(&funcptr_SQL_FieldName, &Addr, sizeof(Addr));
		break;
    case 46 :
		memcpy(&funcptr_SQL_FieldIndex, &Addr, sizeof(Addr));
		break;
    case 47 :
		memcpy(&funcptr_SQL_FieldType, &Addr, sizeof(Addr));
		break;
	}
}

void ErrorMessage(const char* ErrMsg)
{
	std::stringstream ss;
	ss << "Error:  (" << ErrMsg << ") Function not initialized!!!" ;
	MessageBox(NULL, ss.str().c_str(), NULL, 0);
}
int   SYS_Version(){if (!funcptr_SYS_Version) {ErrorMessage("SYS_Version");return NULL;}return funcptr_SYS_Version();}
char* SYS_Registry(){if (!funcptr_SYS_Registry) {ErrorMessage("SYS_Registry");return NULL;}return funcptr_SYS_Registry();}
char* SYS_RootDir(){if (!funcptr_SYS_RootDir) {ErrorMessage("SYS_RootDir");return NULL;}return funcptr_SYS_RootDir();}
char* SYS_OracleHome(){if (!funcptr_SYS_OracleHome) {ErrorMessage("SYS_OracleHome");return NULL;}return funcptr_SYS_OracleHome();}
int   IDE_GetWindowType(){if (!funcptr_IDE_GetWindowType) {ErrorMessage("IDE_GetWindowType");return NULL;}return funcptr_IDE_GetWindowType();}
int   IDE_GetAppHandle(){if (!funcptr_IDE_GetAppHandle) {ErrorMessage("IDE_GetAppHandle");return NULL;}return funcptr_IDE_GetAppHandle();}
int   IDE_GetWindowHandle(){if (!funcptr_IDE_GetWindowHandle) {ErrorMessage("IDE_GetWindowHandle");return NULL;}return funcptr_IDE_GetWindowHandle();}
int   IDE_GetClientHandle(){if (!funcptr_IDE_GetClientHandle) {ErrorMessage("IDE_GetClientHandle");return NULL;}return funcptr_IDE_GetClientHandle();}
int   IDE_GetChildHandle(){if (!funcptr_IDE_GetChildHandle) {ErrorMessage("IDE_GetChildHandle");return NULL;}return funcptr_IDE_GetChildHandle();}
BOOL  IDE_Connected(){if (!funcptr_IDE_Connected) {ErrorMessage("IDE_Connected");return NULL;}return funcptr_IDE_Connected();}
BOOL  IDE_SaveFile(){if (!funcptr_IDE_SaveFile) {ErrorMessage("IDE_SaveFile");return NULL;}return funcptr_IDE_SaveFile();}
char* IDE_Filename(){if (!funcptr_IDE_Filename) {ErrorMessage("IDE_Filename");return NULL;}return funcptr_IDE_Filename();}
void  IDE_CloseFile(){if (!funcptr_IDE_CloseFile) {ErrorMessage("IDE_CloseFile");return;}funcptr_IDE_CloseFile();}
char* IDE_GetText(){if (!funcptr_IDE_GetText) {ErrorMessage("IDE_GetText");return NULL;}return funcptr_IDE_GetText();}
char* IDE_GetSelectedText(){if (!funcptr_IDE_GetSelectedText) {ErrorMessage("IDE_GetSelectedText");return NULL;}return funcptr_IDE_GetSelectedText();}
char* IDE_GetCursorWord(){if (!funcptr_IDE_GetCursorWord) {ErrorMessage("IDE_GetCursorWord");return NULL;}return funcptr_IDE_GetCursorWord();}
int   IDE_GetEditorHandle(){if (!funcptr_IDE_GetEditorHandle) {ErrorMessage("IDE_GetEditorHandle");return NULL;}return funcptr_IDE_GetEditorHandle();}
int   SQL_FieldCount(){if (!funcptr_SQL_FieldCount) {ErrorMessage("SQL_FieldCount");return NULL;}return funcptr_SQL_FieldCount();}
BOOL  SQL_Eof(){if (!funcptr_SQL_Eof) {ErrorMessage("SQL_Eof");return NULL;}return funcptr_SQL_Eof();}
int   SQL_Next(){if (!funcptr_SQL_Next) {ErrorMessage("SQL_Next");return NULL;}return funcptr_SQL_Next();}
void  IDE_ClearErrorPositions(){if (!funcptr_IDE_ClearErrorPositions) {ErrorMessage("IDE_ClearErrorPositions");return;} funcptr_IDE_ClearErrorPositions();}
int   IDE_GetCursorWordPosition(){if (!funcptr_IDE_GetCursorWordPosition) {ErrorMessage("IDE_GetCursorWordPosition");return NULL;}return funcptr_IDE_GetCursorWordPosition();}

void  IDE_MenuState(int ID, int Index, BOOL Enabled)
{
	if (!funcptr_IDE_MenuState) 
	{
		ErrorMessage("IDE_MenuState");
		return;
	}
	funcptr_IDE_MenuState(ID, Index, Enabled);
}
void  IDE_GetConnectionInfo(char **Username, char **Password, char **Database)
{
	if (!funcptr_IDE_GetConnectionInfo) {ErrorMessage("IDE_GetConnectionInfo");return;}
	funcptr_IDE_GetConnectionInfo(Username, Password, Database);
}
void  IDE_GetBrowserInfo(char **ObjectType, char **ObjectOwner, char **ObjectName)
{
	if (!funcptr_IDE_GetBrowserInfo) {ErrorMessage("IDE_GetBrowserInfo");return;}
	funcptr_IDE_GetBrowserInfo(ObjectType, ObjectOwner, ObjectName);
}
void  IDE_CreateWindow(WINTYPE WindowType, char *Text, BOOL Execute)
{
	if (!funcptr_IDE_CreateWindow) {ErrorMessage("IDE_CreateWindow");return;}
	funcptr_IDE_CreateWindow(WindowType, Text, Execute);
}
BOOL  IDE_OpenFile(WINTYPE WindowType, char *Filename)
{
	if (!funcptr_IDE_OpenFile) {ErrorMessage("IDE_OpenFile");return NULL;}
	return funcptr_IDE_OpenFile(WindowType, Filename);
}
void  IDE_SetReadOnly(BOOL ReadOnly)
{
	if (!funcptr_IDE_SetReadOnly) {ErrorMessage("IDE_SetReadOnly");return;}
	funcptr_IDE_SetReadOnly(ReadOnly);
}
int   SQL_Execute(char *SQL)
{
	if (!funcptr_SQL_Execute) {ErrorMessage("SQL_Execute");return NULL;}
	return funcptr_SQL_Execute(SQL);
}
char* SQL_Field(int Field)
{
	if (!funcptr_SQL_Field) {ErrorMessage("SQL_Field");return NULL;}
	return funcptr_SQL_Field(Field);
}
char* SQL_FieldName(int Field)
{
	if (!funcptr_SQL_FieldName) {ErrorMessage("SQL_FieldName");return NULL;}
	return funcptr_SQL_FieldName(Field);
}
int   SQL_FieldIndex(char *Name)
{
	if (!funcptr_SQL_FieldIndex) {ErrorMessage("SQL_FieldIndex");return NULL;}
	return funcptr_SQL_FieldIndex(Name);
}
int   SQL_FieldType(int Field)
{
	if (!funcptr_SQL_FieldType) {ErrorMessage("SQL_FieldType");return NULL;}
	return funcptr_SQL_FieldType(Field);
}
//Newly added functions
BOOL IDE_Perform(PCMD Param)
{
	if (!funcptr_IDE_Perform) {ErrorMessage("IDE_Perform");return NULL;}
	return funcptr_IDE_Perform(Param);
/*This function allows you to perform a specific action as if the menu item as specified in Param was selected. The following values are supported:
  1: Execute
  2: Break
  3: Kill
  4: Commit
  5: Rollback
  6: Print
*/
}
BOOL  IDE_SetText(char *Text)
{
	if (!funcptr_IDE_SetText) {ErrorMessage("IDE_SetText");return NULL;}
	return funcptr_IDE_SetText(Text);
}
BOOL  IDE_SetStatusMessage(char *Text)
{
	if (!funcptr_IDE_SetStatusMessage) {ErrorMessage("IDE_SetStatusMessage");return NULL;}
	return funcptr_IDE_SetStatusMessage(Text);
}
BOOL  IDE_SetErrorPosition(int Line, int Col)
{
	if (!funcptr_IDE_SetErrorPosition) {ErrorMessage("IDE_SetErrorPosition");return NULL;}
	return funcptr_IDE_SetErrorPosition(Line, Col);
}

