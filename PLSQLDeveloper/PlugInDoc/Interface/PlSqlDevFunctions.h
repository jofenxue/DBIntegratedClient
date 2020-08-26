/*$FILE************************************************************************

      Module             PlSqlDevPlugIn

      File               PlSqlDevFunctions.h

      Contents           Pl/Sql Developer Plug-In Interface

      Created            Vladimir Schneider
                         www.winpte.com
                         www.beyondedit.com

                         Sep 10, 2006

      Copyright (c) 2006 Vladimir Schneider, All Rights Reserved

      1.                 Redistribution and use in source and binary forms, with or without
                         modification, are permitted provided that the
                         following conditions are met:

      2.                 Neither the name of WinPTE, BeyondEdit nor the
                         names of its contributors may be used to endorse or
                         promote products derived from this software without
                         specific prior written permission.


      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
      OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
      COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
      EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
      GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
      AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
      NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
      OF THE POSSIBILITY OF SUCH DAMAGE.

******X******************X************************X************************X$*/

#ifndef __PlSqlDevFunctions_h_
#define __PlSqlDevFunctions_h_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef _NO_PACK_
#pragma pack(push, 4)
#endif

//Functions to export to PL/SQL Dev
extern "C"
{
    __declspec(dllexport) void RegisterCallback(int nIndex, void *pvAddr);
    __declspec(dllexport) char* IdentifyPlugIn(int);
    __declspec(dllexport) void OnCreate();
    __declspec(dllexport) void OnActivate();
    __declspec(dllexport) void OnDeactivate();
    __declspec(dllexport) void OnDestroy();
    __declspec(dllexport) BOOL CanClose();

    __declspec(dllexport) char* CreateMenuItem(int);
    __declspec(dllexport) void OnMenuClick(int);

    // uncomment the ones that are of interest
    __declspec(dllexport) void OnBrowserChange();
    __declspec(dllexport) void OnWindowChange();
    __declspec(dllexport) void OnConnectionChange();
    __declspec(dllexport) int OnWindowClose(int WindowType, BOOL Changed);
    __declspec(dllexport) void OnWindowCreate(int WindowType);
    __declspec(dllexport) void Configure();

    __declspec(dllexport) void OnPopup(char *ObjectType, char *ObjectName);
    __declspec(dllexport) void OnMainMenu(char *MenuName);
    __declspec(dllexport) char *About();
    __declspec(dllexport) BOOL OnTemplate(char *Filename, char **Data);
    __declspec(dllexport) char *PlugInName();
    __declspec(dllexport) char *PlugInSubName();
    __declspec(dllexport) char *PlugInShortName();

    // __declspec(dllexport) char *RegisterFileSystem();
    // __declspec(dllexport) char *DirectFileLoad(char *Tag, char *Filename, int WindowType);
    // __declspec(dllexport) BOOL DirectFileSave(char *Tag, char *Filename, int WindowType);
    // __declspec(dllexport) char *RegisterExport();
    // __declspec(dllexport) void ExportInit();
    // __declspec(dllexport) void ExportFinished();
    // __declspec(dllexport) void ExportPrepare();
    // __declspec(dllexport) void ExportData();
}

// tab type returned by IDE_TabInfo()
#define PLSQL_TT_FUNCTION                         "Function"
#define PLSQL_TT_PROCEDURE                        "Procedure"
#define PLSQL_TT_TRIGGER                          "Trigger"
#define PLSQL_TT_JAVA_SOURCE                      "Java source"
#define PLSQL_TT_PACKAGE_SPEC                     "Package"
#define PLSQL_TT_PACKAGE_BODY                     "Package body"
#define PLSQL_TT_TYPE_SPEC                        "Type"
#define PLSQL_TT_TYPE_BODY                        "Type body"

// window type
#define PLSQL_WT_UNKNOWN                          0
#define PLSQL_WT_SQL                              1
#define PLSQL_WT_TEST                             2
#define PLSQL_WT_PROCEDURE                        3
#define PLSQL_WT_COMMAND                          4
#define PLSQL_WT_PLAN                             5
#define PLSQL_WT_REPORT                           6

// cursor word position
#define PLSQL_CWP_UNKNOWN                         0
#define PLSQL_CWP_CURSOR_START_WORD               1
#define PLSQL_CWP_CURSOR_WORD                     2
#define PLSQL_CWP_CURSOR_END_WORD                 3

// perform
#define PLSQL_PERFORM_EXECUTE                     1
#define PLSQL_PERFORM_BREAK                       2
#define PLSQL_PERFORM_KILL                        3
#define PLSQL_PERFORM_COMMIT                      4
#define PLSQL_PERFORM_ROLLBACK                    5
#define PLSQL_PERFORM_PRINT                       6

// keyword style
#define PLSQL_KWS_CUSTOM                          10
#define PLSQL_KWS_KEYWORDS                        11
#define PLSQL_KWS_COMMENT                         12
#define PLSQL_KWS_STRINGS                         13
#define PLSQL_KWS_NUMBERS                         14
#define PLSQL_KWS_SYMBOLS                         15

// popup
#define PLSQL_POPUP_PROGRAMWINDOW                 "PROGRAMWINDOW"
#define PLSQL_POPUP_SQLWINDOW                     "SQLWINDOW"
#define PLSQL_POPUP_TESTWINDOW                    "TESTWINDOW"
#define PLSQL_POPUP_COMMANDWINDOW                 "COMMANDWINDOW"

// refresh object
#define PLSQL_REFRESH_OBJECT_CREATED              1
#define PLSQL_REFRESH_OBJECT_MODIFIED             2
#define PLSQL_REFRESH_OBJECT_DELETED              3

// key simulation
#define PLSQL_KP_SHIFT   1
#define PLSQL_KP_ALT     2
#define PLSQL_KP_CTRL    3

