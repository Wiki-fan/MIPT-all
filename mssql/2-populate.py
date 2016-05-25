import csv
import requests
from random import randrange
import fmtstrs
import bs4
import time


def escape(s):
	return s.replace("'", "''")


number_of_users = 10000
number_of_watches = 1000000
number_of_pictures = 1000000000 # large number, so data will end earlier


def get_rnd_date(begin, end):
	return "CONVERT(DATETIME, '{0}/{1}/{2}', 103)".format(randrange(1, 28), randrange(1, 12), randrange(begin,end))


def populate_users():
	with open('populate_users.sql', "w+", encoding="utf-8") as out:
		i = 0
		for j in range(0, int(number_of_users/100)):
			data = requests.get('https://randomuser.me/api/', params={'results': 100}).json()
			
			for r in data['results']:
				out.write(fmtstrs.users.format(
					ID=i,
					Name="{0} {1}".format(escape(r['name']['first']), escape(r['name']['last'])),
					FirstName=escape(r['name']['first']),
					LastName=escape(r['name']['last']),
					Nickname=escape(r['login']['username']),
					RegistrationDate=get_rnd_date(1990, 2015)
				))
				i += 1
				#time.sleep(0.01)
	print('Users generated')


auxverbs = {'a', 'an', 'the', 'of', 'do', 'at', 'does', 'did', 'has', 'have', 'had', 'is', 'am', 'are', 'was', 'were',
            'be', 'being', 'been', 'may', 'must', 'might', 'should', 'could', 'would', 'shall', 'will', 'can', 'above',
            ' across', ' after', ' at', 'around', ' before', ' behind', 'below', ' beside', ' between', 'by', ' down',
            ' during', ' for', ' from', 'in', ' inside', ' onto', ' of', 'off', ' on', ' out', ' through', 'to',
            ' under', ' up', ' with', 'and', 'or', 'not', 'so'}


def process_names(str):
	if str.find(',') != -1:
		delim = ', '
		first_name = escape(str.split(delim)[1])
		last_name = escape(str.split(delim)[0])
	elif str.find(' ') != -1:
		delim = ' '
		first_name = escape(str.split(delim)[0])
		last_name = escape(str.split(delim)[1])
	else:
		last_name = escape(str)
		first_name = ''
	name = "{0} {1}".format(first_name, last_name)
	return (name, first_name, last_name)


pictures_i = 0
pools_i = 0


# AUTHOR;BORN-DIED;TITLE;DATE;TECHNIQUE;LOCATION;URL;FORM;TYPE;SCHOOL;TIMEFRAME
def populate_pictures_and_artists():
	with open('catalog-utf.csv') as f, \
			open('populate_artists.sql', "w+", encoding="utf-8") as artist_out, \
			open('populate_pictures.sql', 'w+', encoding="utf-8") as picture_out, \
			open('populate_tags.sql', 'w+', encoding="utf-8") as tags_out, \
			open('populate_tags_pictures.sql', 'w+', encoding="utf-8") as tags_pictures_out, \
			open('populate_pools.sql', 'w+', encoding="utf-8") as pools_out, \
			open('populate_pools_pictures.sql', 'w+', encoding="utf-8") as pools_pictures_out:
		reader = csv.reader(f, delimiter=';')
		global pictures_i, pools_i, number_of_pictures
		pools_pictures_i = 0
		tags_i = 0
		tags_pictures_i = 0
		artists_i = 0
		artists = set()
		tags = set()
		pools = set()
		for row in reader:
			(name, first_name, last_name) = process_names(row[0])
			if name not in artists:
				artist_out.write(fmtstrs.artists.format(
					ID=artists_i, Name=name,
					FirstName=first_name,
					LastName=last_name,
					PersonalSite='/'.join(row[6].split('/')[:-1])
				))
				artists_i += 1
				artists.add(name)
			title = escape(row[2])
			picture_out.write(fmtstrs.pictures.format(
				ID=pictures_i, Name=title,
				Description=escape(row[9]),
				Width=randrange(1, 2001), Height=randrange(1, 2001), Rating=randrange(-20, 21),
				ArtistID="(select ID from Artists where name='{0}')".format(name),
				UploaderID=randrange(0, number_of_users),
				DateCreated=get_rnd_date(1900, 1990),
				DateUploaded=get_rnd_date(1990, 2015)
			))
			for tag in title.split(' '):
				if tag not in tags:
					if tag.lower() not in auxverbs:
						tags_out.write(fmtstrs.tags.format(ID=tags_i, Name=tag))
						tags_i += 1
						tags.add(tag)
				tags_pictures_out.write(fmtstrs.tags_pictures.format(
					ID=tags_pictures_i,
					TagID="(select ID from Tags where Name='{0}')".format(tag),
					PictureID=pictures_i
				))
				tags_pictures_i += 1
			pool = escape(row[8])
			if pool not in pools:
				pools_out.write(fmtstrs.pools.format(
					ID=pools_i,
					Name=pool,
					Description="This is a {0}".format(pool)
				))
				pools.add(pool)
				pools_i += 1
			if randrange(0, 10) == 0:
				pools_pictures_out.write(fmtstrs.pools_pictures.format(
					ID=pools_pictures_i,
					PoolID="(select ID from Pools where Name='{0}')".format(pool),
					PictureID=pictures_i
				))
				pools_pictures_i += 1
			pictures_i += 1
			if pictures_i % 1000 == 0:
				picture_out.write('go\n')
				pools_pictures_out.write('go\n')
				tags_pictures_out.write('go\n')
			if pictures_i > number_of_pictures:
				break  # populate_users()
	print('Pictures populated')


