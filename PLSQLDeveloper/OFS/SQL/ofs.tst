PL/SQL Developer Test script 3.0
315
declare
  c ofs.dir_cursor;
  b blob;
  d raw(100);
  i binary_integer;
begin
  execute immediate 'truncate table ofs_items';
  ofs_private.set_os_user(sys_context('USERENV', 'OS_USER'));
  ofs.init_root;
  ---------------------------------------------------------
  -- Directories
  ---------------------------------------------------------
  -- create_dir
  ofs.create_dir('d1');
  ofs.create_dir('d1\d1-1');
  ofs.create_dir('d2');
  ofs.create_dir('d2\d2-1');
  ofs.create_dir('d2\d2-2\d2-2-1');
  ofs.create_dir('d2\d2-2\d2-2-2');
  -- dup 
  begin
    ofs.create_dir('d2\d2-2\d2-2-2');
    dbms_output.put_line('1.1. Dup dir failed. No exception.');
  exception
    when others then
      ofs_private.test_error('OFS-11', '1.1. Dup dir - passed', 
        '1.1. Dup dir failed');
  end;
  -- circular
  begin
    update ofs_items 
      set parent_id = (
        select id 
        from ofs_items
        where name = 'd2-2-2'
      )
      where name = 'd2';
    dbms_output.put_line('1.2. Circular dir failed. No exception.');
  exception
    when others then
      ofs_private.test_error('OFS-1', '1.2. Circular dir - passed', 
        '1.2. Circular dir failed');
  end;
  -- change type
  begin
    update ofs_items 
      set item_type = 'F'
      where name = 'd2';
    dbms_output.put_line('1.3. Change type D->F failed. No exception.');
  exception
    when others then
      ofs_private.test_error('OFS-4', '1.3. Change type D->F - passed', 
        '1.3. Change type D->F failed');
  end;
  -- dir 
  ofs_private.test_count_recs(8, '1.4. Dir num - passed', 
    '1.4. Wrong number of dirs');
  -- non-exist
  begin
    OFS.get_dir('d2\d2-3', '*', 'N', c);  
    dbms_output.put_line('1.5. Get non-exist dir. No exception.');
  exception
    when others then
      ofs_private.test_error('OFS-7', '1.5. Get non-exist dir - passed', 
        '1.5. Get non-exist dir failed');
  end;
  ---------------------------------------------------------
  -- get_dir root
  OFS.get_dir('*', '*', 'N', c);
  ofs_private.test_count_cursor(c, 2, '2.1. get_dir root num - passed',
    '2.1. get_dir root wrong number');    
  -- get_dir sub
  OFS.get_dir('d2\d2-2', '*', 'N', c);
  ofs_private.test_count_cursor(c, 2, '2.2. get_dir sub num - passed',
    '2.2. get_dir sub wrong number');    
  ---------------------------------------------------------
  -- delete 
  ofs.delete('d2\d2-2');
  ofs_private.test_count_recs(5, '3.1. Delete num - passed', 
    '3.1. Wrong number of dirs');
  ---------------------------------------------------------
  -- rename 
  OFS.get_dir('d2', 'd2-1', 'N', c);
  ofs_private.test_count_cursor(c, 1, '4.0. Rename init num - passed',
    '4.0. Rename init wrong number');    
  ofs.rename('d2\d2-1', 'd2-1-new');
  OFS.get_dir('d2', 'd2-1', 'N', c);
  ofs_private.test_count_cursor(c, 0, '4.1. Rename old num - passed',
    '4.1. Rename old wrong number');    
  OFS.get_dir('d2', 'd2-1-new', 'N', c);
  ofs_private.test_count_cursor(c, 1, '4.2. Rename new num - passed',
    '4.2. Rename new wrong number');    
  ---------------------------------------------------------
  -- move
  ofs.move('d2\d2-1-new', 'd1');
  OFS.get_dir('d2', '*', 'N', c);
  ofs_private.test_count_cursor(c, 0, '5.1. Move old num - passed',
    '5.1. Move old wrong number');    
  OFS.get_dir('d1', 'd2-1-new', 'N', c);
  ofs_private.test_count_cursor(c, 1, '5.2. Move new num - passed',
    '5.2. Move new wrong number');    
  ---------------------------------------------------------
  -- read_only
  execute immediate 'truncate table ofs_items';
  ofs.init_root;
  ofs.create_dir('d1');
  ofs.create_dir('d1\d1-1');
  ofs.create_dir('d2');
  ofs.create_dir('d2\d2-1');
  -- get/set ro
  ofs_private.test_attr('d2\d2-1', 'read_only', 'N', '6.1. attr init ro - passed',
    '6.1. attr init ro failed');
  ofs.set_attr('d2\d2-1', 'read_only', 'y');
  ofs_private.test_attr('d2\d2-1', 'read_only', 'Y', '6.2. attr set ro - passed',
    '6.2. attr set ro failed');
  -- create subdir in ro dir
  begin  
    ofs.create_dir('d2\d2-1\d2-1-1');
    dbms_output.put_line('6.3. Dir ro, cant create failed. No exception.');
  exception
    when others then
      ofs_private.test_error('OFS-3', '6.3. Dir ro, cant create - passed', 
        '6.3. Dir ro, cant create failed');
  end;
  -- rename ro dir
  begin  
    ofs.rename('d2\d2-1', 'd2-1-new');
    dbms_output.put_line('6.4. Rename ro dir failed. No exception.');
  exception
    when others then
      ofs_private.test_error('OFS-6', '6.4. Rename ro dir - passed', 
        '6.4. Rename ro dir failed');
  end;
  -- delete ro dir
  begin  
    ofs.delete('d2\d2-1');
    dbms_output.put_line('6.5. Delete ro dir failed. No exception.');
  exception
    when others then
      ofs_private.test_error('OFS-6', '6.5. Delete ro dir - passed', 
        '6.5. Delete ro dir failed');
  end;
  ---------------------------------------------------------
  -- Files
  ---------------------------------------------------------
  dbms_lob.createtemporary(b, false);
  dbms_lob.write(b, 10, 1, 'AABBCCDDEEFFAABBCCDD');
  -- write to dir
  begin  
    ofs.write_file('d1\d1-1', b);
    dbms_output.put_line('7.1. Write to dir failed. No exception.');
  exception
    when others then
      ofs_private.test_error('OFS-8', '7.1. Write to dir - passed', 
        '7.1. Write to dir failed');
  end;
  -- write to file in ro dir
  begin  
    ofs.write_file('d2\d2-1\data.db', b);
    dbms_output.put_line('7.2. Write file to ro dir failed. No exception.');
  exception
    when others then
      ofs_private.test_error('OFS-3', '7.2. Write file to ro dir - passed', 
        '7.2. Write file to ro dir failed');
  end;
  -- write to file in non-exist dir
  begin  
    ofs.write_file('d2\d2-1\qwe\data.db', b);
    dbms_output.put_line('7.3. Write file to non-exist dir failed. No exception.');
  exception
    when others then
      ofs_private.test_error('OFS-7', '7.3. Write file to non-exist dir - passed', 
        '7.3. Write file to non-exist dir failed');
  end;
  ---------------------------------------------------------
  -- write to file 
  ofs.write_file('d1\d1-1\data.db', b);
  OFS.get_dir('d1\d1-1', 'data.db', 'N', c);
  ofs_private.test_count_cursor(c, 1, '8.1. create file - passed',
    '8.1. create file failed');    
  -- read file 
  b := ofs.read_file('d1\d1-1\data.db');
  i := 100;
  dbms_lob.read(b, i, 1, d);
  if (i <> 10) or (d <> 'AABBCCDDEEFFAABBCCDD') then
    dbms_output.put_line('8.2. read file failed');
  else
    dbms_output.put_line('8.2. read file - passed');
  end if;
  -- update file in ro dir
  begin  
    ofs.set_attr('d1\d1-1', 'read_only', 'y');
    ofs.write_file('d1\d1-1\data.db', b);
    dbms_output.put_line('8.3. Update file in ro dir - passed');
  exception
    when others then
      dbms_output.put_line('8.3. Update file in ro dir failed. ' || sqlerrm);
  end;
  -- delete file from ro dir
  begin  
    ofs.delete('d1\d1-1\data.db');
    dbms_output.put_line('8.4. Delete file from ro dir failed. No exception.');
    return;
  exception
    when others then
      ofs_private.test_error('OFS-3', '8.4. Delete file from ro dir - passed', 
        '8.4. Delete file from ro dir failed');
  end;
  ofs.set_attr('d1\d1-1', 'read_only', 'n');
  -- create subobj of file
  begin  
    ofs.create_dir('d1\d1-1\data.db\qwe');
    dbms_output.put_line('8.5. Create subdir of file failed. No exception.');
  exception
    when others then
      ofs_private.test_error('OFS-2', '8.5. Create subdir of file - passed', 
        '8.5. Create subdir of file failed');
  end;
  ---------------------------------------------------------
  -- locking
  ofs.lock_file('d1\d1-1\data.db');
  -- write by me
  begin  
    ofs.write_file('d1\d1-1\data.db', b);
    dbms_output.put_line('9.0. Write locked by me file - passed');
  exception
    when others then
      dbms_output.put_line('9.0. Write locked by me file failed. ' || sqlerrm);
  end;
  ofs_private.set_os_user('who');
  -- write by other
  begin  
    ofs.write_file('d1\d1-1\data.db', b);
    dbms_output.put_line('9.1. Write locked by other file failed. No exception.');
  exception
    when others then
      ofs_private.test_error('OFS-5', '9.1. Write locked by other file - passed', 
        '9.1. Write locked file by other failed');
  end;
  -- delete by other
  begin  
    ofs.delete('d1\d1-1\data.db');
    dbms_output.put_line('9.2. Delete locked by other file failed. No exception.');
    return;
  exception
    when others then
      ofs_private.test_error('OFS-5', '9.2. Delete locked by other file - passed', 
        '9.2. Delete locked file by other failed');
  end;
  -- unlock by other
  begin  
    ofs.unlock_file('d1\d1-1\data.db');
    dbms_output.put_line('9.3. Unlock locked by other file failed. No exception.');
  exception
    when others then
      ofs_private.test_error('OFS-5', '9.3. Unlock locked by other file - passed', 
        '9.3. Unlock locked by other file failed');
  end;
  -- set_attr by other
  begin  
    ofs.set_attr('d1\d1-1\data.db', 'read_only', 'y');
    dbms_output.put_line('9.4. Set_attr locked by other file failed. No exception.');
  exception
    when others then
      ofs_private.test_error('OFS-5', '9.4. Set_attr locked by other file - passed', 
        '9.4. Set_attr locked by other file failed');
  end;
  -- rename ro dir
  begin  
    ofs.rename('d1\d1-1\data.db', 'datanew.db');
    dbms_output.put_line('9.5. Rename locked by other file failed. No exception.');
    return;
  exception
    when others then
      ofs_private.test_error('OFS-5', '9.5. Rename locked by other file - passed', 
        '9.5. Rename locked by other file failed');
  end;
  -- unlock ro file
  ofs_private.set_os_user(sys_context('USERENV', 'OS_USER'));
  ofs.set_attr('d1\d1-1\data.db', 'read_only', 'y');
  begin  
    ofs.unlock_file('d1\d1-1\data.db');
    dbms_output.put_line('9.6. Unlock ro file - passed');
  exception
    when others then
      dbms_output.put_line('9.6. Unlock ro file failed. ' || sqlerrm);
  end;
  commit;
  ---------------------------------------------------------
  -- demo data
  execute immediate 'truncate table ofs_items';
  dbms_lob.createtemporary(b, false);
  dbms_lob.write(b, 10, 1, 'AABBCCDDEEFFAABBCCDD');
  ofs.init_root;
  ofs.create_dir('Program Files');
  ofs.create_dir('Program Files\Borland');
  ofs.write_file('Program Files\Borland\dcc32.exe', b);
  ofs.write_file('Program Files\Borland\install.txt', b);
  ofs.write_file('Program Files\Borland\readme.txt', b);
  ofs.create_dir('Program Files\Visio');
  ofs.write_file('Program Files\Visio\Visio32.exe', b);
  ofs.write_file('Program Files\Visio\Readme.wri', b);
  ofs.write_file('Program Files\Visio\Readme Pro.wri', b);
  ofs.write_file('Program Files\Visio\Filelist.wri', b);
  ofs.create_dir('My Documents');
  ofs.write_file('My Documents\ToDo.txt', b);
  ofs.write_file('My Documents\MyInfo.txt', b);
  ofs.create_dir('My Documents\Reports');
  ofs.write_file('My Documents\Reports\2005-Q1.rpt', b);
  ofs.write_file('My Documents\Reports\2005-Q2.rpt', b);
  ofs.write_file('My Documents\Reports\2005-Q3.rpt', b);
  ofs.write_file('My Documents\Reports\2005-Q4.rpt', b);
  dbms_output.put_line('Demo data created');
  commit;
end;
0
0
