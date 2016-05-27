create function get_pictures_watched_by_user(user_id T_UsersID)
returns table as return (
	select Pictures.ID 
	from Pictures
		inner join Watches on Pictures.ID = Watches.PictureID
		inner join Users on Watches.UserID = Users.ID
	where Users.ID = user_id and Watches.Reposted = 0
)
go
create function get_pictures_reposted_by_user(user_id T_UsersID)
returns table as return (
	select Pictures.ID
	from Pictures
		inner join Watches on Pictures.ID = Watches.PictureID
		inner join Users on Watches.UserID = Users.ID
	where Users.ID = user_id and Watches.Reposted = 1
)
go
create function get_pictures_watched_not_reposted(user_id T_UsersID)
returns table as return (
	select ID from get_pictures_watched_by_user(user_id) 
	except
	select ID from get_pictures_reposted_by_user(user_id) 
)
go
create function get_pictures_reposted_not_watched(user_id T_UsersID)
returns table as return (
	select ID from get_pictures_reposted_by_user(user_id) 
	except
	select ID from get_pictures_watched_by_user(user_id) 
)
go
select * from get_pictures_watched_by_user(0);
select * from get_pictures_reposted_by_user(0);
select * from get_pictures_reposted_not_watched(0);
select * from get_pictures_watched_not_reposted(0);
go
