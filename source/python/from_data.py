import sunspec.core.data as data
import sunspec.core.util as util
import xml.etree.ElementTree as ET


# tree = ET.parse('test_obj.xml')
# root = tree.getroot()
#
# d = data.SunSpecData().from_xml(element=root)

# read xml
# f = open('test_obj.xml', 'r')
# xml = f.read()
#
# sd = data.SunSpecData()
# sd.from_xml(data_record=xml)

print sd.to_xml_str()
