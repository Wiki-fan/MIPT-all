create view InsteadArtists
AS SELECT *
FROM Artists;
GO

create trigger ArtistsInsertTrigger
	on InsteadArtists
	instead of insert
as insert into Artists 
	select FirstName, LastName, PersonalSite from inserted
go

create trigger InsteadArtistsInsertTrigger
	on Artists
	instead of insert
as begin
declare @id T_ArtistsID = (select COUNT(ID) from Artists),
		@name varchar(100) = CONCAT(
		(select FirstName from inserted)
		, ' ', (select LastName from inserted)
		)
	insert into Artists (ID,Name,FirstName,LastName,PersonalSite)
	values (@id, @name,(select FirstName from inserted),
	(select LastName from inserted),
	(select PersonalSite from inserted));
	end;
go

insert into Artists values ('first', 'last', 'site');
go