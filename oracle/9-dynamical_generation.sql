ALTER SESSION SET NLS_LANGUAGE = 'AMERICAN';
/
create or replace function pictures_watched_between(begin_ int, end_ int)
return t_PictureID as ret t_PictureID;
begin
	select cast(multiset(
    select w.PictureID as PictureID
      from Watches w
      where sysdate()-w.WatchDate > end_*365 and
        sysdate()-w.WatchDate < begin_*365
  ) as t_PictureID)
  into ret from dual;
  return ret;
end;
/
create type t_row_count_watches as object(Watches int, ArtistID int);
/
create type t_count_watches is table of t_row_count_watches;
/
create or replace function get_count_watches_between(begin_ int, end_ int)
return t_count_watches as ret t_count_watches;
begin
  select cast(multiset(
    select COUNT(a.ID) as Watches, a.ID as ArtistID
      from table(pictures_watched_between(begin_, end_)) watched
        inner join Pictures p
          on watched.ID = p.ID 
        inner join Artists a
          on p.ArtistID = a.ID 
      group by a.ID
  ) as t_count_watches)
  into ret from dual;
    return ret;
end;
/
CREATE OR REPLACE PROCEDURE exec(STRING IN varchar2) AS
    cursor_name INTEGER;
    ret INTEGER;
BEGIN
   cursor_name := DBMS_SQL.OPEN_CURSOR;
   DBMS_SQL.PARSE(cursor_name, string, DBMS_SQL.NATIVE);
   ret := DBMS_SQL.EXECUTE(cursor_name);
   DBMS_SQL.CLOSE_CURSOR(cursor_name);
END;
/
create or replace procedure get_stat(years in int) as 
      i int;
			query varchar(32766);
begin
  i := 2;
	query := 'select Artists.ID as "Artist ID", ';
	query := query || 'Year' || TO_CHAR( i-1) || '.Watches as "Year ' || TO_CHAR(i-1) || '"';
	i := i + 1;
  --DBMS_OUTPUT.PUT_LINE( query);
	while i < years+2
  loop
		query := query || ', Year' || TO_CHAR(i-1) || '.Watches as "Year ' || TO_CHAR( i-1) || '"';
		i := i + 1;
	end loop;
	query := query || ' from ';
  
	i := 2;
	query := query || '(select ID from Artists) Artists ';
	query := query || 'left join (select ArtistID, Watches from table(get_count_watches_between(' ||
    TO_CHAR(i) || ',' || TO_CHAR(i-1) || '))) Year' || 
    TO_CHAR(i-1) || ' on Artists.ID = Year' || TO_CHAR(i-1) || '.ArtistID';
  i := i + 1;
	--print (query);
	while i < years+2
  loop
		query := query || ' left join (select ArtistID, Watches from table(get_count_watches_between(' || TO_CHAR( i) || 
		',' || TO_CHAR(i-1) || '))) Year' || TO_CHAR(i-1) || ' on Artists.ID = Year' || TO_CHAR(i-1) || '.ArtistID';
		i := i + 1;
	end loop;
  query := query || ' order by Artists.ID asc';
	DBMS_OUTPUT.PUT_LINE(query);
	exec( query);
	
end;
/
SET SERVEROUTPUT ON
begin
get_stat(15);
end;
/
select Artists.ID as "Artist ID", Year1.Watches as "Year 1", Year2.Watches as "Year 2", Year3.Watches as "Year 3", Year4.Watches as "Year 4", Year5.Watches as "Year 5", Year6.Watches as "Year 6", Year7.Watches as "Year 7", Year8.Watches as "Year 8", Year9.Watches as "Year 9", Year10.Watches as "Year 10", Year11.Watches as "Year 11", Year12.Watches as "Year 12", Year13.Watches as "Year 13", Year14.Watches as "Year 14", Year15.Watches as "Year 15" from (select ID from Artists) Artists left join (select ArtistID, Watches from table(get_count_watches_between(2,1))) Year1 on Artists.ID = Year1.ArtistID left join (select ArtistID, Watches from table(get_count_watches_between(3,2))) Year2 on Artists.ID = Year2.ArtistID left join (select ArtistID, Watches from table(get_count_watches_between(4,3))) Year3 on Artists.ID = Year3.ArtistID left join (select ArtistID, Watches from table(get_count_watches_between(5,4))) Year4 on Artists.ID = Year4.ArtistID left join (select ArtistID, Watches from table(get_count_watches_between(6,5))) Year5 on Artists.ID = Year5.ArtistID left join (select ArtistID, Watches from table(get_count_watches_between(7,6))) Year6 on Artists.ID = Year6.ArtistID left join (select ArtistID, Watches from table(get_count_watches_between(8,7))) Year7 on Artists.ID = Year7.ArtistID left join (select ArtistID, Watches from table(get_count_watches_between(9,8))) Year8 on Artists.ID = Year8.ArtistID left join (select ArtistID, Watches from table(get_count_watches_between(10,9))) Year9 on Artists.ID = Year9.ArtistID left join (select ArtistID, Watches from table(get_count_watches_between(11,10))) Year10 on Artists.ID = Year10.ArtistID left join (select ArtistID, Watches from table(get_count_watches_between(12,11))) Year11 on Artists.ID = Year11.ArtistID left join (select ArtistID, Watches from table(get_count_watches_between(13,12))) Year12 on Artists.ID = Year12.ArtistID left join (select ArtistID, Watches from table(get_count_watches_between(14,13))) Year13 on Artists.ID = Year13.ArtistID left join (select ArtistID, Watches from table(get_count_watches_between(15,14))) Year14 on Artists.ID = Year14.ArtistID left join (select ArtistID, Watches from table(get_count_watches_between(16,15))) Year15 on Artists.ID = Year15.ArtistID order by Artists.ID asc
/

