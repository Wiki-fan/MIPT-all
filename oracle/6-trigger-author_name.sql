create or replace trigger AfterArtistsInsertTrigger 
before insert on Artists
for each row
declare
name varchar(100);
begin
  /*select get_name_from_first_and_last(:new.FirstName, :new.LastName) into name from dual ;
	insert into Artists(ID,Name,FirstName,LastName,PersonalSite)
	values (:new.ID, get_name_from_first_and_last(:new.FirstName, :new.LastName), 
  :new.FirstName, :new.LastName, :new.PersonalSite);*/
  select get_name_from_first_and_last(:new.FirstName, :new.LastName) into :new.Name from dual;
  /*update Artists
  set Name = get_name_from_first_and_last(:new.FirstName, :new.LastName)
  where ID = :new.ID;*/
end;
/
insert into Artists (ID, FirstName, LastName, PersonalSite) values
((select MAX(ID)+1 from Artists), 'TestFirstName', 'TestLastName', 'http://google.com/');
/
select * from Artists where FirstName = 'TestFirstName'
/
delete from Artists where Name = 'TestFirstName TestLastName'
/
