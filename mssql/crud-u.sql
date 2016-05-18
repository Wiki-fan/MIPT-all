-- Содержит функции изменения имени картинки по ID
use Gallery;
if object_id (N'change_picture_name', N'P') is not null
	drop procedure change_picture_name;
go
create procedure change_picture_name (@id T_PicturesID, @newName varchar(100))
as
	if (select ID from Pictures where Name = @newName) is not null
		raiserror ('Already used name', 16, 1);
	update Pictures
		set Name = @newName
		where ID=@id
go
-- Test
select Name from Pictures where ID=0
exec change_picture_name 0, 'Newname'
select Name from Pictures where ID=0
go
