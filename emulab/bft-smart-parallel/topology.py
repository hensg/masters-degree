"""A cluster node topology for testing bft-smart with parallel checkpointing.

It uses the disk image 'urn:publicid:IDN+emulab.net+image+ScalableSMR//BFT-SMaRt-PCheckpointD430'
that has the project source code in /srv/EMULAB-RND/

Instructions:
After machine starts, a script to mount the disks will run and you can start
the bft-smart checkpointing experiments.
"""

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
request.setRoutingStyle('static')

portal.context.defineParameter("n", "Number of machines", portal.ParameterType.INTEGER, 4)
portal.context.defineParameter("t", "Type of machines", portal.ParameterType.STRING, 'd430')

params = portal.context.bindParameters()
if params.n < 1 or params.n > 36:
    portal.context.reportError(portal.ParameterError("You must choose at least 1 and no more than 36 machines."))

allowed_types = ["d430","d820"]
if params.t not in allowed_types:
    portal.context.reportError(portal.ParameterError("You must choose one of this types: " + ",".join(allowed_types)))


# Link lan0
link_lan0 = request.LAN('lan0')
link_lan0.setNoBandwidthShaping()
link_lan0.trivial_ok = True
link_lan0.addInterface(iface0)

for i in range(params.n):
    node = request.RawPC('Node'+(i+1))
    node.hardware_type = params.t
    #node.disk_image = "urn:publicid:IDN+emulab.net+image+ScalableSMR//BFT-SMaRt-PCheckpointD430";
    node.addService(rspec.Install(url="https://raw.githubusercontent.com/hensg/masters-degree/main/emulab/bft-smart-parallel/mount_disks.sh", path="/local"))
    node.addService(rspec.Execute(shell="bash", command="/local/mount_disks.sh"))
    iface = node.addInterface('eth0', pg.IPv4Address('10.1.1.'+(i+2),'255.255.255.0'))
    iface.bandwidth = 10000000
    link_lan0.addInterface(iface)

# Print the generated rspec
pc.printRequestRSpec(request)
