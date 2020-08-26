prompt dropping table OFS_ITEMS
drop table OFS_ITEMS;
prompt dropping sequence OFS_ID_SEQ
drop sequence OFS_ID_SEQ;

-- Create table
create table OFS_ITEMS
(
  ID            INTEGER not null,
  PARENT_ID     INTEGER not null,
  ITEM_TYPE     VARCHAR2(1) not null,
  NAME          VARCHAR2(256) not null,
  DATE_CREATED  DATE default sysdate not null,
  CREATED_BY    VARCHAR2(256) not null,
  DATE_MODIFIED DATE default sysdate not null,
  MODIFIED_BY   VARCHAR2(256) not null,
  READ_ONLY     VARCHAR2(1) default 'N' not null,
  COMPRESSED    VARCHAR2(1) default 'N' not null,
  DATA          BLOB,
  LOCKED_BY     VARCHAR2(256),
  DATE_LOCKED   DATE
);

-- Add comments to the columns 
comment on column OFS_ITEMS.ID
  is 'The id of the item (0 = root)';
comment on column OFS_ITEMS.PARENT_ID
  is 'The id of the parent item (0 = root)';
comment on column OFS_ITEMS.ITEM_TYPE
  is 'Item type: D=Directory, F=File';
comment on column OFS_ITEMS.NAME
  is 'The name of the item';
comment on column OFS_ITEMS.DATE_CREATED
  is 'Creation date';
comment on column OFS_ITEMS.CREATED_BY
  is 'OS user that created the item';
comment on column OFS_ITEMS.DATE_MODIFIED
  is 'Modification date';
comment on column OFS_ITEMS.MODIFIED_BY
  is 'OS user that modified the item';
comment on column OFS_ITEMS.READ_ONLY
  is 'Read only: Y/N';
comment on column OFS_ITEMS.COMPRESSED
  is 'Compressed: Y/N (Oracle10g only)';
comment on column OFS_ITEMS.DATA
  is 'The file data';
comment on column OFS_ITEMS.LOCKED_BY
  is 'OS user that has locked the file';
comment on column OFS_ITEMS.DATE_LOCKED
  is 'Date that the file was locked';

-- Create/Recreate primary, unique and foreign key constraints 
alter table OFS_ITEMS
  add constraint OFS_ITEMS_PK primary key (ID)
  using index;
  
create unique index OFS_ITEMS_DIRENTRY 
  on OFS_ITEMS (PARENT_ID, UPPER(NAME));
  
alter table OFS_ITEMS
  add constraint OFS_ITEMS_PARENT_FK foreign key (PARENT_ID)
  references OFS_ITEMS (ID) on delete cascade;

-- Create/Recreate check constraints 
alter table OFS_ITEMS
  add constraint OFS_ITEMS_COMPRESSED
  check (compressed in ('Y', 'N'));
alter table OFS_ITEMS
  add constraint OFS_ITEMS_FILECHECK
  check ((item_type='F') or 
         ((data is null) and 
          (locked_by is null) and 
          (date_locked is null)));
alter table OFS_ITEMS
  add constraint OFS_ITEMS_ITEM_TYPE
  check (item_type in ('D', 'F'));
alter table OFS_ITEMS
  add constraint OFS_ITEMS_READ_ONLY
  check (read_only in ('Y', 'N'));
alter table OFS_ITEMS
  add constraint OFS_ITEMS_SELFREF
  check ((parent_id = 0 and id = 0) or (id <> parent_id));

-- Create sequence
create sequence OFS_ID_SEQ start with 1;
