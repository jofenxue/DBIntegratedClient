// PL/SQL Developer Plug-In demo
// Copyright 1999 Allround Automations
// support@allroundautomations.nl
// http://www.allroundautomations.nl

// This demo shows the use of some basic functions in a practical plug-In that
// implements a wrap function.
// The selected object in the Browser will be saved as a text file and "wrapped"

//---------------------------------------------------------------------------
#include <vcl.h>
#include <registry.hpp>
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
  __declspec(dllexport) void  RegisterCallback(int, void *);
  __declspec(dllexport) void  OnMenuClick(int);
  __declspec(dllexport) void  OnActivate();
  __declspec(dllexport) void  OnBrowserChange();
}

// Description of this Plug-In (as displayed in Plug-In configuration dialog)
char *const Desc = "Wrap demo version 1.0";
int  PlugInID;
AnsiString WrapExe = "";

// Declaration of PL/SQL Developer callback functions

// We want to save and restore the directory in the registry
char* (*SYS_Registry)();
// We need to search the wrap executable in ORACLE_HOME\bin
char* (*SYS_OracleHome)();

// The menu item will only be enabled if a program unit is selected in the Browser
void  (*IDE_MenuState)(int ID, int Index, BOOL Enabled);
// The currently selected object in the Browser is the one that will be wrapped
void  (*IDE_GetBrowserInfo)(char **ObjectType, char **ObjectOwner, char **ObjectName);

