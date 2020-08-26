// PL/SQL Developer Plug-In demo
// Copyright 1999 Allround Automations
// support@allroundautomations.nl
// http://www.allroundautomations.nl

// This demo shows a basic use of all available functions

//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Demo2Unit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDemoForm *DemoForm;
//---------------------------------------------------------------------------
__fastcall TDemoForm::TDemoForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
// All exported functions
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
}

// Description of this Plug-In (as displayed in Plug-In configuration dialog)
char *const Desc = "C++Builder Plug-In demo 2";
int PlugInID;

// Window types
const wtSQL = 1;
const wtTest = 2;
const wtProcEdit = 3;
const wtCommand = 4;
const wtPlan = 5;

// Declaration of PL/SQL Developer callback functions
int   (*SYS_Version)();
char* (*SYS_Registry)();
char* (*SYS_RootDir)();
char* (*SYS_OracleHome)();
void  (*IDE_MenuState)(int ID, int Index, BOOL Enabled);
BOOL  (*IDE_Connected)();
void  (*IDE_GetConnectionInfo)(char **Username, char **Password, char **Database);
void  (*IDE_GetBrowserInfo)(char **ObjectType, char **ObjectOwner, char **ObjectName);
int   (*IDE_GetWindowType)();
int   (*IDE_GetAppHandle)();
int   (*IDE_GetWindowHandle)();
int   (*IDE_GetClientHandle)();
int   (*IDE_GetChildHandle)();
void  (*IDE_CreateWindow)(int WindowType, char *Text, BOOL Execute);
BOOL  (*IDE_OpenFile)(int WindowType, char *Filename);
BOOL  (*IDE_SaveFile)();
char* (*IDE_Filename)();
void  (*IDE_CloseFile)();
void  (*IDE_SetReadOnly)(BOOL ReadOnly);
char* (*IDE_GetText)();
char* (*IDE_GetSelectedText)();
char* (*IDE_GetCursorWord)();
int   (*IDE_GetEditorHandle)();
int   (*SQL_Execute)(char *SQL);
int   (*SQL_FieldCount)();
BOOL  (*SQL_Eof)();
int   (*SQL_Next)();
char* (*SQL_Field)(int Field);
char* (*SQL_FieldName)(int Field);
int   (*SQL_FieldIndex)(char *Name);
int   (*SQL_FieldType)(int Field);
//---------------------------------------------------------------------------
// Plug-In identification, a unique identifier is received and
// the description is returned
char* IdentifyPlugIn(int ID)
{
  PlugInID = ID;
  return Desc;
}
//---------------------------------------------------------------------------
// Registration of PL/SQL Developer callback functions
void RegisterCallback(int Index, void *Addr)
{
  switch (Index)
  {
    case 1 :
      (void *)SYS_Version = Addr;
      break;
    case 2 :
      (void *)SYS_Registry = Addr;
      break;
    case 3 :
      (void *)SYS_RootDir = Addr;
      break;
    case 4 :
      (void *)SYS_OracleHome = Addr;
      break;
    case 10 :
      (void *)IDE_MenuState = Addr;
      break;
    case 11 :
      (void *)IDE_Connected = Addr;
      break;
    case 12 :
      (void *)IDE_GetConnectionInfo = Addr;
      break;
    case 13 :
      (void *)IDE_GetBrowserInfo = Addr;
      break;
    case 14 :
      (void *)IDE_GetWindowType = Addr;
      break;
    case 15 :
      (void *)IDE_GetAppHandle = Addr;
      break;
    case 16 :
      (void *)IDE_GetWindowHandle = Addr;
      break;
    case 17 :
      (void *)IDE_GetClientHandle = Addr;
      break;
    case 18 :
      (void *)IDE_GetChildHandle = Addr;
      break;
    case 20 :
      (void *)IDE_CreateWindow = Addr;
      break;
    case 21 :
      (void *)IDE_OpenFile = Addr;
      break;
    case 22 :
      (void *)IDE_SaveFile = Addr;
      break;
    case 23 :
      (void *)IDE_Filename = Addr;
      break;
    case 24 :
      (void *)IDE_CloseFile = Addr;
      break;
    case 25 :
      (void *)IDE_SetReadOnly = Addr;
      break;
    case 30 :
      (void *)IDE_GetText = Addr;
      break;
    case 31 :
      (void *)IDE_GetSelectedText = Addr;
      break;
    case 32 :
      (void *)IDE_GetCursorWord = Addr;
      break;
    case 33 :
      (void *)IDE_GetEditorHandle = Addr;
      break;
    case 40 :
      (void *)SQL_Execute = Addr;
      break;
    case 41 :
      (void *)SQL_FieldCount = Addr;
      break;
    case 42 :
      (void *)SQL_Eof = Addr;
      break;
    case 43 :
      (void *)SQL_Next = Addr;
      break;
    case 44 :
      (void *)SQL_Field = Addr;
      break;
    case 45 :
      (void *)SQL_FieldName = Addr;
      break;
    case 46 :
      (void *)SQL_FieldIndex = Addr;
      break;
    case 47 :
      (void *)SQL_FieldType = Addr;
      break;
  }
}
//---------------------------------------------------------------------------
// Creating menus
char* CreateMenuItem(int Index)
{
  switch (Index)
  {
    case  1 : return "Tools / &Plug-In 2 Demo / &Systeem info / &Version...";
    case  2 : return "Tools / &Plug-In 2 Demo / &Systeem info / &Registry...";
    case  3 : return "Tools / &Plug-In 2 Demo / &Systeem info / Root&Dir...";
    case  4 : return "Tools / &Plug-In 2 Demo / &Systeem info / &OracleHome...";
    case  5 : return "Tools / &Plug-In 2 Demo / I&DE Info / &Connection Info...";
    case  6 : return "Tools / &Plug-In 2 Demo / I&DE Info / &Browser Info...";
    case  7 : return "Tools / &Plug-In 2 Demo / I&DE Info / &Window Type...";
    case  8 : return "Tools / &Plug-In 2 Demo / I&DE Info / &Filename...";
    case  9 : return "Tools / &Plug-In 2 Demo / &Create / &SQL Window";
    case 10 : return "Tools / &Plug-In 2 Demo / &Create / &Test Window";
    case 11 : return "Tools / &Plug-In 2 Demo / &Create / &Procedure Window";
    case 12 : return "Tools / &Plug-In 2 Demo / &Create / &Command Window";
    case 13 : return "Tools / &Plug-In 2 Demo / &Open File";
    case 14 : return "Tools / &Plug-In 2 Demo / &Save File";
    case 15 : return "Tools / &Plug-In 2 Demo / &Close File";
    case 16 : return "Tools / &Plug-In 2 Demo / &ReadOnly / &On";
    case 17 : return "Tools / &Plug-In 2 Demo / &ReadOnly / O&ff";
    case 18 : return "Tools / &Plug-In 2 Demo / &Editor/ ShowText...";
    case 19 : return "Tools / &Plug-In 2 Demo / &Editor/ ShowSelection...";
    case 20 : return "Tools / &Plug-In 2 Demo / &Editor/ ShowWord...";
    case 21 : return "Tools / &Plug-In 2 Demo / &Editor/ ShowHandle...";
    case 22 : return "Tools / &Plug-In 2 Demo / &Editor/ SetSelection";
    case 23 : return "Tools / &Plug-In 2 Demo / &Maximize Window";
    case 24 : return "Tools / &Plug-In 2 Demo / -";
    case 25 : return "Tools / &Plug-In 2 Demo / &SQL Form...";
  }
  return "";
}
//---------------------------------------------------------------------------
// Called when someone logs on or off
void OnConnectionChange()
{
  BOOL c;

  c = IDE_Connected();
  IDE_MenuState(PlugInID, 5, c);
  IDE_MenuState(PlugInID, 25, c);
}
//---------------------------------------------------------------------------
// Called when another item in the browser gets selected
void OnBrowserChange()
{
  char *Type, *Owner, *Name;

  IDE_GetBrowserInfo(&Type, &Owner, &Name);
  IDE_MenuState(PlugInID, 6, StrComp(Type, "") != 0);
}
//---------------------------------------------------------------------------
// Called when child windows change focus
void OnWindowChange()
{
  int w;

  w = IDE_GetWindowType();
  IDE_MenuState(PlugInID,  8, (w != 0));
  IDE_MenuState(PlugInID, 14, (w != 0));
  IDE_MenuState(PlugInID, 15, (w != 0));
  IDE_MenuState(PlugInID, 16, (w != 0));
  IDE_MenuState(PlugInID, 17, (w != 0));
  IDE_MenuState(PlugInID, 18, (w != 0));
  IDE_MenuState(PlugInID, 19, (w != 0));
  IDE_MenuState(PlugInID, 20, (w != 0));
  IDE_MenuState(PlugInID, 21, (w != 0));
  IDE_MenuState(PlugInID, 22, (w != 0));
  IDE_MenuState(PlugInID, 23, (w != 0));
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
  TDemoForm *Form;

//  Application->Handle = IDE_GetAppHandle;
  Form = new TDemoForm(NULL);
  Form->ShowModal();
  delete Form;
}
//---------------------------------------------------------------------------
void ShowBrowserObject()
{
  char *Type, *Owner, *Name;
  char s[100];

  IDE_GetBrowserInfo(&Type, &Owner, &Name);
  s[0] = 0;
  StrCat(s, Type);
  StrCat(s, "\n");
  StrCat(s, Owner);
  StrCat(s, "\n");
  StrCat(s, Name);
  ShowMessage(s);
}
//---------------------------------------------------------------------------
void ShowConnectionInfo()
{
  char *Username, *Password, *Database;
  char s[100];

  IDE_GetConnectionInfo(&Username, &Password, &Database);
  s[0] = 0;
  StrCat(s, Username);
  StrCat(s, "\n");
  StrCat(s, Password);
  StrCat(s, "\n");
  StrCat(s, Database);
  ShowMessage(s);
}
//---------------------------------------------------------------------------
void SetSelection(char *s)
{
  int H;

  H = IDE_GetEditorHandle();
  if (H > 0)
  {
    SendMessage((HWND)H, EM_REPLACESEL, true, (int)s);
  }
}
//---------------------------------------------------------------------------
void MaximizeWindow()
{
  int H, M;

  H = IDE_GetChildHandle();
  M = IDE_GetClientHandle();
  if (H > 0)
  {
    SendMessage((HWND)M, WM_MDIMAXIMIZE, H, (int)0);
  }
}
//---------------------------------------------------------------------------
void OnMenuClick(int Index)
{
  switch (Index)
  {
    case 1 :
      ShowMessage(IntToStr(SYS_Version()));
      break;
    case 2 :
      ShowMessage(SYS_Registry());
      break;
    case 3 :
      ShowMessage(SYS_RootDir());
      break;
    case 4 :
      ShowMessage(SYS_OracleHome());
      break;
    case 5 :
      ShowConnectionInfo();
      break;
    case 6 :
      ShowBrowserObject();
      break;
    case 7 :
      ShowMessage(IntToStr(IDE_GetWindowType()));
      break;
    case 8 :
      ShowMessage(IDE_Filename());
      break;
    case 9 :
      IDE_CreateWindow(wtSQL, "select * from emp", false);
      break;
    case 10 :
      IDE_CreateWindow(wtTest, "select * from emp", false);
      break;
    case 11 :
      IDE_CreateWindow(wtProcEdit, "select * from emp", false);
      break;
    case 12 :
     IDE_CreateWindow(wtCommand, "select * from emp", false);
      break;
    case 13 :
      if (!IDE_OpenFile(1, "c:\\autoexec.bat")) MessageBeep(0xFFFFFFFF);
      break;
    case 14 :
      if (!IDE_SaveFile()) MessageBeep(0xFFFFFFFF);
      break;
    case 15 :
      IDE_CloseFile();
      break;
    case 16 :
      IDE_SetReadOnly(true);
      break;
    case 17 :
      IDE_SetReadOnly(false);
      break;
    case 18 :
      ShowMessage(IDE_GetText());
      break;
    case 19 :
      ShowMessage(IDE_GetSelectedText());
      break;
    case 20 :
      ShowMessage(IDE_GetCursorWord());
      break;
    case 21 :
      ShowMessage(IntToStr(IDE_GetEditorHandle()));
      break;
    case 22 :
      SetSelection("New selected text");
      break;
    case 23 :
      MaximizeWindow();
      break;
    case 25 :
      DoSQLForm();
      break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TDemoForm::CloseBtnClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TDemoForm::ExecuteBtnClick(TObject *Sender)
{
  int fc, r, c;

  r = 1;
  SQL_Execute(Memo->Text.c_str());
  fc = SQL_FieldCount();
  Grid->ColCount = fc;
  for (c = 0; c < fc; c++)
  {
    Grid->Cells[c][0] = SQL_FieldName(c);
  }
  while (!SQL_Eof())
  {
    for (c = 0; c < fc; c++) Grid->Cells[c][r] = SQL_Field(c);
    r++;
    SQL_Next();
  }
}
//---------------------------------------------------------------------------

