create or replace package OFS_private is
  -- business rules enforcement routines
  procedure change_clear;
  procedure change_verify_row(p_new_id in OFS_ITEMS.id%type, 
    p_new_item_type in OFS_ITEMS.item_type%type, 
    p_old_item_type in OFS_ITEMS.item_type%type, 
    p_old_locked_by in OFS_ITEMS.locked_by%type, 
    p_new_locked_by in OFS_ITEMS.locked_by%type, 
    p_old_read_only in OFS_ITEMS.read_only%type, 
    p_new_read_only in OFS_ITEMS.read_only%type, 
    p_op in char);
  procedure change_register(p_id in OFS_ITEMS.id%type, 
    p_parent_id in OFS_ITEMS.id%type, p_op in char);
  procedure change_verify;
  function os_user return varchar2;
  procedure set_os_user(p_user in varchar2);
  procedure force_unlock(p_id in OFS_ITEMS.id%type);
  
  -- test helper routines
  procedure test_count_recs(p_num in integer, 
    p_ok_msg in varchar2, p_err_msg in varchar2);
  procedure test_count_cursor(p_crs in ofs.dir_cursor,
    p_num in integer, p_ok_msg in varchar2, 
    p_err_msg in varchar2);
  procedure test_error(p_error in varchar2, 
    p_ok_msg in varchar2, p_err_msg in varchar2);
  procedure test_attr(p_path in varchar2, p_attr in varchar2,
    p_val in varchar2, p_ok_msg in varchar2, p_err_msg in varchar2);