// beautifier options
#define PLSQL_BO_AfterCreating                    1
#define PLSQL_BO_AfterLoading                     2
#define PLSQL_BO_BeforeCompiling                  4
#define PLSQL_BO_BeforeSaving                     8

// sql field type
#define PLSQL_FT_Integer                          3
#define PLSQL_FT_Float                            4
#define PLSQL_FT_String                           5
#define PLSQL_FT_Long                             8
#define PLSQL_FT_Date                             12
#define PLSQL_FT_LongRaw                          24

// SYSTEM Info functions

// Returns the PL/SQL Developer main and subversion, for example 210 for version 2.1.0. This
// might be useful if you want to use functions that are not available in all versions.
/*FUNC: 1*/ extern int (*SYS_Version)();

// Returns the registry root name of PL/SQL Developer in HKEY_CURRENT_USER (usually
// “Software\PL/SQL Developer”). If you want to save your settings in the registry, you can
// create a section within the PL/SQL Developer section.
//
// Note: In PL/SQL Developer 3.1, the registry section is moved to: (“Software\Allround
// Automations\PL/SQL Developer”)
/*FUNC: 2*/ extern char *(*SYS_Registry)();

// The directory where PL/SQL Developer is installed, for example “C:\Program Files\PLSQL
// Developer”.
/*FUNC: 3*/ extern char *(*SYS_RootDir)();

// The Oracle directory, for example “C:\Orawin95”
/*FUNC: 4*/ extern char *(*SYS_OracleHome)();

// Returns the path of the OCI DLL that is used by PL/SQL Developer. If you want to initialize
// a new session, you might want to use this value if you want to make sure you’re using the
// same OCI version.
// Available in version 300
/*FUNC: 5*/ extern char *(*SYS_OCIDLL)();

// Returns True if PL/SQL Developer is currently connected in OCI8 Mode (Net8).
// Available in version 300
/*FUNC: 6*/ extern BOOL* (*SYS_OCI8Mode)();

// Returns if PL/SQL Developer is currently using the visual XP style.
// Available in version 700
/*FUNC: 7*/ extern BOOL *(*SYS_XPStyle)();

// If Param is empty, the function will return the full tnsnames filename.
// If Param has a value, the connection details of the alias as specified by
// Param is returned. If Param is *, the connection details of the current
// connection are returned). The return value can look like:
// TEST =
// (DESCRIPTION =
//     (ADDRESS_LIST =
//         (ADDRESS = (PROTOCOL = TCP)(HOST = p2800)(PORT = 1521))
//     )
//     (CONNECT_DATA =
//         (SERVER = DEDICATED)
//         (SERVICE_NAME = AAA)
//     )
// )
// Available in version 700
/*FUNC: 8*/ extern char* (*SYS_TNSNAMES)(char *Param);
// IDE functions

// Use this function to enable or disable a menu. The ID is the Plug-In ID, which is given by
// the IdentifyPlugIn function. The Index is the menu index, which the menu was related to by
// the CreateMenuItem function. The Enabled boolean determines if the menu item is enabled or
// grayed.
/*FUNC: 10*/ extern void (*IDE_MenuState)(int ID, int Index, BOOL Enabled);

// Returns a boolean that indicates if PL/SQL Developer is currently connected to a database.
/*FUNC: 11*/ extern BOOL (*IDE_Connected)();

// Returns the username, password and database of the current connection.
/*FUNC: 12*/ extern void (*IDE_GetConnectionInfo)(char **Username, char **Password, char **Database);

// Returns information about the selected item in the Browser. If no item is selected, all items are empty.
/*FUNC: 13*/ extern void (*IDE_GetBrowserInfo)(char **ObjectType, char **ObjectOwner, char **ObjectName);

// Returns the type of the current window.
// 1 = SQL Window
// 2 = Test Window
// 3 = Procedure Window
// 4 = Command Window
// 5 = Plan Window
// 6 = Report Window
// 0 = None of the above
/*FUNC: 14*/ extern int (*IDE_GetWindowType)();

// Returns the Application handle of PL/SQL Developer
/*FUNC: 15*/ extern int (*IDE_GetAppHandle)();

// Returns the handle of PL/SQL Developers main window
/*FUNC: 16*/ extern HWND (*IDE_GetWindowHandle)();

// Returns the handle of PL/SQL Developers client window
/*FUNC: 17*/ extern HWND (*IDE_GetClientHandle)();

// Returns the handle of the active child form
/*FUNC: 18*/ extern HWND (*IDE_GetChildHandle)();

// Resets the state of the menus, buttons and the active window.  You can call this function if
// you made some changes that affect the state of a menu or window which are unnoticed by PL/SQL
// Developer.
//
// Available in version 213
/*FUNC: 19*/ extern void (*IDE_Refresh)();

// Creates a new window. The Text parameter contains text that is placed in the window. If the Execute Boolean is true, the Window will be executed.
// WindowType can be one of the following values:
// 1 = SQL Window
// 2 = Test Window
// 3 = Procedure Window
// 4 = Command Window
// 5 = Plan Window
// 6 = Report Window
/*FUNC: 20*/ extern void (*IDE_CreateWindow)(int WindowType, char *Text, BOOL Execute);

// Creates a window of type WindowType and loads the specified file.
// WindowType can be one of the following values:
// 1 = SQL Window
// 2 = Test Window
// 3 = Procedure Window
// 4 = Command Window
// The function returns True if successful.
//
// Version 301 and higher
// If you pass 0 as WindowType, PL/SQL Developer will try to determine the actual WindowType on the extension of the filename.
/*FUNC: 21*/ extern BOOL (*IDE_OpenFile)(int WindowType, char *Filename);

// This function saves the current window. It returns True if successful.
/*FUNC: 22*/ extern BOOL (*IDE_SaveFile)();

