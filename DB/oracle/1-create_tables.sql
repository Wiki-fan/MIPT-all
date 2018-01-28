create table Pictures (
ID int primary key not null,
Name varchar(100) not null,
Description varchar(100),
Width int not null constraint CheckWidth
  check (Width > 0),
Height int not null,
constraint CheckHeight
  check (Height > 0),
Rating int not null,
ArtistID int,
UploaderID int,
DateCreated date not null,
DateUploaded date not null
);
create table Users (
ID int primary key not null,
Name varchar(100) not null,
FirstName varchar(100),
LastName varchar(100),
Nickname varchar(100) unique,
RegistrationDate date
);
create table Artists (
ID int primary key not null,
Name varchar(100) unique,
FirstName varchar(100),
LastName varchar(100) not null,
PersonalSite varchar(100)
);
create table Sites (
ID int primary key not null,
URL varchar(100) not null unique,
Name varchar(100) not null unique
);
create table Pools (
ID int primary key not null,
Name varchar(100) not null unique,
Description varchar(100) not null
);
create table ChildPools (
ID int primary key not null,
PoolID int,
ChildPoolID int
);
create table Tags (
ID int primary key not null,
Name varchar(100) not null unique
);
create table Pools_Pictures (
ID int primary key not null,
PoolID int,
PictureID int
);
create table Tags_Pictures (
ID int primary key not null,
TagID int,
PictureID int
);
create table Watches (
ID int primary key not null,
UserID int,
PictureID int,
Liked int not null,
Reposted int not null,
SiteID int null,
WatchDate date not null
);
create table Reposts (
ID int primary key not null,
WatchID int not null,
Message varchar(1024)
);

alter table Pictures add foreign key (ArtistID)
      references Artists(ID);
alter table Pictures add foreign key (UploaderID)
      references Users(ID);
alter table ChildPools add foreign key (PoolID)
      references Pools(ID);
alter table ChildPools add foreign key (ChildPoolID)
      references Pools(ID);
alter table Pools_Pictures add foreign key (PoolID)
      references Pools(ID);
alter table Pools_Pictures add foreign key (PictureID)
      references Pictures(ID);
alter table Tags_Pictures add foreign key (TagID)
      references Tags(ID);      
alter table Tags_Pictures add foreign key (PictureID)
      references Pictures(ID);      
alter table Watches add foreign key (UserID)
      references Users(ID);     
alter table Watches add foreign key (PictureID)
      references Pictures(ID);          
alter table Watches add foreign key (SiteID)
      references Sites(ID);
alter table Reposts add foreign key (WatchID)
      references Watches(ID);
/