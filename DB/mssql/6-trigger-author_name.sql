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
if object_id (N'AfterArtistsInsertTrigger', N'TR') is not null
	drop trigger AfterArtistsInsertTrigger;
go
create trigger AfterArtistsInsertTrigger
	on Artists
	after insert
as begin
if exists(select * from inserted)
	update Artists
	set Name = dbo.get_name_from_first_and_last(FirstName, LastName)
	where ID = (select ID from inserted)
end
go
insert into Artists (ID, FirstName, LastName, PersonalSite) values
((select MAX(ID)+1 from Artists), 'TestFirstName', 'TestLastName', 'http://google.com/');
select * from Artists where FirstName = 'TestFirstName'
go
delete from Artists where Name = 'TestFirstName TestLastName'
go
