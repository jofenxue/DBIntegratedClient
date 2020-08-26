// PL/SQL Developer Plug-In demo
// Copyright 1999 Allround Automations
// support@allroundautomations.nl
// http://www.allroundautomations.nl

// This demo shows a basic use of all available functions

unit Demo2Unit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Buttons, Grids, ExtCtrls;

type
  TDemoForm = class(TForm)
    TopPanel: TPanel;
    Memo: TMemo;
    Grid: TStringGrid;
    ExecuteBtn: TBitBtn;
    CloseBtn: TBitBtn;
    procedure CloseBtnClick(Sender: TObject);
    procedure ExecuteBtnClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  DemoForm: TDemoForm;
  PlugInID: Integer;

const // Window types
  wtSQL = 1;
  wtTest = 2;
  wtProcEdit = 3;
  wtCommand = 4;
  wtPlan = 5;

var // Declaration of PL/SQL Developer callback functions
  SYS_Version: function: Integer; cdecl;
  SYS_Registry: function: PChar; cdecl;
  SYS_RootDir: function: PChar; cdecl;
  SYS_OracleHome: function: PChar; cdecl;

  IDE_MenuState: procedure(ID, Index: Integer; Enabled: Bool); cdecl;
  IDE_Connected: function: Bool; cdecl;
  IDE_GetConnectionInfo: procedure(var Username, Password, Database: PChar); cdecl;
  IDE_GetBrowserInfo: procedure(var ObjectType, ObjectOwner, ObjectName: PChar); cdecl;
  IDE_GetWindowType: function: Integer; cdecl;
  IDE_GetAppHandle: function: Integer; cdecl;
  IDE_GetWindowHandle: function: Integer; cdecl;
  IDE_GetClientHandle: function: Integer; cdecl;
  IDE_GetChildHandle: function: Integer; cdecl;

  IDE_CreateWindow: procedure(WindowType: Integer; Text: PChar; Execute: Bool); cdecl;
  IDE_OpenFile: function(WindowType: Integer; Filename: PChar): Bool; cdecl;
  IDE_SaveFile: function: Bool; cdecl;
  IDE_Filename: function: PChar; cdecl;
  IDE_CloseFile: procedure; cdecl;
  IDE_SetReadOnly: procedure(ReadOnly: Bool); cdecl;

  IDE_GetText: function: PChar; cdecl;
  IDE_GetSelectedText: function: PChar; cdecl;
  IDE_GetCursorWord: function: PChar; cdecl;
  IDE_GetEditorHandle: function: Integer; cdecl;

  SQL_Execute: function(SQL: PChar): Integer; cdecl;
  SQL_FieldCount: function: Integer; cdecl;
  SQL_Eof: function: Bool; cdecl;
  SQL_Next: function: Integer; cdecl;
  SQL_Field: function(Field: Integer): PChar; cdecl;
  SQL_FieldName: function(Field: Integer): PChar; cdecl;
  SQL_FieldIndex: function(Name: PChar): Integer; cdecl;
  SQL_FieldType: function(Field: Integer): Integer; cdecl;

const // Description of this Plug-In (as displayed in Plug-In configuration dialog)
  Desc = 'Delphi Plug-In demo 2';

implementation

{$R *.DFM}

// Plug-In identification, a unique identifier is received and
// the description is returned
function IdentifyPlugIn(ID: Integer): PChar;  cdecl;
begin
  PlugInID := ID;
  Result := Desc;
end;

// Registration of PL/SQL Developer callback functions
procedure RegisterCallback(Index: Integer; Addr: Pointer); cdecl;
begin
  case Index of
     1 : @SYS_Version := Addr;
     2 : @SYS_Registry := Addr;
     3 : @SYS_RootDir := Addr;
     4 : @SYS_OracleHome := Addr;
    10 : @IDE_MenuState := Addr;
    11 : @IDE_Connected := Addr;
    12 : @IDE_GetConnectionInfo := Addr;
    13 : @IDE_GetBrowserInfo := Addr;
    14 : @IDE_GetWindowType := Addr;
    15 : @IDE_GetAppHandle := Addr;
    16 : @IDE_GetWindowHandle := Addr;
    17 : @IDE_GetClientHandle := Addr;
    18 : @IDE_GetChildHandle := Addr;
    20 : @IDE_CreateWindow := Addr;
    21 : @IDE_OpenFile := Addr;
    22 : @IDE_SaveFile := Addr;
    23 : @IDE_Filename := Addr;
    24 : @IDE_CloseFile := Addr;
    25 : @IDE_SetReadOnly := Addr;
    30 : @IDE_GetText := Addr;
    31 : @IDE_GetSelectedText := Addr;
    32 : @IDE_GetCursorWord := Addr;
    33 : @IDE_GetEditorHandle := Addr;
    40 : @SQL_Execute := Addr;
    41 : @SQL_FieldCount := Addr;
    42 : @SQL_Eof := Addr;
    43 : @SQL_Next := Addr;
    44 : @SQL_Field := Addr;
    45 : @SQL_FieldName := Addr;
    46 : @SQL_FieldIndex := Addr;
    47 : @SQL_FieldType := Addr;
  end;
