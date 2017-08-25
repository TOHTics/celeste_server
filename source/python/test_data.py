import sys
import os
import time
import unittest

import sunspec.core.data as data
import sunspec.core.util as util


t = 1387560564.48
expected_xml = '<sunSpecData><d lid="00:00:00:00:00:01" man="Man" mod="Mod" sn="SN" t="2013-12-20T17:29:24Z"><m id="1"><p id="P1">23</p><p id="P2">24</p></m></d></sunSpecData>'

sd = data.SunSpecData()
# def __init__(self, logger_id=None, namespace=None, device_id=None, ifc=None, man=None, mod=None, sn=None, time=None, cid=None):
d = sd.device_add(logger_id='00:00:00:00:00:01', man='Man', mod='Mod', sn='SN', timestamp=t)
m = d.model_add(1)
m.point_add("P1", 23)
m.point_add("P2", 24)

xml = sd.to_xml_str()

if xml != expected_xml:
	raise Exception('XML mismatch: %s %s' % (xml, expected_xml))
else:
	print xml