// Return the filename of the current child window.
// See also IDE_SetFilename()
/*FUNC: 23*/ extern char *(*IDE_Filename)();

// Closes the current child window
/*FUNC: 24*/ extern void (*IDE_CloseFile)();

// Set the ReadOnly status of the current Window
/*FUNC: 25*/ extern void (*IDE_SetReadOnly)(BOOL ReadOnly);

// Get the ReadOnly status of the current Window
// Available in version 213
/*FUNC: 26*/ extern BOOL (*IDE_GetReadOnly)();

// This function will execute a query (SQL parameter) and display the result in a ‘result only’
// SQL Window. Title will be used as the window name and the Updateable parameter determines if
// the results are updateable.
//
// Available in version 300
/*FUNC: 27*/ extern BOOL (*IDE_ExecuteSQLReport)(char *SQL, char *Title, BOOL Updateable);

// Forces the active child window to reload its file from disk.  Note: In PL/SQL Developer 4
// there will no longer be a warning message when modifications were made.
// Available in version 301
/*FUNC: 28*/ extern BOOL (*IDE_ReloadFile)();

// Set the filename of the active child window. The filename should contain a valid path, but
// the file does not need to exist. The new filename will be used when the file is saved.  If
// the Filename parameter is an empty string, the Window will behave as a new created Window.
//
// Available in version 303
/*FUNC: 29*/ extern void (*IDE_SetFilename)(char *Filename);

// Retrieves the text from the current child window.
/*FUNC: 30*/ extern char *(*IDE_GetText)();

// Retrieves the selected text from the current child window.
/*FUNC: 31*/ extern char *(*IDE_GetSelectedText)();

// Retrieves the word the cursor is on in the current child window.
/*FUNC: 32*/ extern char *(*IDE_GetCursorWord)();

// Returns the handle of the editor of the current child window.
/*FUNC: 33*/ extern HWND (*IDE_GetEditorHandle)();

// Sets the text in the editor of current window. If this failed for some reason (ReadOnly?),
// the function returns false.
//
// Available in version 213
/*FUNC: 34*/ extern BOOL (*IDE_SetText)(char *Text);

// Places a message in the status bar of the current window, returns false if the window did not
// have a status bar.
//
// Available in version 213
/*FUNC: 35*/ extern BOOL (*IDE_SetStatusMessage)(char *Text);

// Highlights the given line and places the cursor at the given position.  This will only work
// when the active window is a procedure window, if not, the function returns false.
//
// Available in version 213
/*FUNC: 36*/ extern BOOL (*IDE_SetErrorPosition)(int Line, int Col);

// Resets the highlighted lines.
// Available in version 213
/*FUNC: 37*/ extern void (*IDE_ClearErrorPositions)();

// This function returns the location of the cursor in the word after a call to
// IDE_GetCursorWord. Possible return values:
//
//   0: Unknown
//   1: Cursor was at start of word
//   2: Cursor was somewhere in the middle
//   3: Cursor was at the end
//
// Available in version 400
/*FUNC: 38*/ extern int (*IDE_GetCursorWordPosition)();

// This function allows you to perform a specific action as if the menu item as specified in
// Param was selected. The following values are supported:
//   1: Execute
//   2: Break
//   3: Kill
//   4: Commit
//   5: Rollback
//   6: Print
//
// Available in version 400
/*FUNC: 39*/ extern BOOL (*IDE_Perform)(int Param);

// Returns a list of all keywords as entered in the ‘custom keywords’ option in the Editor
// preference.
//
// Available in version 300
/*FUNC: 60*/ extern char *(*IDE_GetCustomKeywords)();

// Fills the custom keywords with the words in the Keywords parameter. Words should be
// separated by cr/lf. The currently used keywords will be overwritten.
//
// Available in version 300
/*FUNC: 61*/ extern void (*IDE_SetCustomKeywords)(char *Keywords);

// Adds a number of keywords with a specific style.  This function is more specific then
// IDE_SetCustomKeywords because this one can set multiple sets of keywords for different
// highlighting styles.  ID should be the PlugIn ID as returned by the IdentifyPlugIn function.
// Style can be one of the following values:
// 10: Custom
// 11: Keywords
// 12: Comment
// 13: Strings
// 14: Numbers
// 15: Symbols
// Keywords is a cr/lf separated list of words. You can define one list per style.
// Available in version 300
/*FUNC: 62*/ extern void (*IDE_SetKeywords)(int ID, int Style, char *Keywords);

// Activates the keywords as defined by the IDE_SetKeywords function.
// Available in version 300
/*FUNC: 63*/ extern void (*IDE_ActivateKeywords)();

// When this function is called, all menus for this Plug-In are removed and CreateMenuItem will
// be called to build a new set of menus. This only makes sense if you supply a different set
// of menu-items.
//
// Available in version 300
/*FUNC: 64*/ extern void (*IDE_RefreshMenus)(int ID);

// This function allows you to rename a certain menu-item.
// ID is the Plug-In ID, Index is the Menu number and name is the new menu name.
// Available in version 300
/*FUNC: 65*/ extern void (*IDE_SetMenuName)(int ID, int Index, char *Name);

// You can display or remove a check mark for a menu-item.
// Available in version 300
/*FUNC: 66*/ extern void (*IDE_SetMenuCheck)(int ID, int Index, BOOL Enabled);

// With this function you can hide or show a specific menu. You can use this instead of
// IDE_MenuState.
// Available in version 300
/*FUNC: 67*/ extern void (*IDE_SetMenuVisible)(int ID, int Index, BOOL Enabled);

// Returns a list of all standard PL/SQL Developer menu items. Items are separated by cr/lf and
// child menu level is indicated by a number of spaces.  You can use this function to build an
// advanced user configuration dialog where the user could be able to select place where he
// wants to insert the Plug-In menus.
//
// Available in version 300
/*FUNC: 68*/ extern char *(*IDE_GetMenulayout)();

