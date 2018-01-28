create or replace procedure remove_Picture (picture_name varchar)
as 
i int;
tmp int;
begin
  select count(ID) into tmp from Pictures where Name = picture_name;
	if tmp=0 then
		raise_application_error (-20000, 'No such picture');
  end if;
  select ID into i from Pictures where Name = picture_name;
  delete from Tags_Pictures tp where PictureID = i;
	delete from Pictures where Name = picture_name;
end;
/
-- Test.
select * from Pictures where Name = 'name'
/
begin
remove_Picture('name');
end;
/
select * from Pictures where Name = 'name';
/
