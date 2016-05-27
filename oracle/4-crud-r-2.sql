create or replace type t_row_Picture as object
(
ID int,
Name varchar(100),
Description varchar(100),
Width int,
Height int,
Rating int,
"Artist name" varchar(100),
"Uploader name" varchar(100),
DateCreated date,
DateUploaded date
);
/
create or replace type t_Picture is table of t_row_Picture;
/
create or replace function get_picture_by_id (p_id in int default null) RETURN t_Picture AS 
ret t_Picture := t_Picture();
begin
    select cast(multiset(select ID,
				Name,
				Description,
				Width,
				Height,
				Rating,
				(select Name from Artists where ID = ArtistID) as artist,
				(select Nickname from Users where ID = UploaderID) as uploader,
				DateCreated,
				DateUploaded
		from Pictures
		where ID=p_id or p_id is null)
    as t_Picture)
    into ret from dual;
    return ret;
end;
/
-- Test
select * from table(get_picture_by_id(0));
/
select * from table(get_picture_by_id(null));
/
select * from table(get_picture_by_id());
/

