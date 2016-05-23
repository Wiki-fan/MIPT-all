use Gallery;
go 
-- Наиболее популярные по просмотрам теги.
if object_id('get_user_most_popular_tags', 'IF') is not null
	drop function get_user_most_popular_tags;
go
create function get_user_most_popular_tags(@id T_UsersID, @count int)
returns table as 
	return select top(@count) /*count(t.ID),*/ t.Name
	from Tags as t
		inner join Tags_Pictures as tp
			on t.ID = tp.TagID
		inner join Pictures as p
			on p.ID = tp.PictureID
		inner join Watches as w 
			on p.ID = w.PictureID
	where w.UserID = @id
	group by t.Name
	order by count(t.ID) desc
go
if object_id('get_user_most_popular_pools', 'IF') is not null
	drop function get_user_most_popular_pools;
go
create function get_user_most_popular_pools(@id T_UsersID, @count int)
returns table as 
	return select top(@count) /*count(t.ID),*/ t.Name
	from Pools as po
		inner join Pools_Pictures as pp
			on po.ID = pp.TagID
		inner join Pictures as pic
			on pic.ID = pp.PictureID
		inner join Watches as w 
			on pic.ID = w.PictureID
	where w.UserID = @id
	group by p.Name
	order by count(p.ID) desc
go
if object_id('artist_view', 'V') is not null
	drop view artist_view;
go
create view artist_view
as
	select a.Name, 

	(select * from get_artist_like_count(a.ID)) as likes,

	(select * from get_most_popular_tags(a.ID, 1)) as mpt,
	(select * from get_most_popular_tags(a.ID, 2) except select * from get_most_popular_tags(a.ID, 1)) as mpt2,

	(select * from get_avg_age(a.ID)) as avg_date
from Artists as a
go
select * from artist_view
go