end;

// Creating menus
function CreateMenuItem(Index: Integer): PChar;  cdecl;
begin
  Result := '';
  case Index of
    1 : Result := 'Tools / &Plug-In 2 Demo / &Systeem info / &Version...';
    2 : Result := 'Tools / &Plug-In 2 Demo / &Systeem info / &Registry...';
    3 : Result := 'Tools / &Plug-In 2 Demo / &Systeem info / Root&Dir...';
    4 : Result := 'Tools / &Plug-In 2 Demo / &Systeem info / &OracleHome...';
    5 : Result := 'Tools / &Plug-In 2 Demo / I&DE Info / &Connection Info...';
    6 : Result := 'Tools / &Plug-In 2 Demo / I&DE Info / &Browser Info...';
    7 : Result := 'Tools / &Plug-In 2 Demo / I&DE Info / &Window Type...';
    8 : Result := 'Tools / &Plug-In 2 Demo / I&DE Info / &Filename...';
    9 : Result := 'Tools / &Plug-In 2 Demo / &Create / &SQL Window';
   10 : Result := 'Tools / &Plug-In 2 Demo / &Create / &Test Window';
   11 : Result := 'Tools / &Plug-In 2 Demo / &Create / &Procedure Window';
   12 : Result := 'Tools / &Plug-In 2 Demo / &Create / &Command Window';
   13 : Result := 'Tools / &Plug-In 2 Demo / &Open File';
   14 : Result := 'Tools / &Plug-In 2 Demo / &Save File';
   15 : Result := 'Tools / &Plug-In 2 Demo / &Close File';
   16 : Result := 'Tools / &Plug-In 2 Demo / &ReadOnly / &On';
   17 : Result := 'Tools / &Plug-In 2 Demo / &ReadOnly / O&ff';
   18 : Result := 'Tools / &Plug-In 2 Demo / &Editor/ ShowText...';
   19 : Result := 'Tools / &Plug-In 2 Demo / &Editor/ ShowSelection...';
   20 : Result := 'Tools / &Plug-In 2 Demo / &Editor/ ShowWord...';
   21 : Result := 'Tools / &Plug-In 2 Demo / &Editor/ ShowHandle...';
   22 : Result := 'Tools / &Plug-In 2 Demo / &Editor/ SetSelection';
   23 : Result := 'Tools / &Plug-In 2 Demo / &Maximize Window';
   24 : Result := 'Tools / &Plug-In 2 Demo / -';
   25 : Result := 'Tools / &Plug-In 2 Demo / &SQL Form...';
  end;
end;

// Called when someone logs on or off
procedure OnConnectionChange;
var c: Boolean;
begin
  c := IDE_Connected;
  IDE_MenuState(PlugInID, 5, c);
  IDE_MenuState(PlugInID, 25, c);
end;

// Called when another item in the browser gets selected
procedure OnBrowserChange;
var oType, oOwner, oName: PChar;
begin
  IDE_GetBrowserInfo(oType, oOwner, oName);
  IDE_MenuState(PlugInID, 6, StrComp(oType, '') <> 0);
end;

// Called when child windows change focus
procedure OnWindowChange;
var w: Integer;
begin
  w := IDE_GetWindowType;
  IDE_MenuState(PlugInID,  8, (w <> 0));
  IDE_MenuState(PlugInID, 14, (w <> 0));
  IDE_MenuState(PlugInID, 15, (w <> 0));
  IDE_MenuState(PlugInID, 16, (w <> 0));
  IDE_MenuState(PlugInID, 17, (w <> 0));
  IDE_MenuState(PlugInID, 18, (w <> 0));
  IDE_MenuState(PlugInID, 19, (w <> 0));
  IDE_MenuState(PlugInID, 20, (w <> 0));
  IDE_MenuState(PlugInID, 21, (w <> 0));
  IDE_MenuState(PlugInID, 22, (w <> 0));
  IDE_MenuState(PlugInID, 23, (w <> 0));
