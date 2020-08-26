// PL/SQL Developer Plug-In framework
// Copyright 2006 Allround Automations
// support@allroundautomations.com
// http://www.allroundautomations.com

unit Demo3Unit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Buttons, Grids, ExtCtrls, PlugInIntf;

type
  TDemoForm = class(TForm)
    Panel1: TPanel;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  DemoForm: TDemoForm;
  
var // Some demo preferences, stored in the PL/SQL Developer preference file
  Pref1: Integer = 10;
  Pref2: string = 'Hello';

const // Description of this Plug-In (as displayed in Plug-In configuration dialog)
  Desc = 'Delphi Plug-In demo 3';

implementation

uses Demo3PrefUnit;

{$R *.DFM}

// Plug-In identification, a unique identifier is received and
// the description is returned
function IdentifyPlugIn(ID: Integer): PChar;  cdecl;
begin
  PlugInID := ID;
  Result := Desc;
end;

// This function will be called with an Index ranging from 1 to 99. For every Index you
// can return a string that creates a new menu-item in PL/SQL Developer.
function CreateMenuItem(Index: Integer): PChar;  cdecl;
begin
  Result := '';
  case Index of
    1 : Result := 'Tools / &Plug-In 3 Demo ...';
    2 : Result := 'Tools / &Plug-In 3 Preferences ...';
  end;
end;

// This function is called when a user selected a menu-item created with the
// CreateMenuItem function and the Index parameter has the value (1 to 99) it is related to.
procedure OnMenuClick(Index: Integer);  cdecl;
begin
  case Index of
    1 : begin
          DemoForm := TDemoForm.Create(Application);
          DemoForm.ShowModal;
          DemoForm.Free;
          DemoForm := nil;
        end;
    2 : DoPreferences;
  end;
end;

// If your Plug-In depends on a selected item in the Browser, you can use this function
// to enable/disable menu-items. This function is called on every change in the Browser.
// You can use the IDE_GetBrowserInfo callback function to determine if the selected
// item is of interest to you.
procedure OnBrowserChange; cdecl;
begin
end;

// This function is called if PL/SQL Developer child windows change focus. You can use
// the IDE_GetWindowType callback to determine the active child window type.
procedure OnWindowChange; cdecl;
begin
end;

// This function is called when the Plug-In is loaded into memory. You can use it to do
// some one-time initialization. PL/SQL Developer is not logged on yet and you can’t
// use the callback functions, so you are limited in the things you can do.
procedure OnCreate; cdecl;
begin
end;

// OnActivate gets called after OnCreate. However, when OnActivate is called PL/SQL
// Developer and the Plug-In are fully initialized. This function is also called when the
// Plug-In is enabled in the configuration dialog. A good point to enable/disable menus.
procedure OnActivate; cdecl;
begin
  Application.Handle := IDE_GetAppHandle;
  Pref1 := IDE_GetPrefAsInteger(PlugInID, '', 'Pref1', Pref1);
  Pref2 := IDE_GetPrefAsString(PlugInID, '', 'Pref2', PChar(Pref2));
end;

// This is the counterpart of the OnActivate. It is called when the Plug-In is de-activated
// in the configuration dialog.
procedure OnDeactivate; cdecl;
begin
end;

// This is the counterpart of the OnCreate. You can dispose of anything you created in
// the OnCreate.
procedure OnDestroy; cdecl;
begin
end;

// This will be called when PL/SQL Developer is about to close. If your PlugIn is not
// ready to close, you can show a message and return False.
function CanClose: Bool; cdecl;
begin
  Result := True;
end;

// This function is called directly after a new window is created.
procedure OnWindowCreate(WindowType: Integer); cdecl;
begin
end;

// This function allows you to take some action before a window is closed. You can
// influence the closing of the window with the following return values:
// 0 = Default behavior
// 1 = Ask the user for confirmation (like the contents was changed)
// 2 = Don’t ask, allow to close without confirmation
// The Changed Boolean indicates the current status of the window.
function OnWindowClose(WindowType: Integer; Changed: BOOL): Integer; cdecl;
begin
  Result := 0;
end;

// This function is called when the user logs on to a different database or logs off. You
// can use the IDE_Connected and IDE_GetConnectionInfo callback to get information
// about the current connection.
procedure OnConnectionChange; cdecl;
var c: Boolean;
begin
  c := IDE_Connected;
  IDE_MenuState(PlugInID, 1, c);
end;

// This function allows you to display an about dialog. You can decide to display a
// dialog yourself (in which case you should return an empty text) or just return the
// about text.
function About: PChar; cdecl;
begin
  Result := 'Plug-In demo 3';
end;

// If the Plug-In has a configure dialog you could use this function to activate it. This will
// allow a user to configure your Plug-In using the configure button in the Plug-In
// configuration dialog.
procedure Configure; cdecl;
begin
  ShowMessage('This could be a preference dialog...');
end;

// You can use this function if you want the Plug-In to be able to accept commands from
// the command window.
// commands are entered like SQL> plugin [name] command param1 param2 ...
// the [name] is either the dll filename or the identifying (first) part of the description
// See IDE_CommandFeedback for how to return messages to the command window.
procedure CommandLine(FeedbackHandle: Integer; Command, Params: PChar); cdecl;
var i: Integer;
    P: TStringList;
begin
  P := TStringList.Create;
  P.Text := Params;
  IDE_CommandFeedback(FeedbackHandle, PChar('Command "' + Command + '"'));
  for i := 0 to P. Count - 1 do
    IDE_CommandFeedback(FeedbackHandle, PChar('Param' + IntToStr(i + 1) + ' "' + P[i] + '"'));
  P.Free;
end;

// Exported functions
exports
  IdentifyPlugIn,
  CreateMenuItem,
  RegisterCallback,
  OnCreate,
  OnActivate,
  OnDeactivate,
  OnDestroy,
  CanClose,
  OnMenuClick,
  OnBrowserChange,
  OnWindowChange,
  OnWindowCreate,
  OnWindowClose,
  OnConnectionChange,
  About,
  Configure,
  CommandLine;

end.
