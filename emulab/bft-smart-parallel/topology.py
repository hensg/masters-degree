# Import the Portal object.
import geni.portal as portal
# Import the ProtoGENI library.
import geni.rspec.pg as pg
# Import the Emulab specific extensions.
import geni.rspec.emulab as emulab

# Create a portal object,
pc = portal.Context()

# Create a Request object to start building the RSpec.
request = pc.makeRequestRSpec()

# Node node1
node1 = request.RawPC('node1')
node1.hardware_type = 'd430'
node1.disk_image = "urn:publicid:IDN+emulab.net+image+ScalableSMR//CheckpointExperimentosD430";

iface0 = node1.addInterface('eth0', pg.IPv4Address('10.1.1.2','255.255.255.0'))

# Node node2
node2 = request.RawPC('node2')
node2.hardware_type = 'd430'
node2.disk_image = "urn:publicid:IDN+emulab.net+image+ScalableSMR//CheckpointExperimentosD430";
iface1 = node2.addInterface('eth0', pg.IPv4Address('10.1.1.3','255.255.255.0'))

# Node node3
node3 = request.RawPC('node3')
node3.hardware_type = 'd430'
node3.disk_image = "urn:publicid:IDN+emulab.net+image+ScalableSMR//CheckpointExperimentosD430";
iface2 = node3.addInterface('eth0', pg.IPv4Address('10.1.1.4','255.255.255.0'))

# Node node4
node4 = request.RawPC('node4')
node4.hardware_type = 'd430'
node4.disk_image = "urn:publicid:IDN+emulab.net+image+ScalableSMR//CheckpointExperimentosD430";
iface3 = node4.addInterface('eth0', pg.IPv4Address('10.1.1.5','255.255.255.0'))

# Link lan0
link_lan0 = request.LAN('lan0')
link_lan0.setNoBandwidthShaping()
link_lan0.trivial_ok = True
iface0.bandwidth = 10000000
link_lan0.addInterface(iface0)
iface1.bandwidth = 10000000
link_lan0.addInterface(iface1)
iface2.bandwidth = 10000000
link_lan0.addInterface(iface2)
iface3.bandwidth = 10000000
link_lan0.addInterface(iface3)

request.setRoutingStyle('static')

# Print the generated rspec
pc.printRequestRSpec(request)
