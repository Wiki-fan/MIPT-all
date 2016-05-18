if object_id (N'get_user_artists', N'IF') is not null
	drop function get_user_artists;
go
select COUNT(PictureID), PictureID from Watches group by PictureID;
go
create function most_popular_by_views_pictures()
returns table as return (
	select distinct Pictures.*
	from Pictures
	where ID in ()
)
go
select * from most_popular_by_views_pictures();
go
