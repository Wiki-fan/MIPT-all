use Gallery;
go
if object_id (N'pictures_watched_between', N'IF') is not null
	drop function pictures_watched_between;
go
if object_id (N'get_count_watches_artists_between', N'IF') is not null
	drop function get_count_watches_artists_between;
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
if object_id (N'get_stat', N'P') is not null
	drop procedure get_stat;
go
create procedure get_stat(@years int) as 
begin
	declare @i int = 2,
			@query varchar(max);
	set @query = 'select Artists.ID as [Artist ID], ';
	set @query = @query + CONCAT('Year',CONVERT(nvarchar(2), @i-1), '.Watches as [Year ',CONVERT(nvarchar(2), @i-1),']');
	set @i = @i + 1;
	while @i < @years+2
	begin
		set @query = @query + CONCAT(', Year',CONVERT(nvarchar(2), @i-1), '.Watches as [Year ',CONVERT(nvarchar(2), @i-1),']');
		set @i = @i + 1;
		--print (@query);
	end;
	set @query = @query + ' from '
	set @i = 2;
	set @query = @query + '(select ID from Artists) as Artists ';
	set @query = @query + 
		CONCAT('left join (select ArtistID, Watches from get_count_watches_artists_between(dateadd(year,',
		CONVERT(nvarchar(2), @i),
		', 0),dateadd(year,',
		CONVERT(nvarchar(2), @i-1),
		', 0))) as Year',CONVERT(nvarchar(2), @i-1), ' on Artists.ID = Year',CONVERT(nvarchar(2), @i-1),'.ArtistID' );
	set @i = @i + 1;
	--print (@query);
	while @i < @years+2
	begin
		set @query = @query + 
		CONCAT(' left join (select ArtistID, Watches from get_count_watches_artists_between(dateadd(year,',
		CONVERT(nvarchar(2), @i),
		', 0),dateadd(year,',
		CONVERT(nvarchar(2), @i-1),
		', 0))) as Year',CONVERT(nvarchar(2), @i-1),' on Artists.ID = Year', CONVERT(nvarchar(2), @i-1), '.ArtistID');
		set @i = @i + 1;
		--print (@query);
	end;
	set @query = @query + ' order by Artists.ID asc';
	print (@query);
	exec (@query);
end
go
exec get_stat 15;
go
