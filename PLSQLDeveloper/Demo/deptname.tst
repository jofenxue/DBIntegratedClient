PL/SQL Developer Test script 2.0
5
begin
  -- Demo test script to test the deptname function
  -- Requires that employee.pck is compiled
  :result := employee.deptname(p_empno => :p_empno);
end;
2
result
RESEARCH      
1
p_empno
7788
2
