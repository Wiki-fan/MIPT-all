create or replace procedure add_Picture (
	NName varchar,
	NDescription varchar,
	NWidth int,
	NHeight int,
	Nartist_name varchar,
	Nuploader_nickname varchar,
	NDateCreated date
) as
id_ int;
aid_ int;
uid_ int;
tmp int;
begin
	select COUNT(*) into tmp from Pictures where NName = Name;
  if tmp > 0 then
		raise_application_error(-20000, 'Repeated value Name');
  end if;
  select MAX(ID)+1 into id_ from Pictures;
  select ID into aid_ from Artists where Name=Nartist_name;
  select ID into uid_ from Users where Nickname=Nuploader_nickname;
  insert into Pictures
		(ID,Name,Description,Width,Height,Rating,
		ArtistID,UploaderID,DateCreated,DateUploaded)
	values ( id_,
			NName,
			NDescription,
			NWidth,
			NHeight,
			0,
			aid_,
			uid_,
			NDateCreated,
			sysdate()
	);
end;
/
-- Test.
select * from Pictures where name='name';
/
begin
add_Picture('name', 'desc',20, 30, 'Hans von AACHEN','tinyduck196', TO_DATE('15/10/2013', 'dd/mm/yy'));
end;
/
select * from Pictures where name='name';
/
create or replace procedure add_Tag_to_Picture (picture_name varchar, tag_ varchar ) as
id_ int;
tmp int;
tid int;
pid int;
begin
  select ID into pid from Pictures where Name = picture_name;
  select COUNT(*) into tmp from table(get_picture_tags(pid)) tbl where Tag = tag_;
	if tmp>0 then
		raise_application_error(1, 'Repeated value Name');
  end if;
  select count(ID) into tmp from Tags where Tags.Name = tag_;
	if tmp=0 then
		insert into Tags (ID, Name) 
		values (id_,tag_);
  end if;
  select MAX(ID)+1 into tmp from Tags_Pictures;
  select ID into tid from Tags where Name = tag_;
	insert into Tags_Pictures(ID,TagID,PictureID)
	values (temp,tid,pid);
end;
/
-- Test.
--declare picture_name varchar = (select Name from Pictures where ID=0);
select t.Name as Tag from Pictures p
inner join Tags_Pictures tp 
on p.ID = tp.PictureID
inner join Tags t
on t.ID = tp.TagID
where p.Name = 'name';
/
begin
add_Tag_to_Picture ('name', 'test tag');
end;
/
select * from table(get_picture_tags(0));
/
