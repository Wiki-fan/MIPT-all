-- Содержит функцию удаления картинки по названию.
use Gallery;
if object_id (N'remove_Picture', N'P') is not null
	drop procedure remove_Picture;
go
create procedure remove_Picture (@picture_name varchar(100))
as 
	delete from Pictures where Name = @picture_name
go
-- Test.
select * from Pictures where Name = 'name'
exec remove_Picture 'name'
select * from Pictures where Name = 'name'
go
