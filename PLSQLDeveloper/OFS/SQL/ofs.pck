create or replace package OFS is

  procedure init_root;
  function os_user return varchar2;

  -- Cursor type to return directory entries.
  -- Includes all OFS_ITEMS columns, except id, parent_id, and data.
  -- It also includes dbms_lob.getlength(data) as size.
  type dir_cursor is ref cursor;

  -- Get directory entries.
  -- %param p_path    \dir1\dir2
  -- %param p_mask    hello*.da?[; mask2[; ...]]
  -- %param p_alldirs 'Y' - include all subdirs, 'N' - subdirs according mask
  -- %param p_crs     cursor with the matching directory entries
  procedure get_dir(p_path    in varchar2, 
                    p_mask    in varchar2,
                    p_alldirs in varchar2,
                    p_crs     in out dir_cursor);

  -- Read file data.
  -- %param p_filename \dir1\dir2\file.ext
  function read_file(p_filename in varchar2) return blob;
  
  -- Write file data.
  -- File must be unlocked or locked by current OS user and not read_only.
  -- For new files, parent directory must not be read_only. New files in a
  -- compressed directory are implicitly compressed.
  -- %param p_filename \dir1\dir2\file.ext
  -- %param p_data temporary BLOB with all file data
  procedure write_file(p_filename in varchar2,
                       p_data     in blob);

  -- Create directory.
  -- Parent directory must not be read_only.
  -- %param p_dirname \dir1\dir
  procedure create_dir(p_dirname in varchar2);
  
  -- Delete file or directory.
  -- File must be unlocked or locked by current OS user and not read_only.
  -- Directory must not be read_only.
  -- Parent directory must not be read_only.
  -- %param p_dirname \dir1\dir or \dir1\file.ext
  procedure delete(p_itemname in varchar2);
  
  -- Set file or directory attribute.
  -- File must be unlocked or locked by current OS user.
  -- Compression is ignored on Oracle9.2 or earlier (utl_compress dependency).
  -- %param p_itemname The name of the file or directory
  -- %param p_attrname 'READ_ONLY', 'COMPRESSED' or 'LOCKED'
  -- %param p_value    'Y' or 'N'
  procedure set_attr(p_itemname in varchar2,
                     p_attrname in varchar2,
                     p_value    in varchar2);
  
  -- Get file or directory attribute.
  -- %param p_itemname The name of the file or directory
  -- %param p_attrname 'READ_ONLY', 'COMPRESSED' or 'LOCKED'
  -- %return 'Y' or 'N'
  function get_attr(p_itemname in varchar2,
                    p_attrname in varchar2) return varchar2;

  -- Get file or directory last modification date.
  function get_date(p_itemname in varchar2) return date;

  -- Lock file.
  -- File must be unlocked and not read_only.
  -- %param p_filename \dir1\dir2\file.ext
  procedure lock_file(p_filename in varchar2);
  
  -- Unlock file.
  -- File must be locked by current OS user
  -- %param p_filename \dir1\dir2\file.ext
  procedure unlock_file(p_filename in varchar2);

  -- Rename file or directory.
  -- File must be unlocked or locked by current OS user and not read_only.
  -- %param p_itemname \dir1\dir2\file.ext
  -- %param p_newname  newfile.ext
  procedure rename(p_itemname in varchar2, p_newname in varchar2);
  
  -- Move file or directory.
  -- File must be unlocked or locked by current OS user and not read_only.
  -- %param p_itemname \dir1\dir2\file.ext
  -- %param p_newdir   \dir1\dir2
  procedure move(p_itemname in varchar2, p_newdir in varchar2);

  -- Copy file or directory.
  -- File must be unlocked or locked by current OS user and not read_only.
  -- %param p_itemname \dir1\dir2\file.ext
  -- %param p_newdir   \dir1\dir2
  procedure copy(p_itemname in varchar2, p_newdir in varchar2);

  -- Tests if a specified file exists.
  -- %param p_filename \dir1\dir2\file.ext
  -- %return 'Y' or 'N'
  function file_exists(p_filename in varchar2) return char;

  -- Tests if a specified directory exists.
  -- %param p_path \dir1\dir2
  -- %return 'Y' or 'N'
  function directory_exists(p_path in varchar2) return char;

