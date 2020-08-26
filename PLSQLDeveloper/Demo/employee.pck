create or replace package employee is

  -- Demo employee package for the dept/emp tables that are
  -- provided with Oracle. You may need to create these 
  -- tables before compiling this package. Just run the 
  -- following command in SQL*Plus or the Command Window:
  --
  -- Windows NT: 
  --   @c:\orant\dbs\demobld.sql
  -- Windows 95: 
  --   @c:\orawin95\dbs\demobld.sql

  -- Return the name of the department
  function deptname(p_empno in emp.empno%type) return dept.dname%type;

  -- Return the name of the manager
  function mgrname(p_empno in emp.empno%type) return emp.ename%type;

  -- Declare purity of these functions, so that they can be used
  -- in SQL statements
  pragma restrict_references (deptname, wnds);
  pragma restrict_references (mgrname, wnds);

end;
/
create or replace package body employee is

  -- Return the name of the department
  -- If you get a PLS-00452 compilation error here, just remove
  -- the calls to dbms_output.put_line
  function deptname(p_empno in emp.empno%type) return dept.dname%type is
    result dept.dname%type;
  begin
    if p_empno is null 
    then
      -- If empno is null, return an empty name
      result := null;
    else
      -- Fetch the name of the department
      select dept.dname into result
        from dept, emp
       where emp.empno = p_empno
         and dept.deptno = emp.deptno;
      -- Write some output
      dbms_output.put_line(to_char(sysdate, 'dd-mm-yyyy hh24:mi:ss') ||
                           ' - Department ' || rtrim(result) || ' fetched for ' ||
                           'employee ' || to_char(p_empno));
    end if;
    return(result);
  exception
    -- If the employee does not exist, return an empty name
    when no_data_found then return(null);
  end;
  
  -- Return the name of the manager
  function mgrname(p_empno in emp.empno%type) return emp.ename%type is
    result emp.ename%type;
  begin
    if p_empno is null then
      -- If empno is null, return an empty name
      result := null;
    else
      -- Fetch the name of the manager
      select m.ename into result
        from emp e, emp m
       where e.empno = p_empno
         and m.empno = e.mgr;
      -- Write some output
      dbms_output.put_line(to_char(sysdate, 'dd-mm-yyyy hh24:mi:ss') ||
                           ' - Manager ' || rtrim(result) || ' fetched for ' ||
                           'employee ' || to_char(p_empno));
    end if;
    return(result);
  exception
    when no_data_found then return(null);
  end;

end;
/
