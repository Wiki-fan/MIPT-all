use Gallery;
drop table Pools_Pictures;
drop table Tags_Pictures;
drop table ChildPools;
drop table Reposts;
drop table Watches;
drop table Pictures;
drop table Artists;
drop table Pools;
drop table Sites;
drop table Tags;
drop table Users;

drop type T_PicturesID;
drop type T_UsersID;
drop type T_PoolsID;
drop type T_TagsID;
drop type T_ArtistsID;
drop type T_SitesID;
drop type T_WatchesID;
drop type T_Pools_PicturesID;
drop type T_Tags_PicturesID;
drop type T_RepostsID;
drop type T_ChildPoolsID;
go
use master;
go
drop database Gallery;
go