sites_i = 110

def populate_sites():
	with open('populate_sites.sql', "w+", encoding="utf-8") as sites_out:
		global sites_i
		for index in range(0, 16):
			soup = bs4.BeautifulSoup(
				requests.get(
					"https://yandex.ru/yaca/cat/Entertainment/community/{0}.html".format(index)).text,
				"html.parser"
			)
			# print (soup.prettify())
			lst = soup.find_all('a', class_="link link_theme_islands yaca-snippet__title-link link__control i-bem")
			# print (lst)
			for i in lst:
				site = i.text
				url = i['href']
				#print (site)
				# print (url)
				sites_i += 1
				sites_out.write(fmtstrs.sites.format(
					ID=sites_i,
					URL=escape(url),
					Name=escape(site)
				))
			time.sleep(5)
	print('Sites populated')


def populate_watches():
	with open('populate_watches.sql', "w+", encoding="utf-8") as watches_out, \
			open('populate_reposts.sql', "w+", encoding="utf-8") as reposts_out:
		global sites_i
		watches_i = 0
		reposts_i = 0
		for i in range(0, number_of_watches):
			is_reposted = randrange(0, 2)
			watches_out.write(fmtstrs.watches.format(
				ID=watches_i,
				UserID=randrange(0, number_of_users),
				PictureID=randrange(0, pictures_i),
				Liked='1' if randrange(0, 2) else '0',
				Reposted='1' if is_reposted else '0',
				SiteID= randrange(0, sites_i) if is_reposted else 'null',
				WatchDate= get_rnd_date(1990, 2015)
			))
			watches_i += 1
			if watches_i % 1000 == 0:
				watches_out.write('go\n')
				reposts_out.write('go\n')
			if is_reposted == 1:
				reposts_out.write(fmtstrs.reposts.format(
					ID=reposts_i,
					WatchID=watches_i,
					Message='This picture is awesome'
				))
				reposts_i += 1
	print('Watches populated')


def populate_childpools():
	with open('populate_childpools.sql', "w+", encoding="utf-8") as childpools_out:
		childpools_i = 0
		checked = set()
		for i in range(0, 11):
			cur = randrange(0, pools_i)
			checked.add(cur)
			for j in range(0, pools_i):
				if randrange(0, 11) == 0 and j not in checked:
					childpools_out.write(fmtstrs.childpools.format(
						ID=childpools_i,
						PoolID=cur,
						ChildPoolID=j
					))

populate_users()
#populate_sites()
populate_pictures_and_artists()
populate_watches()
#populate_childpools()
