-- Содержит процедуры добавления картинки (по имени художника и загрузившего) и добавления тега.
use Gallery;
if object_id (N'add_Picture', N'P') is not null
	drop procedure add_Picture;
go
create procedure add_Picture (
	@Name varchar(100),
	@Description varchar(100),
	@Width int,
	@Height int,
	@artist_name varchar(100),
	@uploader_nickname varchar(100),
	@DateCreated date
)
as
	if (select ID from Pictures where Name = @Name) is not Null
		raiserror('Repeated value Name', 16, 1)
	insert into Pictures
		(ID,Name,Description,Width,Height,Rating,
		ArtistID,UploaderID,DateCreated,DateUploaded)
	values ((select COUNT(ID) from Pictures),
			@Name,
			@Description,
			@Width,
			@Height,
			0,
			(select ID from Artists where Name=@artist_name),
			(select ID from Users where Nickname=@uploader_nickname),
			@DateCreated,
			getdate()
	)
go
-- Test.
use Gallery;
declare @date_created date = CONVERT(DATETIME, '15/10/2013', 104);
declare @user_name varchar(100) = (select Nickname from Users where ID=0);
declare @artist_name varchar(100) = (select Name from Artists where ID=0);
exec add_Picture 'name', 'desc',20, 30, @artist_name,@user_name,@date_created
go


if object_id (N'add_Tag_to_Picture', N'P') is not null
	drop procedure add_Tag_to_Picture;
go
create procedure add_Tag_to_Picture (@picture_name varchar(100), @tag varchar(100) )
as 
	if (select Name from get_picture_tags(@picture_name) where Name = @tag) is not null
		raiserror('Repeated value Name', 16, 1)
	if (select ID from Tags where Tags.Name = @tag) is null
		insert into Tags (ID, Name) 
		values ((select COUNT(ID) from Tags),
				@tag
		)
	insert into Tags_Pictures(ID,TagID,PictureID)
	values ((select COUNT(ID) from Tags_Pictures),
			(select ID from Tags where Name = @tag),
			(select ID from Pictures where Name = @picture_name)
	)
go
-- Test.
declare @picture_name varchar(100) = (select Name from Pictures where ID=0);
select * from get_picture_tags(@picture_name);
exec add_Tag_to_Picture 0, 'test tag';
select * from get_picture_tags(@picture_name);
go
