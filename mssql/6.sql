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
	select Watches.PictureID
		from Watches  
		where dateadd(day, datediff(day, @period, 0), Watches.WatchDate) 
			> sysdatetime()
)
go
create function get_count_watches_artists(@period date )
returns table as return (
	select COUNT(ArtistID) as Watches, ArtistID
		from pictures_watched_in(@period) as watched
			inner join Pictures on watched.PictureID = Pictures.ID 
			inner join Artists on Pictures.ArtistID = Artists.ID 
		group by ArtistID
)
go
create function most_popular_by_watches_artist(@period date)
returns table as return (
	select ArtistID, Watches
	from get_count_watches_artists(@period)
	where Watches = (select MAX(tbl.Watches) as Max from get_count_watches_artists(@period) as tbl) 
) 
go
select * from most_popular_by_watches_artist(dateadd(month, 1, 0));
go
