create procedure InsertPicture(
@ID T_PicturesID,
@Name varchar,
@Description varchar,
@Width int,
@Height int,
@Rating int,
@ArtistID T_ArtistsID,
@UploaderID T_UsersID,
@DateCreated date,
@DateUploaded date
)
as
	Begin
	if 
	insert into Pictures 
		value (@ID,@Name,@Description,@Width,@Height,@Rating,
		@ArtistID,@UploaderID,@DateCreated,@DateUploaded);
		
	end
