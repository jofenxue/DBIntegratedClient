@echo off
set ORACLE_HOME=%cd%\instantclient_12_1
set TNS_ADMIN=%cd%\instantclient_12_1
set PATH=%PATH%;%cd%\instantclient_12_1
set NLS_LANG=SIMPLIFIED CHINESE_CHINA.ZHS16GBK
start "" "./PLSQLDeveloper/plsqldev.exe"