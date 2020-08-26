PL/SQL Developer Test script 2.0
5
begin
  -- Demo test script to test the mgrname function
  -- Requires that employee.pck is compiled
  :result := employee.mgrname(p_empno => :p_empno);
end;
2
result
JONES     
1
p_empno
7788
2