// With this function you can add items to certain popup menus. The ID is the Plug-In ID and
// the index is the menu index. You can pass any number as the menu index, it can be an
// existing menu (as used by CreateMenuItem) or anything else. If the popup menu gets selected,
// OnMenuClick is called with the corresponding index.  The Name is the menu name as it will be
// displayed. The ObjectType determines in which popup menus this item will be displayed. Some
// possible values are: ‘TABLE’, ‘VIEW’, ‘PACKAGE’, etc.
//
// Version 301 and higher
// If you pass one of the following values as ObjectType, you can add items to specific Windows.
//   PROGRAMWINDOW
//   SQLWINDOW
//   TESTWINDOW
//   COMMANDWINDOW
//
// Version 400 and higher
// You can add popup items to Object Browser items like Tables, Views, etc. by passing their
// name as ObjectType.
//
// Version 510 and higher
// If you want to create popup menus for multiple selected items (of the same object type), you
// can add a + to the ObjectType parameter like ‘TABLE+’, ‘VIEW+’, etc. The OnMenuClick will be
// called for every selected item, and the GetPopupObject will return the correct details.
//
// Available in version 300
/*FUNC: 69*/ extern void* (*IDE_CreatePopupItem)(int ID, int Index, char *Name, char *ObjectType);

// This function allows you to reconnect PL/SQL Developer as another user. The return value
// indicates if the connection was successful.  The function will fail if there is a childwindow
// with an active query.  Also see IDE_SetConnectionAs
// Available in version 301
/*FUNC: 70*/ extern BOOL (*IDE_SetConnection)(char *Username, char *Password, char *Database);

// This function returns Oracle information about the item in the AnObject parameter. The
// SubObject returns the name of the procedure if the Object is a packaged procedure.
//
// Available in version 400
/*FUNC: 71*/ extern int (*IDE_GetObjectInfo)(char *AnObject, char **ObjectType, char **ObjectOwner, char **ObjectName, char **SubObject);

// Returns a cr/lf separated list of items from the Object Browser. The Node parameter
// determines which items are returned. This can be one of the main items like TABLES, but you
// can also us a slash to get more specific items like TABLES/DEPT/COLUMNS.  The GetItems
// boolean determines if PL/SQL Developer will fetch these values from the database if the item
// has not been opened yet in the Browser.
//
// Available in version 400
/*FUNC: 72*/ extern char *(*IDE_GetBrowserItems)(char *Node, BOOL GetItems);

// Force a refresh to the Object Browser. If Node is empty, all items are refreshed. To
// refresh a specific item you can enter the name in the Node parameter.
//
// Note: Version 500 allows you to pass a * to refresh the current selected browser item.
//
// Available in version 400
// Note:
// Version 500 allows you to pass a * to refresh the current selected
// browser item.
// Note:
// Version 600 allows you to pass a ** to refresh to parent of the current
// browser item, and you can pass *** to refresh to root item.
/*FUNC: 73*/ extern void (*IDE_RefreshBrowser)(char *Node);

// This function returns information about the item for which a popup menu (created with
// IDE_CreatePopupItem) was activated.  If the item is a Browser folder, the name of the folder
// will be returned in ObjectName and ObjectType will return ‘FOLDER’
//
// Available in version 400
/*FUNC: 74*/ extern int (*IDE_GetPopupObject)(char **ObjectType, char **ObjectOwner, char **ObjectName, char **SubObject);

// This function returns the name of browser root item for which a popup menu (created with
// IDE_CreatePopupItem) was activated.
//
// Available in version 400
/*FUNC: 75*/ extern char *(*IDE_GetPopupBrowserRoot)();

// If you modify database objects in your Plug-In and you want to update PL/SQL Developer to
// reflect these changes, you can do so by calling this function. You should pass the object
// type, owner, name and the action that you performed on the object. The action can be one of
// the following:
//
//   1 = Object created
//   2 = Object modified
//   3 = Object deleted
//
// PL/SQL Developer will update the browser and all windows that might use the object.
//
// Available in version 400
/*FUNC: 76*/ extern void (*IDE_RefreshObject) (char *ObjectType, char *ObjectOwner, char *ObjectName, int Action);

// This function will return the details of the first selected in the Browser. The function
// will return false if no items are selected.  Use in combination with IDE_NextSelectedObject
// to determine all selected items.
//
// Available in version 500
/*FUNC: 77*/ extern BOOL (*IDE_FirstSelectedObject)(char *ObjectType, char *ObjectOwner, char *ObjectName, char *SubObject);

// This function can be called after a call to IDE_FirstSelectedObject to determine all selected
// objects. You can keep calling this function until it returns false.
//
// Available in version 500
/*FUNC: 78*/ extern BOOL (*IDE_NextSelectedObject)(char *ObjectType, char *ObjectOwner, char *ObjectName, char *SubObject);

// Returns the source for the specified object. This function will only return source for
// objects that actually have source (packages, views, …).
//
// Available in version 511
/*FUNC: 79*/ extern char *(*IDE_GetObjectSource)(char *ObjectType, char *ObjectOwner, char *ObjectName);

// Returns the number of child windows in PL/SQL Developer. In combination with
// IDE_SelectWindow you can communicate with all child windows.
//
// Available in version 301
/*FUNC: 80*/ extern int (*IDE_GetWindowCount)();

// This function will ‘select’ one of PL/SQL Developers child Windows. Index is the window
// number where 0 is the top child window. The return value will indicate if the window
// existed.
//
// Normally all window related functions communicate with the active child window. With this
// function you can select any window and all window-related IDE functions will refer to the
// selected window.
//
// Note: IDE_SelectWindow does not actually bring the window to front, you need
// IDE_ActivateWindow to do that.
//
// Available in version 301
/*FUNC: 81*/ extern BOOL (*IDE_SelectWindow)(int Index);

