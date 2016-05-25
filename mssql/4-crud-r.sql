use Gallery;
if object_id (N'get_picture_tags', N'IF') is not null
	drop function get_picture_tags;
go
create function get_picture_tags (@id T_PicturesID)
	returns table as return (
		select Tags.Name from Pictures 
			inner join Tags_Pictures on Pictures.ID=Tags_Pictures.PictureID
			inner join Tags on Tags_Pictures.TagID = Tags.ID
		where Pictures.ID = @id
	)
go
-- ѕолучает картинку по ID. ¬озвращает все картинки, если передан null или ничего не передано.
if object_id (N'get_picture_by_id', N'IF') is not null
	drop function get_picture_by_id;
go
create function get_picture_by_id (@id T_PicturesID = null)
	returns table as return (
		select ID,
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
		where ID=@id or @id is null
	)
go
-- Test
select * from get_picture_by_id(0);
select * from get_picture_by_id(null);
select * from get_picture_by_id(default);
select * from get_picture_tags(0);
go
