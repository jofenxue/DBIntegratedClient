// PL/SQL Developer Plug-In framework
// Copyright 2006 Allround Automations
// support@allroundautomations.com
// http://www.allroundautomations.com

unit Demo3PrefUnit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, PlugInIntf;

type
  TDemoPrefsForm = class(TForm)
    Edit1: TEdit;
    Edit2: TEdit;
    Label1: TLabel;
    Label2: TLabel;
    Button1: TButton;
    Button2: TButton;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  DemoPrefsForm: TDemoPrefsForm;

procedure DoPreferences;

implementation

uses Demo3Unit;

{$R *.DFM}

procedure DoPreferences;
begin
  DemoPrefsForm := TDemoPrefsForm.Create(Application);
  with DemoPrefsForm do
  begin
    Edit1.Text := IntToStr(Pref1);
    Edit2.Text := Pref2;
    if ShowModal = mrOK then
    begin
      // Store preferences
      Pref1 := StrToInt(Edit1.Text);
      Pref2 := Edit2.Text;
      IDE_SetPrefAsInteger(PlugInID, '', 'Pref1', Pref1);
      IDE_SetPrefAsString(PlugInID, '', 'Pref2', PChar(Pref2));
    end;
    Free;
  end;
  DemoPrefsForm := nil;
end;

end.