// Brings the Indexth child window with to front.
// Available in version 301
/*FUNC: 82*/ extern BOOL (*IDE_ActivateWindow)(int Index);

// Returns if the contents of the window is modified.
// Available in version 301
/*FUNC: 83*/ extern BOOL (*IDE_WindowIsModified)();

// Returns if there is anything running in the current window.
// Available in version 301
/*FUNC: 84*/ extern BOOL (*IDE_WindowIsRunning)();

// Creates an empty splash screen (the one you see when PL/SQL Developer is starting or
// printing) which allows you to show some kind of progress on lengthy operations.  If the
// ProgressMax parameter is larger then 0, a progress bar is displayed which you can advance
// with the IDE_SplashProgress function.
//
// Note: There can only be one splash screen active at a
// time. If a splash screen is created while one was active, the first one will get re-used.
//
// Available in version 303
/*FUNC: 90*/ extern void (*IDE_SplashCreate)(int ProgressMax);

// Hides the splash screen. This function will work on any splash screen, you can even hide the
// one created by PL/SQL Developer.
//
// Available in version 303
/*FUNC: 91*/ extern void (*IDE_SplashHide)();

// Add text to the splash screen.
// Available in version 303
/*FUNC: 92*/ extern void (*IDE_SplashWrite)(char *s);

// Add text to the splash screen beginning on the next line.
// Available in version 303
/*FUNC: 93*/ extern void (*IDE_SplashWriteLn)(char *s);

// If the splash screen was created with a progress bar, you can indicate progress with this function.
// Available in version 303
/*FUNC: 94*/ extern void (*IDE_SplashProgress)(int Progress);

// This function returns the path where the templates are located.
// Available in version 400
/*FUNC: 95*/ extern char *(*IDE_TemplatePath)();

// If you want to execute a template from within your PlugIn you can do so with this function.
// The NewWindow parameter indicates if a new window should be created or that the result of the
// template should be pasted at the current cursor position in the active window. The template
// parameter should contain the template name. If the template is located in one or more
// folders, the folder name(s) should be prefixed to the template name separated by a backslash.
//
// Available in version 400
/*FUNC: 96*/ extern BOOL (*IDE_ExecuteTemplate)(char *Template, BOOL NewWindow);

// Use this function to determine if the current connection has a specific ‘Connect As’.
// Possible return values are: '', 'SYSDBA' and 'SYSOPER'
// Available in version 500
/*FUNC: 97*/ extern char *(*IDE_GetConnectAs)();

// Identical to IDE_SetConnection, but with an option to specify a ConnectAs parameter. You can
// pass 'SYSDBA' or 'SYSOPER', all other values will be handled as 'NORMAL'.
//
// Available in version 500
/*FUNC: 98*/ extern BOOL (*IDE_SetConnectionAs)(char *Username, char *Password, char *Database, char *ConnectAs);

//*****************************************************************************
//
//    External FileSystem functions
//
//****X******************X************************X************************X***

// If you want to create a new ‘File Open’ menu with the same items as the standard menu, you
// can use this function to determine the standard items. You can call this function in a loop
// while incrementing MenuIndex (starting with 0) until the return value is an empty string.
// The return values are the menu names in the File Open menu and the WindowType is the
// corresponding window type.
//
// Available in version 400
/*FUNC: 100*/ extern char *(*IDE_GetFileOpenMenu)(int MenuIndex, int *WindowType);

// Returns True if the active child window can be saved. (which are the SQL, Test, Program and
// Command windows).
//
// Available in version 400
/*FUNC: 101*/ extern BOOL (*IDE_CanSaveWindow)();

// Creates a new Window (of type WindowType) for the specified (and registered) FileSystem, Tag
// and Filename.
//
// Available in version 400
/*FUNC: 102*/ extern void (*IDE_OpenFileExternal)(int WindowType, char *Data, char *FileSystem, char *Tag, char *Filename);

// Returns the defined filetypes for a specific WindowType.
//
// Available in version 400
/*FUNC: 103*/ extern char *(*IDE_GetFileTypes)(int WindowType);

// Returns the default extension (without period) for a specific window type.
// Available in version 400
/*FUNC: 104*/ extern char *(*IDE_GetDefaultExtension)(int WindowType);

// Returns the data of a window. You can use this function to get the data and save it.
//
// Available in version 400
/*FUNC: 105*/ extern char *(*IDE_GetFiledata)();

// You can call this function when a file is saved successfully. The filename will be set in
// the Window caption and the status will display that the file is ‘saved successfully’.
// FileSystem and FileTag can be nil.
//
// Available in version 400
/*FUNC: 106*/ extern void (*IDE_FileSaved)(char *FileSystem, char *FileTag, char *Filename);

// This function displays a html file in a child window. The url parameter identifies the file
// and the hash parameter allows you to jump to a specific location. The title parameter will
// be used as window title.  You can refresh the contents of an already opened window by
// specifying an ID. If ID is not empty, and a window exists with the same ID, this will be
// used, otherwise a new window will be created.
//
// Available in version 510
/*FUNC: 107*/ extern BOOL (*IDE_ShowHTML)(char *Url, char *Hash, char *Title, char *ID);

// Refresh the contents of a HTML Window. You can pass a url to refress all windows that show a
// specific url, or you can pass an ID to refresh a specific Window.
//
// Available in version 512
/*FUNC: 108*/ extern BOOL (*IDE_RefreshHTML)(char *Url, char *ID, BOOL     BringToFront);

// Returns the define file extension of a specific object type. The oType parameter can hold
// one of the following valies:
//
// FUNCTION
// PROCEDURE
// TRIGGER
// PACKAGE
// PACKAGE BODY
// PACKAGE SPEC AND BODY
// TYPE
// TYPE BODY
// TYPE SPEC AND BODY
// JAVA SOURCE
//
// Available in version 514
/*FUNC: 109*/ extern char *(*IDE_GetProcEditExtension)(char *oType);

