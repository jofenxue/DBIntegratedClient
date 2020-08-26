// PL/SQL Developer Plug-In demo
// Copyright 1999 Allround Automations
// support@allroundautomations.nl
// http://www.allroundautomations.nl

// This demo shows an elementary interface

unit Demo1Unit;

interface

uses
  Windows, Dialogs;

var
  PlugInID: Integer;

const // Description of this Plug-In (as displayed in Plug-In configuration dialog)
  Desc = 'Delphi Plug-In demo 1';

implementation

// Plug-In identification, a unique identifier is received and
// the description is returned
function IdentifyPlugIn(ID: Integer): PChar;  cdecl;
begin
  PlugInID := ID;
  Result := Desc;
end;

// Creating a menu item
function CreateMenuItem(Index: Integer): PChar;  cdecl;
begin
  Result := '';
  case Index of
    1 : Result := 'Tools / &Plug-In 1 Demo...';
  end;
end;

// The menu item got selected
procedure OnMenuClick(Index: Integer);  cdecl;
begin
  case Index of
    1 : ShowMessage('Hello');
  end;
end;

exports // The three basic export functions
  IdentifyPlugIn,
  CreateMenuItem,
  OnMenuClick;

end.

