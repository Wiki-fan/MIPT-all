use Gallery;
if object_id (N'get_user_artists', N'IF') is not null
	drop function get_user_artists;
go
create function get_user_artists(@user_id varchar(100))
returns table as return (
	select distinct Artists.ID
	from Artists 
		inner join Pictures on Artists.ID = Pictures.ArtistID
		inner join Users on Pictures.UploaderID = Users.ID
	where Users.ID = @user_id
)
go
select * from Artists inner join get_user_artists(0) as user_artists on Artists.ID = user_artists.ID;
go
