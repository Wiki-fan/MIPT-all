use Gallery;
-- 6) Наиболее популярный по просмотрам художник.
if object_id (N'pictures_watched_in', N'IF') is not null
	drop function pictures_watched_in;
go
if object_id (N'get_count_watches_artists', N'IF') is not null
	drop function get_count_watches_artists;
go
if object_id (N'most_popular_by_watches_artist', N'IF') is not null
	drop function most_popular_by_watches_artist;
go
create function pictures_watched_in(@period date)
returns table as return (
	select w.PictureID
		from Watches as w
		where datediff(day, w.WatchDate, getdate()) < datediff(day, 0,@period)
)
go
create function get_count_watches_artists(@period date )
returns table as return (
	select COUNT(a.ID) as Watches, a.ID as ArtistID
		from pictures_watched_in(@period) as watched
			inner join Pictures as p
				on watched.PictureID = p.ID 
			inner join Artists as a
				on p.ArtistID = a.ID 
		group by a.ID
)
go
create function most_popular_by_watches_artist(@period date)
returns table as return (
	select ArtistID, Watches
	from get_count_watches_artists(@period)
	where Watches = (select MAX(tbl.Watches) as Max from get_count_watches_artists(@period) as tbl) 
) 
go
select * from pictures_watched_in(dateadd(year, 2, 0));
select * from get_count_watches_artists(dateadd(year, 2, 0));
select * from most_popular_by_watches_artist(dateadd(year, 2, 0));
go
