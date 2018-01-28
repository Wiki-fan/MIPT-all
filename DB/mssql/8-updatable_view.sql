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
	return select top(@count) /*count(t.ID),*/ po.Name
	from Pools as po
		inner join Pools_Pictures as pp
			on po.ID = pp.PoolID
		inner join Pictures as pic
			on pic.ID = pp.PictureID
		inner join Watches as w 
			on pic.ID = w.PictureID
	where w.UserID = @id
	group by po.Name
	order by count(po.ID) desc
go
if object_id('user_view', 'V') is not null
	drop view user_view;
go
create view user_view
as
	select u.Name, 
	u.Nickname,
	(select * from get_user_most_popular_tags(u.ID, 1)) as mpt,
	(select * from get_user_most_popular_tags(u.ID, 2) except select * from get_user_most_popular_tags(u.ID, 1)) as mpt2,
	(select * from get_user_most_popular_pools(u.ID, 1)) as mpp,
	(select * from get_user_most_popular_pools(u.ID, 2) except select * from get_user_most_popular_pools(u.ID, 1)) as mpp2


from Users as u
go
select top 100 * from user_view
go
update user_view
set Nickname='WonderfulUser'
where Name=(select Name from Users where ID=2)
go
select * from Users where ID=2
go
