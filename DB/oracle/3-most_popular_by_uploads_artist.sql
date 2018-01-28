create function pictures_uploaded_in(@period date)
returns table as return (
	select p.*
		from Pictures as p
		where /*dateadd(day, datediff(day, @period, 0), p.DateUploaded) > getdate()*/
		datediff(day, p.DateUploaded, getdate()) < datediff(day, 0,@period)
)
go
create function get_count_uploads_artists(@period date )
returns table as return (
	select COUNT(ArtistID) as Uploads, ArtistID
		from pictures_uploaded_in(@period)
		group by ArtistID
)
go
create function most_popular_by_uploads_artist(@period date)
returns table as return (
	select ArtistID, Uploads
	from get_count_uploads_artists(@period)
	where Uploads = (select MAX(tbl.Uploads) as Max from get_count_uploads_artists(@period) as tbl) 
) 
go
select * from pictures_uploaded_in(dateadd(year, 5, 0))
select * from get_count_uploads_artists(dateadd(year, 5, 0))
select * from most_popular_by_uploads_artist(dateadd(year, 5, 0));
go
