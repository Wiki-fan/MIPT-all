use Gallery;
go 
/*drop index Tags.TagsIDX
drop index Watches.WatchesIDX
drop index Watches.WatchesPictureIDX
drop index Watches.WatchesUserIDX
drop index Watches.WatchesLikedIDX*/
/*create unique index TagsIDX on Tags(ID);
create unique index WatchesIDX on Watches(ID);
create index WatchesPictureIDX on Watches(PictureID) include (UserID);
create unique index WatchesUserIDX on Watches(UserID);
create index WatchesLikedIDX on Watches(Liked) include (PictureID);
go*/
if object_id('get_artist_like_count', 'FN') is not null
	drop function get_artist_like_count;
go
create function get_artist_like_count(@id T_ArtistsID)
returns int as 
	begin
	declare @ret int = 
	(select COUNT(w.ID) as cnt 
		from Artists as a
		join Pictures as p
			on a.ID = p.ArtistID
		join Watches as w 
			on p.ID = w.PictureID
		where a.ID = @id)
	return @ret
	end
go
if object_id('get_max_tag_count', 'FN') is not null
	drop function get_max_tag_count;
go
create function get_max_tag_count(@id T_ArtistsID)
returns int as 
	begin
	declare @ret int = (
	select top 1 max(count(t.ID)) over() as cnt 
	from Tags as t
		inner join Tags_Pictures as tp
			on t.ID = tp.TagID
		inner join Pictures as p
			on p.ID = tp.PictureID
		inner join Watches as w 
			on p.ID = w.PictureID
	where w.Liked = 1 and p.ArtistID = @id
	group by t.ID
	)
	return @ret
	end
go
if object_id('get_most_popular_tags', 'IF') is not null
	drop function get_most_popular_tags;
go
create function get_most_popular_tags(@id T_ArtistsID, @count int)
returns table as 
	return select top(@count) t.Name
	from Tags as t
		inner join Tags_Pictures as tp
			on t.ID = tp.TagID
		inner join Pictures as p
			on p.ID = tp.PictureID
		inner join Watches as w 
			on p.ID = w.PictureID
	where w.Liked = 1 and p.ArtistID = @id
	group by t.Name
	order by count(t.ID) desc
go
if object_id('get_avg_age', 'FN') is not null
	drop function get_avg_age;
go
create function get_avg_age(@id T_ArtistsID)
returns date as 
	begin
	declare @ret date = (
		select dateadd(day, avg(datediff(day, u.RegistrationDate, getdate())), CONVERT(DATE, '00010101', 112)) as age
		from Users as u
			inner join Watches as w
				on w.UserID = u.ID
			inner join Pictures as p
				on w.PictureID = p.ID
		where p.ArtistID = @id
	)
	return @ret
	end
go
if object_id('artist_view', 'V') is not null
	drop view artist_view;
go
create view artist_view
as
	select a.Name, 

	 dbo.get_artist_like_count(a.ID) as likes,

	(select * from dbo.get_most_popular_tags(a.ID, 1)) as mpt,
	(select * from dbo.get_most_popular_tags(a.ID, 2) 
		except select * from dbo.get_most_popular_tags(a.ID, 1)) as mpt2,

	dbo.get_avg_age(a.ID) as avg_date
from Artists as a
go
select top 100 * from artist_view
go
