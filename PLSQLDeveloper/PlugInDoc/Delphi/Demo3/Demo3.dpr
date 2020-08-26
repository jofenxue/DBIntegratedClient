// PL/SQL Developer Plug-In framework
// Copyright 2006 Allround Automations
// support@allroundautomations.com
// http://www.allroundautomations.com

library Demo3;

{ Important note about DLL memory management: ShareMem must be the
  first unit in your library's USES clause AND your project's (select
  View-Project Source) USES clause if your DLL exports any procedures or
  functions that pass strings as parameters or function results. This
  applies to all strings passed to and from your DLL--even those that
  are nested in records and classes. ShareMem is the interface unit to
  the DELPHIMM.DLL shared memory manager, which must be deployed along
  with your DLL. To avoid using DELPHIMM.DLL, pass string information
  using PChar or ShortString parameters. }

uses
  Demo3Unit in 'Demo3Unit.pas' {DemoForm},
  PlugInIntf in 'PlugInIntf.pas',
  Demo3PrefUnit in 'Demo3PrefUnit.pas' {DemoPrefsForm};

begin
end.