// To retrieve the source from the database, we need to query sys.all_source
int   (*SQL_Execute)(char *SQL);
BOOL  (*SQL_Eof)();
int   (*SQL_Next)();
char* (*SQL_Field)(int Field);
//---------------------------------------------------------------------------
// Determine the name for a temporary file
AnsiString TempFilename()
{
  char Path[144];

  GetTempPath(sizeof(Path), Path);
  return (AnsiString)Path + "wrap.tmp";
}
//---------------------------------------------------------------------------
// Determine the Wrap executable. If more than one is present, use the most
// recent one
void GetWrapExe()
{
  AnsiString Path;
  TSearchRec SearchRec;
  int TimeStamp;
  int Error;

  Path = (AnsiString)SYS_OracleHome() + "bin\\";
  Error = FindFirst(Path + "wrap*.exe", faArchive, SearchRec);
  TimeStamp = 0;
  while (Error == 0)
  {
    if (SearchRec.Time > TimeStamp)
    {
      WrapExe = Path + SearchRec.Name;
      TimeStamp = SearchRec.Time;
    }
    Error = FindNext(SearchRec);
  }
  FindClose(SearchRec);
}
//---------------------------------------------------------------------------
// Save the currently selected program unit to a temporary file
BOOL SaveProgramUnit()
{
  char *ObjectType, *ObjectOwner, *ObjectName;
  int Error;
  AnsiString SQL;
  TStringList *Source;
  AnsiString Line;
  BOOL Result;

  // Determine what object is selected in the Browser
  IDE_GetBrowserInfo(&ObjectType, &ObjectOwner, &ObjectName);
  // Build a select statement to retrieve the source
  SQL = Format("select text from sys.all_source"
               " where type = '%s' and owner = '%s' and name = '%s'"
               " order by line", ARRAYOFCONST((ObjectType, ObjectOwner, ObjectName)));
  // Execute the select statement and retrieve all source lines
  Source = new TStringList;
  Error = SQL_Execute(SQL.c_str());
  while ((Error == 0) && (!SQL_Eof()))
  {
    Line = TrimRight(SQL_Field(0));
    // Add the 'create or replace' syntax on the first line
    if (Source->Count == 0) Line = "create or replace " + Line;
    Source->Add(Line);
    Error = SQL_Next();
  }
  Result = (Error == 0);
  if (!Result)
    ShowMessage("SQL error " + IntToStr(Error));
  else
  {
    try
    {
      // Save the text to a temporary file
      Source->SaveToFile(TempFilename());
    }
    catch (Exception &E)
    {
      ShowMessage(E.Message);
      Result = False;
    }
  }
  delete Source;
  return Result;
}
//---------------------------------------------------------------------------
// Determine the destination file name
AnsiString GetDestination()
{
  char *ObjectType, *ObjectOwner, *ObjectName;
  TSaveDialog *SaveDialog;
  TRegistry *Registry;
  AnsiString LastDir;
  AnsiString Result;

  // Determine the last used directory, as stored in the regitsry
  Registry = new TRegistry;
  Registry->RootKey = HKEY_CURRENT_USER;
  if (Registry->OpenKey((AnsiString)SYS_Registry() + "\\WrapPlugIn", TRUE))
    LastDir = Registry->ReadString("LastDir");
  else
    LastDir = "";
  // Determine the object name, to be used as a default filename
  IDE_GetBrowserInfo(&ObjectType, &ObjectOwner, &ObjectName);
  // Create a dialog to ask for a destination filename
  SaveDialog = new TSaveDialog(NULL);
  // Set various properties of the SaveDialog
  SaveDialog->Title      = "Save wrapped output file as";
  SaveDialog->DefaultExt = "plb";
  SaveDialog->Filter     = "Wrapped files (*.plb)|*.plb|All files (*.*)|*.*";
  SaveDialog->InitialDir = LastDir;
  SaveDialog->FileName   = AnsiLowerCase(ObjectName);
  SaveDialog->Options    << ofOverwritePrompt;
  // Execute it
  if (!SaveDialog->Execute())
    Result = "";
  else
  {
    // Save the destination directory in the registry
    Result = SaveDialog->FileName;
    Registry->WriteString("LastDir", ExtractFilePath(Result));
  }
  delete Registry;
  return Result;
}
//---------------------------------------------------------------------------
// Wrap the program unit currently selected in the Browser
void WrapIt()
{
  AnsiString WrapCommand;
  AnsiString Destination;

  // Check if we can actually wrap
  if (WrapExe == "")
  {
    ShowMessage("Wrap executable not found in " + (AnsiString)SYS_OracleHome() + "bin");
//    Exit;
  }
  // Ask for the destination filename
  Destination = GetDestination();
  // If successful, continue
  if (!Destination.IsEmpty())
  {
    // Save the currently selected program unit to a temporary file
    if (SaveProgramUnit())
    {
      // If successfully saved, execute the wrap command
      WrapCommand = Format("%s iname=""%s"" oname=""%s""", ARRAYOFCONST((WrapExe, TempFilename(), Destination)));
      WinExec(WrapCommand.c_str(), SW_NORMAL);
    }
  }
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
// Registration of PL/SQL Developer callback functions
void RegisterCallback(int Index, void *Addr)
{
  switch (Index)
  {
    case 2 :
      (void *)SYS_Registry = Addr;
      break;
    case 4 :
      (void *)SYS_OracleHome = Addr;
      break;
    case 10 :
      (void *)IDE_MenuState = Addr;
      break;
    case 13 :
      (void *)IDE_GetBrowserInfo = Addr;
      break;
    case 40 :
      (void *)SQL_Execute = Addr;
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
  }
}
//---------------------------------------------------------------------------
// Creating a menu item
char* CreateMenuItem(int Index)
{
  switch (Index)
  {
    case 1 : return "Tools / &Wrap program unit...";
  }
  return "";
}
//---------------------------------------------------------------------------
// Depending on the selected object in the Browser, enable/disable the menu item
void OnBrowserChange()
{
  char *ObjectType, *ObjectOwner, *ObjectName;
  BOOL Enabled;

  IDE_GetBrowserInfo(&ObjectType, &ObjectOwner, &ObjectName);
  Enabled = (StrComp(ObjectType, "FUNCTION") == 0) ||
            (StrComp(ObjectType, "PROCEDURE") == 0) ||
            (StrComp(ObjectType, "PACKAGE") == 0) ||
            (StrComp(ObjectType, "PACKAGE BODY") == 0) ||
            (StrComp(ObjectType, "TRIGGER") == 0) ||
            (StrComp(ObjectType, "TYPE") == 0) ||
            (StrComp(ObjectType, "TYPE BODY") == 0);
  IDE_MenuState(PlugInID, 1, Enabled);
}
//---------------------------------------------------------------------------
// When the Plug-In gets activated, call OnBrowserChange to de/activate the menu
void OnActivate()
{
  GetWrapExe();
  OnBrowserChange();
}
//---------------------------------------------------------------------------
// The menu item got selected
void OnMenuClick(int Index)
{
  switch (Index)
  {
    case 1 :
      WrapIt();
      break;
  }
}
//---------------------------------------------------------------------------
