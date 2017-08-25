import sunspec.core.data as data
import sunspec.core.util as util
import sunspec.core.device as device
import xml.etree.ElementTree as ET

tree = ET.parse('test_obj.xml')

models = tree.getiterator(data.SDX_MODEL)

print 'List one model'
m = models.pop()
print ET.tostring(m)

mt = device.ModelType().from_smdx(m)

print mt