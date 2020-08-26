create or replace package ofs_compress is
  function is_active return boolean;
  procedure comp(src in     blob,
                 dst in out nocopy blob);
  function uncomp(src in blob) return blob;
  procedure uncomp(src in     blob,
                   dst in out nocopy blob);
end;
/
create or replace package body ofs_compress is
  function is_active return boolean is
  begin
    return false;
  end;

  procedure not_impl is
  begin
    raise_application_error(-20000, 'OFS-16: Compression is implemented on 10g only');
  end;
  
  procedure comp(src in     blob,
                 dst in out nocopy blob) is
  begin
    not_impl;
  end;
                       
  function uncomp(src in blob) return blob is
  begin
    not_impl;
    return null;
  end;
  
  procedure uncomp(src in     blob,
                   dst in out nocopy blob) is
  begin
    not_impl;
  end;  
end;
/
