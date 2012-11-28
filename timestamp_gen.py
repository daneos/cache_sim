#!/usr/bin/python

from datetime import datetime 
from calendar import timegm

try:
	date = raw_input()
	while date:
		print timegm(datetime.strptime(date, '[%d/%b/%Y:%H:%M:%S').utctimetuple())
		date = raw_input()
except Exception:
	pass