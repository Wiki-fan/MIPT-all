use Gallery;
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