// Get info about the object opened in a Window. This will only work for Program Windows.
//
// Available in version 512
/*FUNC: 110*/ extern BOOL (*IDE_GetWindowObject) (char **ObjectType, char **ObjectOwner, char **ObjectName, char **SubObject);

//*****************************************************************************
//
//    IDE functions
//
//****X******************X************************X************************X***

// Simulates a key press. You can use this function to do the things you can also do with the
// keyboard. The Key parameter is the virtual key code of the key, and the Shift parameter
// holds the status of the Shift Ctrl and Alt keys. You can combine the following values:
//  1 = Shift
//  2 = Alt
//  3 = Ctrl
// Available in version 510
/*FUNC: 120*/ extern void (*IDE_KeyPress)(int Key, int Shift);

// This function will return an ‘index’ of a specific menu item. The MenuName parameter must
// specify the menu path separated by a slash, for example ‘edit / selection / uppercase’. The
// menu name is not case sensitive. If the function returns zero, the menu did not exist.  You
// can use the return value with IDE_SelectMenu
// Available in version 510
/*FUNC: 121*/ extern int (*IDE_GetMenuItem)(char *MenuName);

// You can execute a menu item with this function. The MenuItem parameter has to be determined
// by the IDE_SelectMenu function. If this function returns false, the menu did not exist, or
// it was disabled.
// Available in version 510
/*FUNC: 122*/ extern BOOL (*IDE_SelectMenu)(int MenuItem);

// Returns the currently used translation file. If the return value is empty, no translation is
// used.
// Available in version 510
/*FUNC: 130*/ extern char *(*IDE_TranslationFile)();

// Returns the language of the currently used translation file. If the return value is empty,
// no translation is used.
// Available in version 510
/*FUNC: 131*/ extern char *(*IDE_TranslationLanguage)();

// Returns a list of all standard PL/SQL Developer menu items like IDE_GetMenuLayout, but this
// function will return the translated menus.
// Available in version 510
/*FUNC: 132*/ extern char *(*IDE_GetTranslatedMenuLayout)();

// PL/SQL Developer has a preference to save all opened files on a time interval, and/or when an
// Execute is performed. In case of a crash (from the system, Oracle or PL/SQL Dev), the user
// will be able to recover the edited files.  If the Plug-In can do things that have a possible
// risk of causing a crash, you can call this function to protect the user’s work.
//
// Available in version 510
/*FUNC: 140*/ extern BOOL  (*IDE_SaveRecoveryFiles)();

// Returns the (1 based) character position of the cursor in the current editor.
// Available in version 510
/*FUNC: 141*/ extern int (*IDE_GetCursorX)();

// Returns the (1 based) line position of the cusror in the current editor.
// Available in version 510
/*FUNC: 142*/ extern int (*IDE_GetCursorY)();

// Set the cursor in the current editor. If the X or Y parameter is 0, the position will not change.
// This function will also update the position display in the statusbar.
// Available in version 510
/*FUNC: 143*/ extern void (*IDE_SetCursor)(int X, int Y);

// Create a bookmark at position X (character), Y (line). Index is the bookmark (0..9) you want
// to set. If you pass –1 as bookmark, the first free bookmark will be used. The returned
// value is the used bookmark.
//
// Normally, from within PL/SQL Developer. Bookmarks can only be used for windows with a gutter
// (Test window and Program editor), but the Plug-In interface allows you to use bookmarks for
// all windows.
//
// Available in version 510
/*FUNC: 144*/ extern int (*IDE_SetBookmark)(int Index, int X, int Y);

// Clears the specified bookmark
// Available in version 510
/*FUNC: 145*/ extern void (*IDE_ClearBookmark)(int Index);

// Jumps to a bookmark
// Available in version 510
/*FUNC: 146*/ extern void (*IDE_GotoBookmark)(int Index);

// Get the cursor position for a specific bookmark
// Available in version 510
/*FUNC: 147*/ extern BOOL (*IDE_GetBookmark)(int Index, int X, int Y);

// Returns the description tab page Index (zero based). The return value is empty if the tab
// page does not exist. This function allows you to determine which tab pages (if any) are
// available for the current window.
//
// Available in version 511
/*FUNC: 148*/ extern char *(*IDE_TabInfo)(int Index);

// This function allows you to read or set the active tab page. To set a specific page, pass a
// zero based value to the Index parameter. The return value is the actual selected page. To
// determine the active page (without setting it) pass a value of –1 to the Index parameter.
//
// Available in version 511
/*FUNC: 149*/ extern int (*IDE_TabIndex)(int Index);

// This function allows you to add Toolbuttons to your Plug-In, similar to IDE_CreatePopupItem.
// The ID is the Plug-In ID and the index is the menu index. When a button is selected,
// OnMenuClick is called with the corresponding index.
//
// The Name will appear as hint for the button, and as name in the preferences dialog.
//
// The button can be enabled and disabled with IDE_MenuState.
//
// The image for the button can be set by passing a filename to a bmp file in the BitmapFile
// parameter, or as a handle to a bitmap in memory.  The bmp image can have any number of
// colors, but should approximately be 20 x 20 pixels in size.
//
// The button will only be visible if it is selected in the Toolbar preference.
//
// Available in version 510
/*FUNC: 150*/ extern void (*IDE_CreateToolButton)(int ID, int Index, char *Name, char *BitmapFile, int BitmapHandle);

// Returns the PL/SQL Beautifier options.  The result is a value where the following values are
// or-ed together:
//   1 AfterCreating enabled
//   2 AfterLoading enabled
//   4 BeforeCompiling enabled
//   8 BeforeSaving enabled
// You can use this to determine if you need to call the beautifier.
//
// Available in version 510
/*FUNC: 160*/ extern int (*IDE_BeautifierOptions)();

