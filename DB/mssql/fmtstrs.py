users = """insert into Users (ID,Name,FirstName,LastName,Nickname,RegistrationDate)
values ('{ID}', '{Name}', '{FirstName}','{LastName}','{Nickname}',{RegistrationDate});
"""

artists = """insert into Artists (ID,Name,FirstName,LastName,PersonalSite)
values ('{ID}','{Name}','{FirstName}','{LastName}','{PersonalSite}');
"""

pictures = """insert into Pictures
(ID,Name,Description,Width,Height,Rating,ArtistID,UploaderID,DateCreated,DateUploaded)
values ({ID},'{Name}','{Description}','{Width}','{Height}','{Rating}',{ArtistID},'{UploaderID}',{DateCreated},{DateUploaded});
"""

tags = """insert into Tags (ID, Name) values
({ID}, '{Name}');
"""
pools = """insert into Pools (ID, Name, Description) values
({ID}, '{Name}', '{Description}');
"""

tags_pictures = """insert into Tags_Pictures(ID,TagID,PictureID)
values ({ID}, {TagID}, {PictureID});
"""

pools_pictures = """insert into Pools_Pictures(ID,PoolID,PictureID)
values ({ID}, {PoolID}, {PictureID});
"""

watches = """insert into Watches(ID,UserID,PictureID,Liked,Reposted,SiteID,WatchDate)
values ({ID},{UserID},{PictureID},{Liked},{Reposted},{SiteID},{WatchDate});
"""

sites = """insert into Sites(ID,URL,Name)
values ({ID},'{URL}','{Name}')
"""

reposts = """insert into Reposts(ID,WatchID,Message)
values ({ID},{WatchID},'{Message}')
"""

childpools = """insert into ChildPools(ID,PoolID,ChildPoolID)
values ({ID},{PoolID},{ChildPoolID})
"""

storedproc = """
"""
