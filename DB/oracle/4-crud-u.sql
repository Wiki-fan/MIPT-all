create or replace procedure change_picture_name (id_ int, newName varchar) as
tmp int;
Begin
  select count(ID) into tmp from Pictures where Name = newName;
	if tmp>0 then
		raise_application_error (-20000, 'Already used name');
  end if;
	update Pictures
		set Name = newName
		where ID=id_;
end;
/
-- Test
select Name from Pictures where ID=0
/
begin
change_picture_name(0, 'Newname');
end;
/
select Name from Pictures where ID=0
/
