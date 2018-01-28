use Gallery;
go
create table Pictures (
ID T_PicturesID primary key not null,
Name varchar(100) not null,
Description varchar(100),
Width int not null constraint CheckWidth
  check (Width > 0),
Height int not null,
constraint CheckHeight
  check (Height > 0),
Rating int not null,
ArtistID T_ArtistsID,
UploaderID T_UsersID,
DateCreated date not null,
DateUploaded date not null
);
create table Users (
ID T_UsersID primary key not null,
Name varchar(100) not null,
FirstName varchar(100),
LastName varchar(100),
Nickname varchar(100) unique,
RegistrationDate date
);
create table Artists (
ID T_ArtistsID primary key not null,
Name varchar(100) not null unique,
FirstName varchar(100) not null,
LastName varchar(100) not null,
PersonalSite varchar(100)
);
create table Sites (
ID T_SitesID primary key not null,
URL varchar(100) not null unique,
Name varchar(100) not null unique
);
create table Pools (
ID T_PoolsID primary key not null,
Name varchar(100) not null unique,
Description varchar(100) not null
);
create table ChildPools (
ID T_ChildPoolsID primary key not null,
PoolID T_PoolsID,
ChildPoolID T_ChildPoolsID
);
create table Tags (
ID T_TagsID primary key not null,
Name varchar(100) not null unique
);
create table Pools_Pictures (
ID T_Pools_PicturesID primary key not null,
PoolID T_PoolsID,
PictureID T_PicturesID
);
create table Tags_Pictures (
ID T_Tags_PicturesID primary key not null,
TagID T_TagsID,
PictureID T_PicturesID
);
create table Watches (
ID T_WatchesID primary key not null,
UserID T_UsersID,
PictureID T_PicturesID,
Liked bit not null,
Reposted bit not null,
SiteID T_SitesID null,
WatchDate date not null
);
create table Reposts (
ID T_RepostsID primary key not null,
WatchID T_WatchesID not null,
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
