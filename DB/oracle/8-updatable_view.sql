create type t_row_Top as object(Name varchar(100));
/
create or replace type t_Top is table of t_row_Top;
/
create or replace function get_user_most_popular_tags(id_ int, cnt int)
return t_Top 
as ret t_Top;
begin
	select cast(multiset(
  select t.Name as nme
	from Tags t
		inner join Tags_Pictures tp
			on t.ID = tp.TagID
		inner join Pictures p
			on p.ID = tp.PictureID
		inner join Watches w 
			on p.ID = w.PictureID
	where w.UserID = id_ and rownum<=cnt
	group by t.Name
	order by count(t.ID) desc
  ) as t_Top) into ret from dual;
  return ret;
end;
/
create or replace function get_user_most_popular_pools(id_ int, cnt int)
return t_Top as ret t_Top;
begin
	select cast(multiset(select /*count(t.ID),*/ po.Name as nme
	from Pools po
		inner join Pools_Pictures pp
			on po.ID = pp.PoolID
		inner join Pictures pic
			on pic.ID = pp.PictureID
		inner join Watches w 
			on pic.ID = w.PictureID
	where w.UserID = id_ and rownum<=cnt
	group by po.Name
	order by count(po.ID) desc
  ) as t_Top) into ret from dual;
  return ret;
end;
/
create or replace view user_view
as
	select u.Name, 
	u.Nickname,
	(select * from table(get_user_most_popular_tags(u.ID, 1))) as mpt,
	((select * from table(get_user_most_popular_tags(u.ID, 2))) minus (select * from table(get_user_most_popular_tags(u.ID, 1)))) as mpt2,
	(select * from table(get_user_most_popular_pools(u.ID, 1))) as mpp,
	((select * from table(get_user_most_popular_pools(u.ID, 2))) minus (select * from table(get_user_most_popular_pools(u.ID, 1)))) as mpp2
from Users u;
/
select * from user_view where rownum <= 100
/
update user_view
set Nickname='WonderfulUser'
where Name=(select Name from Users where ID=2)
/
select * from Users where ID=2;
/