end OFS;
/
create or replace package body OFS is
  type VC2_array is 
    table of OFS_ITEMS.name%type index by pls_integer;

  -- Set of routines to raise exceptions and check conditions.
  procedure select_error(p_msg varchar2, p_path in varchar2) is
  begin
    raise_application_error(-20000, p_msg || 
      '. path=' || p_path);
  end;

  procedure unknown_attr_error(p_attr varchar2) is
  begin
    raise_application_error(-20000, 'OFS-9: Unknown attribute.' || 
      '. attr=' || p_attr);
  end;

  procedure path_not_found_error(p_path in varchar2) is
  begin
    select_error('OFS-7: Path not found', p_path);
  end;
  
  procedure check_is_file(p_type in OFS_ITEMS.item_type%type, 
                          p_filename in varchar2) is
  begin
    if p_type <> 'F' then
      select_error('OFS-8: OFS item must be file', p_filename);
    end if;
  end;
  
  procedure check_is_directory(p_type in OFS_ITEMS.item_type%type, 
                               p_filename in varchar2) is
  begin
    if p_type <> 'D' then
      select_error('OFS-10: OFS item must be directory', p_filename);
    end if;
  end;

  procedure already_locked_error(p_filename in varchar2) is
  begin
    select_error('OFS-13: File is already locked', p_filename);
  end;
  
  procedure already_unlocked_error(p_filename in varchar2) is
  begin
    select_error('OFS-14: File is already unlocked', p_filename);
  end;

  function unquote_name(p_name in varchar2) return varchar2 is
  begin
    if SubStr(p_name, 1, 1) = '"' and
       SubStr(p_name, Length(p_name), 1) = '"' then
      return SubStr(p_name, 2, Length(p_name) - 2);
    else
      return p_name;
    end if;
  end;
  
  -- Parses path
  -- p_path      The path to parse.
  -- p_raise_exc If True, then raise exception if path is not found.
  --             If False, then function return False if path not found.
  -- p_last_item Returns p_path last OFS item info.
  -- p_no_items  Returns collection of p_path not found names.
  -- Returns     True, if p_path found, else False.
  function path_to_item(p_path in varchar2, 
                        p_raise_exc in boolean,
                        p_last_item out OFS_ITEMS%rowtype,
                        p_no_items out VC2_array) return boolean is
    path        varchar2(1000);
    item_name   OFS_ITEMS.name%type;
    i           pls_integer;
    prev_i      pls_integer;
    j           pls_integer;
    path_exists boolean;
  begin
    path := unquote_name(p_path);
    if path is null or path = '\' then
      path := '\*';
    else
      if SubStr(path, 1, 1) <> '\' then
        path := '\' || path;
      end if;
      if SubStr(path, Length(path), 1) = '\' then
        path := SubStr(path, 1, Length(path) - 1);
      end if;
    end if;
    prev_i := 1;
    path_exists := true;
    j := 1;
    p_last_item.id := 0;
    loop
      i := Instr(path, '\', prev_i + 1);
      if i = 0 then
        item_name := unquote_name(SubStr(path, prev_i + 1, Length(path)));
      else
        item_name := unquote_name(SubStr(path, prev_i + 1, i - (prev_i + 1)));
      end if;
      if path_exists then
        begin
          select *
            into p_last_item
            from OFS_ITEMS
            where parent_id = p_last_item.id and Upper(name) = Upper(item_name);
        exception
          when no_data_found then
            if p_raise_exc then
              path_not_found_error(p_path);
            end if;
            path_exists := false;
        end;
      end if;
      if not path_exists then
        p_no_items(j) := item_name;
        j := j + 1;
      end if;
      exit when i = 0;
      prev_i := i;
    end loop;
    return path_exists;
  end;

  -- Convert DOS file mask into Oracle LIKE mask
  function mask_to_like(p_mask in varchar2) return varchar2 is
    ch  char;
    res varchar2(300);
  begin
    if (p_mask is null) or (p_mask = '*.*') then
      return '%';
    else
      res := '';
      for i in 1 .. Length(p_mask) loop
        ch := SubStr(p_mask, i, 1);
        if ch = '?' then
          res := res || '_';
        elsif ch = '*' then
          res := res || '%';
        elsif (ch = '_') or (ch = '%') or (ch = '\') then
          res := res || '\' || ch;
        else
          res := res || ch;
        end if;
      end loop;
      return res;
    end if;
  end;

  -- Get directory entries.
  -- %param p_path    \dir1\dir2
  -- %param p_mask    hello*.da?[; mask2[; ...]]
  -- %param p_alldirs 'Y' - include all subdirs, 'N' - subdirs according mask
  -- %param p_crs     cursor with the matching directory entries
  procedure get_dir(p_path    in varchar2, 
                    p_mask    in varchar2,
                    p_alldirs in varchar2,
                    p_crs     in out dir_cursor) is
    item       OFS_ITEMS%rowtype;
    no_items   VC2_array;
    ora_mask   varchar2(300);
    dir_q      varchar2(1000);
    i          pls_integer;
    prev_i     pls_integer;
    first_mask boolean;
  begin
    if not path_to_item(p_path, true, item, no_items) then
      return;
    end if;
    check_is_directory(item.item_type, p_path);
    dir_q := 
      'select ITEM_TYPE, NAME, DATE_CREATED, CREATED_BY,' ||
      ' DATE_MODIFIED, MODIFIED_BY, READ_ONLY, COMPRESSED,' ||
      ' dbms_lob.getlength(DATA) as DATA_LENGTH, LOCKED_BY, DATE_LOCKED ' ||
      'from OFS_ITEMS i ' ||
      'where PARENT_ID = :PID and PARENT_ID <> ID';
    prev_i := 0;
    first_mask := true;
    loop
      i := Instr(p_mask, ';', prev_i + 1);
      if i = 0 then
        ora_mask := SubStr(p_mask, prev_i + 1, Length(p_mask));
      else
        ora_mask := SubStr(p_mask, prev_i + 1, i - (prev_i + 1));
      end if;
      if Length(ora_mask) <> 0 then
        ora_mask := Upper(mask_to_like(ora_mask));
        if first_mask then
          dir_q := dir_q || ' and (';
          first_mask := false;
        else
          dir_q := dir_q || ' or ';
        end if;
        dir_q := dir_q || 'Upper(NAME) like ''' || ora_mask || ''' escape ''\''';
      end if;
      exit when i = 0;
      prev_i := i;
    end loop;
    if not first_mask and p_alldirs = 'Y' then
      dir_q := dir_q || ' or ITEM_TYPE = ''D''';
    end if;
    if not first_mask then
      dir_q := dir_q || ')';
    end if;
    dir_q := dir_q || ' order by ITEM_TYPE, NAME';
    open p_crs for dir_q using item.id;
  end;                   

  -- Read file data.
  -- %param p_filename \dir1\dir2\file.ext
  function read_file(p_filename in varchar2) return blob is
    item     OFS_ITEMS%rowtype;
    no_items VC2_array;
  begin
    if path_to_item(p_filename, true, item, no_items) then
      check_is_file(item.item_type, p_filename);
      if ofs_compress.is_active and item.compressed = 'Y' then
        return ofs_compress.uncomp(item.data);
      else
        return item.data;
      end if;
    else
      return null;
    end if;
  end;
  
  -- Write file data.
  -- File must be unlocked or locked by current OS user and not read_only.
  -- For new files, parent directory must not be read_only. New files in a
  -- compressed directory are implicitly compressed.
  -- %param p_filename \dir1\dir2\file.ext
  -- %param p_data temporary BLOB with all file data
  procedure write_file(p_filename in varchar2,
                       p_data     in blob) is
    item     OFS_ITEMS%rowtype;
    no_items VC2_array;
  begin
    if path_to_item(p_filename, false, item, no_items) then
      check_is_file(item.item_type, p_filename);
      update OFS_ITEMS
        set modified_by = OFS_private.os_user
        where id = item.id;
    else
      if no_items.count > 1 then
        path_not_found_error(p_filename);
      end if;
      insert into OFS_ITEMS (parent_id, item_type, name, data) 
        values (item.id, 'F', no_items(1), empty_blob())
        returning data into item.data;
    end if;
    if ofs_compress.is_active and item.compressed = 'Y' then
      ofs_compress.comp(p_data, item.data);
    else
      dbms_lob.trim(item.data, 0);
      dbms_lob.append(item.data, p_data);
    end if;
  end;

  -- Create directory.
  -- Parent directory must not be read_only.
  -- %param p_dirname \dir1\dir
  procedure create_dir(p_dirname in varchar2) is
    item     OFS_ITEMS%rowtype;
    no_items VC2_array;
  begin
    if path_to_item(p_dirname, false, item, no_items) then
      check_is_directory(item.item_type, p_dirname);
      select_error('OFS-11: Specified directory already exists', p_dirname);
    else
      for i in 1 .. no_items.count loop
        insert into OFS_ITEMS (parent_id, item_type, name)
          values (item.id, 'D', no_items(i))
          returning id into item.id;
      end loop;
    end if;
  end;
  
  -- Delete file or directory.
  -- File must be unlocked or locked by current OS user and not read_only.
  -- Directory must not be read_only.
  -- Parent directory must not be read_only.
  -- %param p_dirname \dir1\dir or \dir1\file.ext
  procedure delete(p_itemname in varchar2) is
    item     OFS_ITEMS%rowtype;
    no_items VC2_array;
  begin
    if path_to_item(p_itemname, true, item, no_items) then
      delete from OFS_ITEMS
        where id = item.id;
    end if;
  end;
  
  -- Set file or directory attribute.
  -- File must be unlocked or locked by current OS user.
  -- Compression is ignored on Oracle9.2 or earlier (utl_compress dependency).
  -- %param p_itemname The name of the file or directory
  -- %param p_attrname 'READ_ONLY', 'COMPRESSED' or 'LOCKED'
  -- %param p_value    'Y' or 'N'
  procedure set_attr(p_itemname in varchar2,
                     p_attrname in varchar2,
                     p_value    in varchar2) is
    item     OFS_ITEMS%rowtype;
    no_items VC2_array;
    attrname varchar2(20);
    val      char(1);
    temp_lob blob;
  begin
    if path_to_item(p_itemname, true, item, no_items) then
      attrname := Upper(p_attrname);
      val := Upper(p_value);
      if attrname = 'READ_ONLY' then
        if item.read_only <> val then
          update OFS_ITEMS
            set read_only = val
            where id = item.id;
        end if;
      elsif attrname = 'COMPRESSED' then
        if item.compressed <> val then
          update OFS_ITEMS
            set compressed = val
            where id = item.id;
          if ofs_compress.is_active then
            dbms_lob.createtemporary(temp_lob, false);
            begin
              dbms_lob.append(temp_lob, item.data);
              if val = 'Y' then
                ofs_compress.comp(temp_lob, item.data);
              else
                ofs_compress.uncomp(temp_lob, item.data);
              end if;
            exception when others then
              dbms_lob.freetemporary(temp_lob);
              raise;
            end;
            dbms_lob.freetemporary(temp_lob);
          end if;
        end if;
      elsif attrname = 'LOCKED' then
        check_is_file(item.item_type, p_itemname);
        if val = 'Y' then
          if Upper(item.locked_by) = Upper(OFS_private.os_user) then
            already_locked_error(p_itemname);
          end if;
          update OFS_ITEMS
            set locked_by = OFS_private.os_user
            where id = item.id;
        elsif val = 'N' then
          if item.locked_by is null then
            already_unlocked_error(p_itemname);
          end if;
          update OFS_ITEMS
            set locked_by = null
            where id = item.id;
        end if;
      else
        unknown_attr_error(p_attrname);
      end if;
    end if;
  end;
  
  -- Get file or directory attribute.
  -- %param p_itemname The name of the file or directory
  -- %param p_attrname 'READ_ONLY', 'COMPRESSED' or 'LOCKED'
  -- %return 'Y' or 'N'
  function get_attr(p_itemname in varchar2,
                    p_attrname in varchar2) return varchar2 is
    item     OFS_ITEMS%rowtype;
    no_items VC2_array;
    attrname varchar2(20);
  begin
    if path_to_item(p_itemname, true, item, no_items) then
      attrname := Upper(p_attrname);
      if attrname = 'READ_ONLY' then
        return item.read_only;
      elsif attrname = 'COMPRESSED' then
        return item.compressed;
      elsif attrname = 'LOCKED' then
        if item.locked_by is not null then
          return 'Y';
        else
          return 'N';
        end if;
      elsif attrname = 'ID' then
        return to_char(item.id);
      else
        unknown_attr_error(p_attrname);
      end if;
    else
      return null;
    end if;
  end;

  -- Get file or directory last modification date.
  function get_date(p_itemname in varchar2) return date is
    item     OFS_ITEMS%rowtype;
    no_items VC2_array;
  begin
    if path_to_item(p_itemname, true, item, no_items) then
      return item.date_modified;
    else
      return null;
    end if;
  end;

  -- Lock file.
  -- File must be unlocked and not read_only.
  -- %param p_filename \dir1\dir2\file.ext
  procedure lock_file(p_filename in varchar2) is
  begin
    set_attr(p_filename, 'LOCKED', 'Y');
  end;
  
  -- Unlock file.
  -- File must be locked by current OS user
  -- %param p_filename \dir1\dir2\file.ext
  procedure unlock_file(p_filename in varchar2) is
  begin
    set_attr(p_filename, 'LOCKED', 'N');
  end;

  -- Rename file or directory.
  -- File must be unlocked or locked by current OS user and not read_only.
  -- %param p_itemname \dir1\dir2\file.ext
  -- %param p_newname  newfile.ext
  procedure rename(p_itemname in varchar2, p_newname in varchar2) is
    item      OFS_ITEMS%rowtype;
    no_items  VC2_array;
    item_name OFS_ITEMS.NAME%type;
  begin
    item_name := unquote_name(p_newname);
    if Instr(item_name, '\') <> 0 then
      select_error('OFS-12: New name must be without path', p_newname);
    end if;
    if path_to_item(p_itemname, true, item, no_items) then
      update OFS_ITEMS
        set name = item_name
        where id = item.id;
    end if;
  end;
  
  -- Move file or directory.
  -- File must be unlocked or locked by current OS user and not read_only.
  -- %param p_itemname \dir1\dir2\file.ext
  -- %param p_newdir   \dir1\dir2
  procedure move(p_itemname in varchar2, p_newdir in varchar2) is
    item1     OFS_ITEMS%rowtype;
    no_items1 VC2_array;
    item2     OFS_ITEMS%rowtype;
    no_items2 VC2_array;
  begin
    if path_to_item(p_itemname, true, item1, no_items1) and
       path_to_item(p_newdir, true, item2, no_items2) then
      update OFS_ITEMS
        set parent_id = item2.id
        where id = item1.id;
    end if;
  end;
    
  -- Copy file or directory.
  -- File must be unlocked or locked by current OS user and not read_only.
  -- %param p_itemname \dir1\dir2\file.ext
  -- %param p_newdir   \dir1\dir2
  procedure copy(p_itemname in varchar2, p_newdir in varchar2) is
    item1     OFS_ITEMS%rowtype;
    no_items1 VC2_array;
    item2     OFS_ITEMS%rowtype;
    no_items2 VC2_array;
  begin
    if path_to_item(p_itemname, true, item1, no_items1) and
       path_to_item(p_newdir, true, item2, no_items2) then
      if item1.item_type = 'D' then 
        create_dir(p_newdir || '\' || item1.name);
        for c in (select name from OFS_ITEMS where parent_id = item1.id) loop
          copy(p_itemname || '\' || c.name, p_newdir || '\' || item1.name);
        end loop;
      else
        write_file(p_newdir || '\' || item1.name, item1.data);
      end if;
    end if;
  end;
  
  -- Tests if a specified file exists.
  -- %param p_filename \dir1\dir2\file.ext
  function file_exists(p_filename in varchar2) return char is
    item     OFS_ITEMS%rowtype;
    no_items VC2_array;
  begin
    if path_to_item(p_filename, false, item, no_items) and
       (item.item_type = 'F') then
      return 'Y';
    else
      return 'N';
    end if;
  end;

  -- Tests if a specified directory exists.
  -- %param p_path \dir1\dir2
  function directory_exists(p_path in varchar2) return char is
    item     OFS_ITEMS%rowtype;
    no_items VC2_array;
  begin
    if path_to_item(p_path, false, item, no_items) and
       (item.item_type = 'D') then
      return 'Y';
    else
      return 'N';
    end if;
  end;

  procedure init_root is
    i OFS_ITEMS.id%type;
  begin
    select id
      into i
      from OFS_ITEMS
      where id = 0;
  exception
    when others then
      insert into OFS_ITEMS (id, parent_id, item_type, name)
        values (0, 0, 'D', '*');
  end;
  
  function os_user return varchar2 is
  begin
    return ofs_private.os_user;
  end;
  
end OFS;
/
