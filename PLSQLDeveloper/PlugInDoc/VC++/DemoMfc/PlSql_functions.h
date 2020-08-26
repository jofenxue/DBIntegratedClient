// PL/SQL Developer Plug-In demo
// Copyright 1999 Allround Automations
// support@allroundautomations.nl
// http://www.allroundautomations.nl
// Demo Developed by David Maisonave (david@axter.com)

#if !defined(HELPER_H_INCLUDED_)
#define HELPER_H_INCLUDED_

extern char *const Desc;
extern int PlugInID;

// Window types
enum WINTYPE{wtSQL = 1, wtTest, wtProcEdit, wtCommand, wtPlan};
enum PCMD{pcmd_Execute = 1, pcmd_Break, pcmd_Kill, pcmd_Commit, pcmd_Rollback, pcmd_Print};

// Declaration of PL/SQL Developer callback functions
int   SYS_Version();
char* SYS_Registry();
char* SYS_RootDir();
char* SYS_OracleHome();
void  IDE_MenuState(int ID, int Index, BOOL Enabled);
BOOL  IDE_Connected();
void  IDE_GetConnectionInfo(char **Username, char **Password, char **Database);
void  IDE_GetBrowserInfo(char **ObjectType, char **ObjectOwner, char **ObjectName);
int   IDE_GetWindowType();
int   IDE_GetAppHandle();
int   IDE_GetWindowHandle();
int   IDE_GetClientHandle();
int   IDE_GetChildHandle();
void  IDE_CreateWindow(WINTYPE WindowType, char *Text, BOOL Execute);
BOOL  IDE_OpenFile(WINTYPE WindowType, char *Filename);
BOOL  IDE_SaveFile();
char* IDE_Filename();
void  IDE_CloseFile();
void  IDE_SetReadOnly(BOOL ReadOnly);
char* IDE_GetText();
char* IDE_GetSelectedText();
char* IDE_GetCursorWord();
int   IDE_GetEditorHandle();
BOOL  IDE_SetText(char *Text);//34*******************
BOOL  IDE_SetStatusMessage(char *Text);//35*******************
BOOL  IDE_SetErrorPosition(int Line, int Col);//36*******************
void  IDE_ClearErrorPositions();//37*******************
int   IDE_GetCursorWordPosition();//38*******************
int   SQL_Execute(char *SQL);
int   SQL_FieldCount();
BOOL  SQL_Eof();
int   SQL_Next();
char* SQL_Field(int Field);
char* SQL_FieldName(int Field);
int   SQL_FieldIndex(char *Name);
int   SQL_FieldType(int Field);
BOOL IDE_Perform(PCMD Param);
bool FileExist(const char* FileName);
bool FileExistViaAccess(const char* FileName);
bool IsDirectory(const char* FileName);

#endif // !defined(HELPER_H_INCLUDED_)
