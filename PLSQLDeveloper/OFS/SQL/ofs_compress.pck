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
    return true;
  end;

  procedure comp(src in     blob,
                 dst in out nocopy blob) is
  begin
    utl_compress.lz_compress(src, dst, 6);    
  end;
                       
  function uncomp(src in blob) return blob is
  begin
    return utl_compress.lz_uncompress(src);
  end;

  procedure uncomp(src in     blob,
                   dst in out nocopy blob) is
  begin
    utl_compress.lz_uncompress(src, dst);
  end;  
end;
/
