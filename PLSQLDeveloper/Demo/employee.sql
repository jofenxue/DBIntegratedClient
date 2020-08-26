-- Demo SQL script, requires that employee.pck is compiled
select emp.*,
       employee.deptname(empno) deptname,
       employee.mgrname(empno) mgrname
from emp
order by deptno, ename

