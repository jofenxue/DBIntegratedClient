
[General]
Version=1

[Preferences]
Username=
Password=2038
Database=
DateFormat=dd-mm-yyyy
CommitCount=100
CommitDelay=0
InitScript=create table DEPTDEMO
=(
=  DEPTNO NUMBER(2) not null,
=  DNAME  VARCHAR2(14),
=  LOC    VARCHAR2(13)
=);
=create table EMPDEMO
=(
=  EMPNO    NUMBER(4) not null,
=  ENAME    VARCHAR2(10),
=  JOB      VARCHAR2(9),
=  MGR      NUMBER(4),
=  HIREDATE DATE,
=  SAL      NUMBER(7,2),
=  COMM     NUMBER(7,2),
=  DEPTNO   NUMBER(2)
=);
=truncate table deptdemo;
=truncate table empdemo;

[Table]
Owner=
Name=DEPTDEMO
Count=5

[Record]
Name=DEPTNO
Type=NUMBER
Size=20
Data=sequence(10, 10)
Master=

[Record]
Name=DNAME
Type=VARCHAR2
Size=14
Data=[A(3..5)]
Master=

[Record]
Name=LOC
Type=VARCHAR2
Size=13
Data=UpperCase(City)
Master=


[Table]
Owner=
Name=EMPDEMO
Count=2..30

[Record]
Name=EMPNO
Type=NUMBER
Size=40
Data=random(4000..8000)
Master=

[Record]
Name=ENAME
Type=VARCHAR2
Size=10
Data=UpperCase(lastname)
Master=

[Record]
Name=JOB
Type=VARCHAR2
Size=9
Data=list('CLERK'(50), 'SALESMAN'(30), 'MANAGER'(10), 'ANALYST'(30), 'PRESIDENT'(2))
Master=

[Record]
Name=MGR
Type=NUMBER
Size=40
Data=random(4000..8000)
Master=

[Record]
Name=HIREDATE
Type=DATE
Size=
Data=random(01-01-1970..01-01-2002)
Master=

[Record]
Name=SAL
Type=NUMBER
Size=7,2
Data=random(1000..8000)
Master=

[Record]
Name=COMM
Type=NUMBER
Size=7,2
Data=list('', '', '', '', 100, 500, 2000)
Master=

[Record]
Name=DEPTNO
Type=NUMBER
Size=20
Data=
Master=DEPTDEMO.DEPTNO

