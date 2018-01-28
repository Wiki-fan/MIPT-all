create function pictures_watched_in(period_ date)
returns table as return (
	select w.PictureID
		from Watches as w
		where datediff(day, w.WatchDate, getdate()) < datediff(day, 0,period_)
)
go
create function get_count_watches_artists(period_ date )
returns table as return (
	select COUNT(a.ID) as Watches, a.ID as ArtistID
		from pictures_watched_in(period_) as watched
			inner join Pictures as p
				on watched.PictureID = p.ID 
			inner join Artists as a
				on p.ArtistID = a.ID 
		group by a.ID
)
go
create function most_popular_by_watches_artist(period_ date)
returns table as return (
	select ArtistID, Watches
	from get_count_watches_artists(period_)
	where Watches = (select MAX(tbl.Watches) as Max from get_count_watches_artists(period_) as tbl) 
) 
go
select * from pictures_watched_in(dateadd(year, 2, 0));
select * from get_count_watches_artists(dateadd(year, 2, 0));
select * from most_popular_by_watches_artist(dateadd(year, 2, 0));
go
