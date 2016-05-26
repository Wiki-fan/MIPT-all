use Gallery;
go
if object_id (N'pictures_watched_between', N'IF') is not null
	drop function pictures_watched_between;
go
if object_id (N'get_count_watches_artists_between', N'IF') is not null
	drop function get_count_watches_artists_between;
go
if object_id (N'most_popular_by_watches_artist_between', N'IF') is not null
	drop function most_popular_by_watches_artist_between;
go
create function pictures_watched_between(@begin date, @end date)
returns table as return (
	select w.PictureID
		from Watches as w
		where datediff(day, w.WatchDate, getdate()) > datediff(day, 0,@end) and
			datediff(day, w.WatchDate, getdate()) < datediff(day, 0,@begin)
)
go
create function get_count_watches_artists_between(@begin date, @end date)
returns table as return (
	select COUNT(a.ID) as Watches, a.ID as ArtistID
		from pictures_watched_between(@begin, @end) as watched
			inner join Pictures as p
				on watched.PictureID = p.ID 
			inner join Artists as a
				on p.ArtistID = a.ID 
		group by a.ID
)
go
create function most_popular_by_watches_artist_between(@begin date, @end date)
returns table as return (
	select ArtistID, Watches
	from get_count_watches_artists_between(@begin, @end)
	where Watches = (select MAX(tbl.Watches) as Max from get_count_watches_artists_between(@begin, @end) as tbl) 
) 
go
if object_id (N'get_stat', N'P') is not null
	drop procedure get_stat;
go
create procedure get_stat(@years int) as 
begin
	declare @i int = 2,
			@query varchar(max);
	set @query = 'select Year, ArtistID, Watches from (';

	set @query = @query + 
		CONCAT('(select ',CONVERT(nvarchar(2), @i-1),' as Year, ArtistID, Watches from most_popular_by_watches_artist_between(dateadd(year,',
		CONVERT(nvarchar(2), @i),
		', 0),dateadd(year,',
		CONVERT(nvarchar(2), @i-1),
		', 0))) ');
	set @i = @i + 1;
	print (@query);
	while @i < @years
	begin
		set @query = @query + 
		CONCAT(' UNION (select ',CONVERT(nvarchar(2), @i-1),' as Year, ArtistID, Watches from most_popular_by_watches_artist_between(dateadd(year,',
		CONVERT(nvarchar(2), @i),
		', 0),dateadd(year,',
		CONVERT(nvarchar(2), @i-1),
		', 0)))');
		set @i = @i + 1;
		print (@query);
	end;
	set @query = @query + ') as tbl';
	print (@query);
	exec (@query);
end
go
exec get_stat 15;
go