end;

// Called when the Plug-In is created
procedure OnCreate; cdecl;
begin
end;

// Called when the Plug-In is activated
procedure OnActivate; cdecl;
begin
  OnBrowserChange;
  OnConnectionChange;
  OnWindowChange;
end;

// Called when the Plug-In is destroyed
procedure OnDestroy; cdecl;
begin
end;

procedure DoSQLForm;
var Form: TDemoForm;
begin
  Application.Handle := IDE_GetAppHandle; 
  Form := TDemoForm.Create(Application);
  with Form do
  begin
    ShowModal;
    Free;
  end;
end;

procedure ShowBrowserObject;
var oType, oOwner, oName: PChar;
begin
  IDE_GetBrowserInfo(oType, oOwner, oName);
  ShowMessage(oType + #13 + oOwner + #13 + oName);
end;

procedure ShowConnectionInfo;
var Username, Password, Database: PChar;
begin
  IDE_GetConnectionInfo(Username, Password, Database);
  ShowMessage(Username + #13 + Password + #13 + Database);
end;

procedure SetSelection(S: string);
var H: Integer;
begin
  H := IDE_GetEditorHandle;
  if H > 0 then
  begin
    SendMessage(H, em_ReplaceSel, Integer(True), Integer(PChar(S)));
  end;
end;

procedure MaximizeWindow;
var H: Integer;
begin
  H := IDE_GetChildHandle;
  if H > 0 then
  begin
    SendMessage(IDE_GetClientHandle, wm_MDIMaximize, H, 0);
  end;
end;

// One of our menus is selected
procedure OnMenuClick(Index: Integer);  cdecl;
begin
  case Index of
    1 : ShowMessage(IntToStr(SYS_Version));
    2 : ShowMessage(SYS_Registry);
    3 : ShowMessage(SYS_RootDir);
    4 : ShowMessage(SYS_OracleHome);
    5 : ShowConnectionInfo;
    6 : ShowBrowserObject;
    7 : ShowMessage(IntToStr(IDE_GetWindowType));
    8 : ShowMessage(IDE_Filename);
    9 : IDE_CreateWindow(wtSQL, 'select * from emp', False);
   10 : IDE_CreateWindow(wtTest, 'select * from emp', False);
   11 : IDE_CreateWindow(wtProcEdit, 'select * from emp', False);
   12 : IDE_CreateWindow(wtCommand, 'select * from emp', False);
   13 : if not IDE_OpenFile(1, 'c:\autoexec.bat') then MessageBeep($FFFF);
   14 : if not IDE_SaveFile then MessageBeep($FFFF);
   15 : IDE_CloseFile;
   16 : IDE_SetReadOnly(True);
   17 : IDE_SetReadOnly(False);
   18 : ShowMessage(IDE_GetText);
   19 : ShowMessage(IDE_GetSelectedText);
   20 : ShowMessage(IDE_GetCursorWord);
   21 : ShowMessage(IntToStr(IDE_GetEditorHandle));
   22 : SetSelection('New selected text');
   23 : MaximizeWindow;
   25 : DoSQLForm;
  end;
end;

// SQL Form

procedure TDemoForm.CloseBtnClick(Sender: TObject);
begin
  Close;
end;

procedure TDemoForm.ExecuteBtnClick(Sender: TObject);
var fc, r, c: Integer;
begin
  r := 1;
  SQL_Execute(PChar(Memo.Text));
  fc := SQL_FieldCount;
  Grid.ColCount := fc;
  for c := 0 to fc-1 do
  begin
    Grid.Cells[c, 0] := SQL_FieldName(c);
  end;
  while not SQL_Eof do
  begin
    for c := 0 to fc-1 do Grid.Cells[c, r] := SQL_Field(c);
    inc(r);
    SQL_Next;
  end;
end;

// All exported functions
exports
  IdentifyPlugIn,
  CreateMenuItem,
  RegisterCallback,
  OnCreate,
  OnActivate,
  OnDestroy,
  OnMenuClick,
  OnBrowserChange,
  OnWindowChange,
  OnConnectionChange;

end.
