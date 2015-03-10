#!/usr/bin/env python

import sys
import json
import urllib2
import base64


URL = 'http://localhost:3000/upload'


def http_send_json(url, data):
    req = urllib2.Request(url)
    req.add_header('Content-Type', 'application/json')
    res = urllib2.urlopen(req, json.dumps(data))
    return res.read()


def load_file(fname):
    with open(fname) as f:
        return f.read()


def base64_encode_file(fname):
    f = load_file(fname)
    return base64.b64encode(f)


def main():
    fname = sys.argv[1]
    buff = base64_encode_file(fname)
    data = {
        'user': 'brian',
        'filedata': buff,
        'filename': fname,
    }

    print 'sending data {} to server {}'.format(data, URL)

    http_send_json(URL, data)


main()
