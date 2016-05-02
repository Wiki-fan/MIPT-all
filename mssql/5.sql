use Gallery;
if object_id (N'pictures_uploaded_in', N'IF') is not null
	drop function pictures_uploaded_in;
go
if object_id (N'get_count_uploads_artists', N'IF') is not null
	drop function get_count_uploads_artists;
go
if object_id (N'most_popular_by_uploads_artist', N'IF') is not null
	drop function most_popular_by_uploads_artist;
go
create function pictures_uploaded_in(@period date)
returns table as return (
	select Pictures.*
		from Pictures
		where dateadd(day, datediff(day, @period, 0), Pictures.DateUploaded) 
			> sysdatetime()
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
select * from most_popular_by_uploads_artist(dateadd(month, 1, 0));
go
