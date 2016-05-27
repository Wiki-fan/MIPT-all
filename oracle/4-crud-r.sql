create or replace type t_row_PictureTags as object (TagName varchar(100));
/
create or replace type t_PictureTags is table of t_row_PictureTags;
/
create or replace function get_picture_tags (P_id in int) RETURN t_PictureTags AS 
ret t_PictureTags ;
begin
		select cast(multiset(select Tags.Name as Tag from Pictures 
			inner join Tags_Pictures on Pictures.ID=Tags_Pictures.PictureID
			inner join Tags on Tags_Pictures.TagID = Tags.ID
		where Pictures.ID = P_id
    ) as t_PictureTags)
    into ret from dual;
    return ret;
end;
/
select * from table (get_picture_tags(15));
/