end;
/
create or replace package body OFS_private is
  type changed_ID_rec is 
    record (
      id        OFS_ITEMS.id%type,
      parent_id OFS_ITEMS.id%type,
      op        char(1)
    );
  type changed_ID_array is 
    table of changed_ID_rec index by pls_integer;

  changed_IDs    changed_ID_array;
  changed_ID_ind pls_integer;
  g_os_user      OFS_ITEMS.created_by%type;

  procedure change_error(p_msg varchar2, p_id in OFS_ITEMS.id%type, 
                         p_op in char) is
  begin
    raise_application_error(-20000, p_msg || 
      '. Op=' || p_op || ', id=' || to_char(p_id));
  end;

  procedure change_clear is
  begin
    changed_IDs.delete;
    changed_ID_ind := 1;
  end;
  
  procedure change_register(p_id in OFS_ITEMS.id%type, 
                            p_parent_id in OFS_ITEMS.id%type,
                            p_op in char) is
  begin
    changed_IDs(changed_ID_ind).id := p_id;
    changed_IDs(changed_ID_ind).parent_id := p_parent_id;
    changed_IDs(changed_ID_ind).op := p_op;
    changed_ID_ind := changed_ID_ind + 1;
  end;
    
  procedure change_verify is
    parent_item OFS_ITEMS%rowtype;
    n           integer;
  begin
    for i in 1 .. changed_ID_ind - 1 loop
      if changed_IDs(i).id <> 0 then
        begin
          select count(*)
            into n 
            from OFS_ITEMS
            where id <> parent_id
            start with id = changed_IDs(i).id
            connect by prior id <> prior parent_id and id = prior parent_id;
        exception
          when others then
            change_error('OFS-1: Circular directory references not allowed', 
              changed_IDs(i).id, changed_IDs(i).op);
        end;
        begin
          select * 
            into parent_item 
            from OFS_ITEMS
            where id = changed_IDs(i).parent_id;
          if parent_item.item_type <> 'D' then
            change_error('OFS-2: Parent OFS item must be directory', 
              changed_IDs(i).id, changed_IDs(i).op);
          end if;
          if (changed_IDs(i).op in ('I', 'D')) and
             (parent_item.read_only = 'Y') then
            change_error('OFS-3: Parent directory is read-only', 
              changed_IDs(i).id, changed_IDs(i).op);
          end if;
        exception
          when no_data_found then
            if changed_IDs(i).op <> 'D' then
              raise;
            end if;
        end;
      end if;
    end loop;
    change_clear;
  exception
    when others then
      change_clear;
      raise;
  end;

  procedure change_verify_row(p_new_id in OFS_ITEMS.id%type, 
    p_new_item_type in OFS_ITEMS.item_type%type, 
    p_old_item_type in OFS_ITEMS.item_type%type, 
    p_old_locked_by in OFS_ITEMS.locked_by%type, 
    p_new_locked_by in OFS_ITEMS.locked_by%type, 
    p_old_read_only in OFS_ITEMS.read_only%type, 
    p_new_read_only in OFS_ITEMS.read_only%type, 
    p_op in char) is
  begin    
    if p_new_item_type <> p_old_item_type then
      change_error('OFS-4: OFS item type cannot be changed', p_new_id, p_op);
    end if;
    if p_new_item_type = 'F' then
      if (p_old_locked_by is not null) and (p_old_locked_by <> os_user) then
        change_error('OFS-5: File is locked by ' || p_old_locked_by, p_new_id, p_op);
      end if;
      if not (p_old_locked_by is not null and p_new_locked_by is null) and
         (p_old_read_only = p_new_read_only) and (p_new_read_only = 'Y') then
        change_error('OFS-6: File is read-only', p_new_id, p_op);
      end if;
    elsif p_new_item_type = 'D' then
      if p_new_locked_by is not null then
        change_error('OFS-15: Cannot lock directory', p_new_id, p_op);
      end if;
      if (p_old_read_only = p_new_read_only) and (p_new_read_only = 'Y') then
        change_error('OFS-6: Directory is read-only', p_new_id, p_op);
      end if;
    end if;
  end;    

  function os_user return varchar2 is
  begin
    return g_os_user;
  end;
  
  procedure set_os_user(p_user in varchar2) is
  begin
    g_os_user := p_user;
  end;

  procedure force_unlock(p_id in OFS_ITEMS.id%type) is
    prev_usr OFS_ITEMS.locked_by%type;
    usr      OFS_ITEMS.locked_by%type;
  begin
    select locked_by
      into usr
      from ofs_items
      where id = p_id;
    if usr is not null then
      prev_usr := os_user;
      set_os_user(usr);
      begin
        update ofs_items
          set locked_by = null, date_locked = null
          where id = p_id;
        set_os_user(prev_usr);
      exception
        when others then
          set_os_user(prev_usr);
          raise;
      end;
    end if;
  end;
  
  procedure test_count_recs(p_num in integer, 
      p_ok_msg in varchar2, p_err_msg in varchar2) is
    n integer;
  begin
    select count(*)
      into n
      from ofs_items;
    if n <> p_num then
      dbms_output.put_line(substr(p_err_msg || '. Must be ' || 
        to_char(p_num) || ', is ' || to_char(n), 1, 255));
    else
      dbms_output.put_line(substr(p_ok_msg, 1, 255));
    end if;
  end;

  procedure test_count_cursor(p_crs in ofs.dir_cursor,
      p_num in integer, p_ok_msg in varchar2, p_err_msg in varchar2) is
    n integer;
    vitem_type ofs_items.item_type%type;
    vname ofs_items.name%type;
    vdate_created ofs_items.date_created%type;
    vcreated_by ofs_items.created_by%type;
    vdate_modified ofs_items.date_modified%type;
    vmodified_by ofs_items.modified_by%type;
    vread_only ofs_items.read_only%type;
    vcompressed ofs_items.compressed%type;
    vdata_length number;
    vlocked_by ofs_items.locked_by%type;
    vdate_locked ofs_items.date_locked%type;
  begin
    n := 0;
    loop
      fetch p_crs INTO vitem_type, vname, vdate_created, vcreated_by,
        vdate_modified, vmodified_by, vread_only, vcompressed,
        vdata_length, vlocked_by, vdate_locked;
      exit when p_crs%notfound;
      n := n + 1;
    end loop;
    close p_crs;
    if n <> p_num then
      dbms_output.put_line(substr(p_err_msg || '. Must be ' || 
        to_char(p_num) || ', is ' || to_char(n), 1, 255));
    else
      dbms_output.put_line(substr(p_ok_msg, 1, 255));
    end if;
  end;

  procedure test_error(p_error in varchar2, 
      p_ok_msg in varchar2, p_err_msg in varchar2) is
  begin
    if instr(sqlerrm, p_error) <> 0 then
      dbms_output.put_line(substr(p_ok_msg, 1, 255));
    else
      dbms_output.put_line(substr(p_err_msg || '. ' || sqlerrm, 1, 255));
    end if;
  end;  
  
  procedure test_attr(p_path in varchar2, p_attr in varchar2,
      p_val in varchar2, p_ok_msg in varchar2, p_err_msg in varchar2) is
    c char(1);
  begin
    c := ofs.get_attr(p_path, p_attr);
    if c = p_val then
      dbms_output.put_line(substr(p_ok_msg, 1, 255));
    else
      dbms_output.put_line(substr(p_err_msg || '. Must be [' || 
        p_val || '], is [' || c || ']', 1, 255));
    end if;
  end;

begin
  g_os_user := sys_context('USERENV', 'OS_USER');
end;
/