// Calls the PL/SQL Beautifier for the current Window. The result indicates if the operations
// succeeded.
// Available in version 510
/*FUNC: 161*/ extern BOOL (*IDE_BeautifyWindow)();

// Calls the PL/SQL Beautifier to beautify the text in the S parameter. The result is the
// beautified text or it is empty if the function failed
//
// Available in version 510
/*FUNC: 162*/ extern char *(*IDE_BeautifyText)(char *S);

// This function allows you to do a specific action for the object specified.
// The following actions are available:
// VIEW, VIEWSPECANDBODY, EDIT, EDITSPECANDBODY, EDITDATA,
// QUERYDATA, TEST
// Available in version 514
/*FUNC: 165*/ extern BOOL (*IDE_ObjectAction)(char *Action, char *ObjectType, char *ObjectOwner, char *ObjectName);

// This allows you to start a specific PL/SQL Developer dialog. The
// following are supported:
// AUTHORIZATIONS
// PROJECTITEMS
// BREAKPOINTS
// PREFERENCES
// CONFIG PLUGINS
// CONFIG TOOLS
// CONFIG DOCUMENTS
// CONFIG REPORTS
// CONFIG MACROS
// CONFIG AUTOREFRESH
// The Param parameter is for future use.
// Available in version 700
/*FUNC: 166*/ extern BOOL (*IDE_ShowDialog)(char *Dialog, char *Param);

// When debuggin is on, this function allows you to add messages in the
// debug.txt file generated.
// Available in version 700
/*FUNC: 173*/ extern void (*IDE_DebugLog)(char *Msg);

// This function returns a command-line parameter, or a parameter
// specified in the params.ini file.
// Available in version 700
/*FUNC: 174*/ extern char* (*IDE_GetParamString)(char *Name);

// This function returns a command-line parameter, or a parameter
// specified in the params.ini file.
// Available in version 700
/*FUNC: 175*/ extern BOOL (*IDE_GetParamBool)(char *Name);

// This function allows you to return feedback to the command window. The
// description S will be displayed in the window identified by the
// FeedbackHandle. See the CommandLine Plug-In function for details.
// Available in version 513
/*FUNC: 180*/ extern void (*IDE_CommandFeedback)(int FeedbackHandle, char *S);

// Returns the number of rows in the result grid of a SQL or Test Window.
// Available in version 516
/*FUNC: 190*/ extern int (*IDE_ResultGridRowCount)();

// Returns the number of cols in the result grid of a SQL or Test Window.
// Available in version 516
/*FUNC: 191*/ extern int (*IDE_ResultGridColCount)();

// This function allows you to access the results of a query in a SQL or Test
// Window. Use the above two functions to determine the number of rows
// and cols.
// Available in version 516
/*FUNC: 192*/ extern char* (*IDE_ResultGridCell)(int Col, int Row);

// In PL/SQL Developer 6 we introduced the concept of Authorization. You
// should test if a specific feature is allowed for the current user with this
// function. In the Category parameter you can specify one of the main
// categories (objects, menus, system). The name parameter specifies the
// item (session.kill or objects.drop). Some items have a subname, like
// objects.drop with the different objects.
// Available in version 600
/*FUNC: 200*/ extern BOOL (*IDE_Authorized)(char *Category, char *Name, char *SubName);

// For a quick check if authorization allows the Plug-In to create a specific
// function, you can use this function.
// Available in version 600
/*FUNC: 201*/ extern BOOL (*IDE_WindowAllowed)(int WindowType, BOOL ShowErrorMessage);

// Returns if authorization is enabled or not.
// Available in version 600
/*FUNC: 202*/ extern BOOL (*IDE_Authorization)();

// If you want a list off all available authorization items, you can call this
// function. It will return a cr/lf separated list.
// Available in version 600
/*FUNC: 203*/ extern char* (*IDE_AuthorizationItems)(char *Category);

// If you want to add items to the authorization list to allow them to be
// managed through the authorization option, you can use this function.
// Pass the PlugInID to identify your Plug-In, and pass the Name
// parameter with the item you want to add. The name should be unique,
// so you should prefix it with the name the Plug-In, for example:
// MyPlugIn.Create New Command
// All items will be added in the PlugIns category, so if you want to
// test if this feature is allowed you should call:
// IDE_Authorized('PlugIns ', ' MyPlugIn.Create New Command')
// Available in version 600
/*FUNC: 204*/ extern void (*IDE_AddAuthorizationItem)(int PlugInID, char *Name);

// Returns a list of all personal preference sets.
// If you to have the Plug-In to use different preferences depending on the
// current connection, you can use this function to build a list of possible
// preference sets.
// Available in version 600
/*FUNC: 210*/ extern char* (*IDE_GetPersonalPrefSets)();

// Returns a list of all default preference sets.
// Available in version 600
/*FUNC: 211*/ extern char* (*IDE_GetDefaultPrefSets)();

// Read a Plug-In preference from the preferences. In PL/SQL Developer 6, personal preferences
// are stored in files, not in the registry. You can still use the registry, but if you want to
// store your preferences in a shared location, you can use this function.  Pass the PlugInID
// you received with the IdentifyPlugIn call. The PrefSet parameter can be empty to retrieve
// default preferences, or you can specify one of the existing preference sets.
// Available in version 600
/*FUNC: 212*/ extern BOOL (*IDE_GetPrefAsString)(int PlugInID, char * PrefSet, char *Name, char *Default);

// As IDE_GetPrefAsString, but for integers.
// Available in version 600
/*FUNC: 213*/ extern int (*IDE_GetPrefAsInteger)(int PlugInID, char * PrefSet, char *Name, BOOL Default);

