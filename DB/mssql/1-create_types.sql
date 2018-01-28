create database Gallery;
go
use Gallery;
create type T_PicturesID from int not null;
create type T_UsersID from int not null;
create type T_PoolsID from int not null;
create type T_TagsID from int not null;
create type T_ArtistsID from int not null;
create type T_SitesID from int not null;
create type T_WatchesID from int not null;
create type T_Pools_PicturesID from int not null;
create type T_Tags_PicturesID from int not null;
create type T_RepostsID from int;
create type T_ChildPoolsID from int not null;
go
