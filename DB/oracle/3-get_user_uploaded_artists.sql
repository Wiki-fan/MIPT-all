create type t_row_ID as object(ID int);
/
create type t_ID is table of t_row_id;
/
create or replace function get_user_uploaded_artists(user_id varchar)
return t_ID as ret t_ID;
begin
	select cast(multiset(
    select distinct Artists.ID
    from Artists 
      inner join Pictures on Artists.ID = Pictures.ArtistID
      inner join Users on Pictures.UploaderID = Users.ID
    where Users.ID = user_id
  ) as t_ID)
  into ret from dual;
  return ret;
end;
/
select * from Artists inner join table(get_user_uploaded_artists(1)) user_artists on Artists.ID = user_artists.ID;
/