// As IDE_GetPrefAsString, but for booleans.
// Available in version 600
/*FUNC: 214*/ extern BOOL (*IDE_GetPrefAsBool)(int PlugInID, char * PrefSet, char *Name, BOOL Default);

// Set a Plug-In preference. Pass the PlugInID you received with the
// IdentifyPlugIn call. The PrefSet parameter can be empty to set default
// preferences, or you can specify one of the existing preference sets. The
// return value indicates if the function succeeded.
// Available in version 600
/*FUNC: 215*/ extern BOOL (*IDE_SetPrefAsString)(int PlugInID, char *PrefSet, char *Name, char *Value);

// As IDE_SetPrefAsString, but for integers.
// Available in version 600
/*FUNC: 216*/ extern BOOL (*IDE_SetPrefAsInteger)(int PlugInID, char *PrefSet, char *Name, int Value);

// As IDE_SetPrefAsString, but for booleans.
// Available in version 600
/*FUNC: 217*/ extern BOOL (*IDE_SetPrefAsBool)(int PlugInID, char *PrefSet, char *Name, BOOL Value);

// Returns the value of a preference. The names can be found in the
// preference ini file under the [Preferences] section.
// Available in version 700
/*FUNC: 218*/ extern char* (*IDE_GetGeneralPref)(char *Name);

// This will overrule default app behavior.  Currently only the setting "NOFILEDATECHECK" is
// supported where you can pass "TRUE" or "FALSE"
/*FUNC: 219*/ extern BOOL (*IDE_PlugInSetting)(int PlugInID, char *Setting, char *Value);

// Returns the number of overloads for a specific procedure.
// Result < 0 = Procedure doesn`t exist
// Result > 0 = overload count
// Available in version 700
/*FUNC: 220*/ extern int (*IDE_GetProcOverloadCount)(char *Owner, char *PackageName, char *ProcedureName);

// Shows a dialog to allow the user to select an overloaded procedure.
// Result < 0 = Cancel
// Result 0 = No overloadings
// Result > 0 = Overload index
// Available in version 700
/*FUNC: 221*/ extern int (*IDE_SelectProcOverloading)(char *Owner, char *PackageName, char *ProcedureName);

// This function will return one of the Session parameters as you see in the
// grid of the session tool. You will only get a result if the Session Window
// is active, so this will only work from a Popup menu created for the
// SESSIONWINDOW object.
// Available in version 700
/*FUNC: 230*/ extern char* (*IDE_GetSessionValue)(char *Name);

//*****************************************************************************
//
//    SQL functions
//
//****X******************X************************X************************X***

// Executes the statement defined in the SQL parameter. The function returns 0 if successful,
// else the Oracle error number.
/*FUNC: 40*/ extern int (*SQL_Execute)(char *SQL);

// Returns the number of fields after a SQL_Execute.
/*FUNC: 41*/ extern int (*SQL_FieldCount)();

// Returns if there are any more rows to fetch.
/*FUNC: 42*/ extern BOOL (*SQL_Eof)();

// Returns the next row after a SQL_Execute. The function returns 0 if successful, else the
// Oracle error number.
/*FUNC: 43*/ extern int (*SQL_Next)();

// Returns the field specified by the Field parameter.
/*FUNC: 44*/ extern char *(*SQL_Field)(int Field);

// Returns the fieldname specified by the Field parameter.
/*FUNC: 45*/ extern char *(*SQL_FieldName)(int Field);

// Converts a fieldname into an index, which can be used in the SQL_Field, SQL_FieldName and
// SQL_FieldType functions. If the field does not exist, the return value is -1.
/*FUNC: 46*/ extern int (*SQL_FieldIndex)(char *Name);

// Normally, the SQL functions will use the main PL/SQL Developer Oracle
// session. If you want to make sure you don’t interfere with other
// transactions, and you want the PlugIn to use a private session, call this
// function.
// The return value indicates if the function succeeded.
// Available in version 600
/*FUNC: 50*/ extern BOOL (*SQL_UsePlugInSession)(int PlugInID);

// This function will cancel the previous function and set the Oracle session
// back to default.
// Available in version 600
/*FUNC: 51*/ extern void (*SQL_UseDefaultSession)(int PlugInID);

// Forces PL/SQL Developer to check if the current connection to the
// database is still open (and tries a re-connect if necessary). The return
// value indicates if there is a connection.
// Available in version 700
/*FUNC: 52*/ extern BOOL (*SQL_CheckConnection)();

// Returns sys.dbms_output for the current (PlugIn specific) session.
// Available in version 700
/*FUNC: 53*/ extern char* (*SQL_GetDBMSGetOutput)();

// This function declares a variable. Call this for al variables you use in the
// statement you pass in SQL_Execute.
// Available in version 700
/*FUNC: 54*/ extern void (*SQL_SetVariable)(char *Name, char *Value);

// This function will return the value of a variable.
// Available in version 700
/*FUNC: 55*/ extern char* (*SQL_GetVariable)(char *Name);

// Clear all declared variables. If you are finished doing a query it is a good
// idea to call this function to prevent errors for the next execute.
// Available in version 700
/*FUNC: 56*/ extern void (*SQL_ClearVariables)();

// Return the fieldtype of a field.
// 3 = otInteger
// 4 = otFloat
// 5 = otString
// 8 = otLong
// 12 = otDate
// 24 = otLongRaw
/*FUNC: 47*/ extern int (*SQL_FieldType)(int Field);

// This function will return the error message for any error that occurred during:
// SQL_Execute
// SQL_Eof
// SQL_Next
// IDE_SetConnection
// Available in version 301
/*FUNC: 48*/ extern char *(*SQL_ErrorMessage)();

#ifndef _NO_PACK_
#pragma pack(pop)
#endif

#endif   // __PlSqlDevFunctions_h_
