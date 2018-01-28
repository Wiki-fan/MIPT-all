use Gallery;
-- 1) Картинка с заданным тегом.
if object_id (N'get_pictures_by_tag', N'IF') is not null
	drop function get_pictures_by_tag;
go
create function get_pictures_by_tag (@tag_id varchar(100))
  returns table as return (
	  select distinct PictureID
	  from Tags_Pictures 
		  inner join Pictures on Pictures.ID = Tags_Pictures.PictureID
		  inner join Tags on Tags.ID = Tags_Pictures.TagID
	  where Tags.ID = @tag_id
);
go
use Gallery;
select * from Pictures inner join get_pictures_by_tag(0) as with_tag on Pictures.ID = with_tag.PictureID;
go
