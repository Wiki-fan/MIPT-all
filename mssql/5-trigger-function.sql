use Gallery;
go
if object_id (N'get_name_from_first_and_last', N'FN') is not null
	drop function get_name_from_first_and_last;
go
create function get_name_from_first_and_last(@first varchar(100), @last varchar(100))
returns varchar(100) as 
begin
declare @ret varchar(200) =  (select CONCAT(@first, ' ', @last))
return @ret
end
go
