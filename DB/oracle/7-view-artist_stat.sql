create or replace function get_artist_like_count(id_ int)
return int as 
ret int;
begin
	select COUNT(w.ID) into ret
		from Artists a
		join Pictures p
			on a.ID = p.ArtistID
		join Watches w 
			on p.ID = w.PictureID
		where a.ID = id_;
	return ret;
end;
/
create or replace function get_max_tag_count(id_ int)
return int as 
ret int;
begin
	select  max(count(t.ID)) into ret 
	from Tags t
		inner join Tags_Pictures tp
			on t.ID = tp.TagID
		inner join Pictures p
			on p.ID = tp.PictureID
		inner join Watches w 
			on p.ID = w.PictureID
	where w.Liked = 1 and p.ArtistID = id_
	group by t.ID;
	return ret;
end;
/
create or replace function get_most_popular_tags(id_ int, cnt int)
return t_Top as ret t_Top;
begin
  select cast(multiset(select t.Name
	from Tags t
		inner join Tags_Pictures tp
			on t.ID = tp.TagID
		inner join Pictures p
			on p.ID = tp.PictureID
		inner join Watches w 
			on p.ID = w.PictureID
	where w.Liked = 1 and p.ArtistID = id_ and rownum<=cnt
	group by t.Name
	order by count(t.ID) desc
  ) as t_Top)
    into ret from dual;
    return ret;
end;
/
create or replace function get_avg_age(id_ int)
return date as 
ret date;
days_ int;
begin
  select avg(sysdate()-u.RegistrationDate) into days_
		from Users u
			inner join Watches w
				on w.UserID = u.ID
			inner join Pictures p
				on w.PictureID = p.ID
		where p.ArtistID = id_;
  select to_date(0,'y') + days_* interval '1' day into ret from dual;
	return ret;
end;
/
create or replace view artist_view
as
	select a.Name, 

  get_artist_like_count(a.ID) as likes,

	(select * from table(get_most_popular_tags(a.ID, 1))) as mpt,
	(select * from table(get_most_popular_tags(a.ID, 2)) 
		minus select * from table(get_most_popular_tags(a.ID, 1))) as mpt2,

	get_avg_age(a.ID) as avg_date
from Artists a;
/
select * from artist_view where rownum<=100;
/
