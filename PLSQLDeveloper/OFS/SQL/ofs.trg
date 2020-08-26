create or replace trigger TRG_OFS_ITEMS_BIUD
  before insert or update or delete on OFS_ITEMS  
begin
  OFS_private.change_clear;
end;
/
create or replace trigger TRG_OFS_ITEMS_BIUDR
  before insert or update or delete on OFS_ITEMS  
  for each row
begin
  if inserting then
    if :new.id is null then
      select OFS_ID_SEQ.nextval into :new.id from dual;
    end if;
    :new.created_by := OFS_private.os_user;
    :new.date_created := sysdate;
    :new.modified_by := :new.created_by;
    :new.date_modified := :new.date_created;
    OFS_private.change_register(:new.id, :new.parent_id, 'I');
  elsif updating then
    :new.modified_by := OFS_private.os_user;
    :new.date_modified := sysdate;
    OFS_private.change_verify_row(:new.id, :new.item_type, :old.item_type,
      :old.locked_by, :new.locked_by, :old.read_only, :new.read_only, 'U');
    OFS_private.change_register(:new.id, :new.parent_id, 'U');
  elsif deleting then
    OFS_private.change_verify_row(:old.id, :old.item_type, :old.item_type,
      :old.locked_by, :old.locked_by, :old.read_only, :old.read_only, 'D');
    OFS_private.change_register(:old.id, :old.parent_id, 'D');
  end if;
  if not deleting then
    if :old.locked_by is null and :new.locked_by is not null then
      :new.date_locked := sysdate;
    elsif :old.locked_by is not null and :new.locked_by is null then
      :new.date_locked := null;
    end if;
  end if;
end;
/
create or replace trigger TRG_OFS_ITEMS_AIUD
  after insert or update or delete on OFS_ITEMS  
begin
  OFS_private.change_verify;
end;
/
