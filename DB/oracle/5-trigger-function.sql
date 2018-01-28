create or replace function get_name_from_first_and_last(first in varchar, last in varchar)
return varchar is ret varchar(200);
begin
ret :=  first || ' ' || last;
return ret;
end;
/