﻿#!/usr/bin/python
# -*- coding: utf-8 -*-

import lplayer
import re
from lplayer_utils import list_links, parse_flvcd_page

def parse(url, options):
    origin_url = url
    url = 'http://www.flvcd.com/parse.php?go=1&kw=' + origin_url
    if options & lplayer.OPT_QL_SUPER:
        url += '&format=super'
    elif options & lplayer.OPT_QL_HIGH:
        url += '&format=high'
    print url
    lplayer.get_url(url, parse_cb, (options, origin_url))
    
## Parse videos
cantonese_re = re.compile(r'''<a [^>]*href=['"](.+?_lang=1.*?)['"]''')
def parse_cb(page, data):
    options = data[0]
    url = data[1]
    match = cantonese_re.search(page)
    if match and not '_lang=1' in lplayer.final_url:
        if lplayer.question('是否解析为粤语版？'):
            url = match.group(1)
            if not url.startswith('http://'):
                url = 'http://www.flvcd.com/' + url
            url += '&go=1'
            lplayer.get_url(url, parse_cb, data)
            return
        lplayer.warn('解析为粤语版失败！')
    result = parse_flvcd_page(page, None)
    if len(result) == 0:
        lplayer.warn('Cannot parse this video:\n' + url)
    elif options & lplayer.OPT_DOWNLOAD:
        lplayer.download(result, result[0])
    else:
        lplayer.play(result)
