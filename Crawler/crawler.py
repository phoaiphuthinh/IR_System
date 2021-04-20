import requests
import time
import datetime
from bs4 import BeautifulSoup as bs
from selenium import webdriver
from dateutil.parser import parse
from dateutil.relativedelta import *
base = 'https://vnexpress.net/'
driver = webdriver.Chrome('./chromedriver')

#define all categories in VnExpress
cate = ['thoi-su','the-gioi','kinh-doanh', 'giai-tri', 'the-thao', 'phap-luat', 'giao-duc', 'suc-khoe', 'doi-song', 'du-lich', 'khoa-hoc', 'so-hoa', 'oto-xe-may']

def gap(date):

    '''
    Calculating the day gap between date and today

    '''

    now = datetime.datetime.today()
    date = datetime.datetime.strptime(date, '%d/%m/%Y')
    delta = now - date
    return delta.days


def getDate(url):

    '''
    Getting the date from the news, url

    '''


    driver.get(url)
    page = bs(driver.page_source, 'html.parser')
    content = page.find_all('span', {'class' : 'date'})
    if len(content) > 0:
        info = content[0].text.strip()
        info = info.split(',')
        while info[1][0] == ' ':
            info[1] = info[1][1:]
        #Due to the format in VnExpress, the date is the second part
        return info[1]
    return None


def crawl(url):

    '''
    Getting all titles available in the given page, url

    '''

    titles = []
    driver.get(url)
    time.sleep(5)
    page = bs(driver.page_source, 'html.parser')
    block = page.find_all(class_='title-news')

    for element in block:
        title = element.find('a')
        if title.get('target', '') == '' and title['href'][0] != '/':
            date = getDate(title['href'])
            if date is not None:
                g = gap(date)
                if g <= 7:
                    titles.append(title.text.strip())
                else:
                    return titles, None
    return titles, []


#Main function
'''
The flow as followed:
    Go to all categories
    For each category, try to get all titles in the page
    If can go on, try to go to next page
    Go on if the last title to be retrieved is published within a week.

'''

titles = []
for category in cate:
    url = base + category
    title, legal = crawl(url)
    titles.extend(title)
    next_page = 2
    while legal is not None :
        URL = url + '-p' + str(next_page)
        title, legal = crawl(URL)
        titles.extend(title)
        next_page += 1


#Write to file
with open('18125044.txt', 'w') as f:
    for title in titles:
        f.write(title + '\n')
