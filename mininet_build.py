from mininet.topo import Topo
from mininet.net import Mininet
from mininet.node import Node
from mininet.log import setLogLevel, info
from mininet.cli import CLI
from mininet.link import TCLink
from mininet.util import dumpNodeConnections

class NetworkTopo( Topo ):
    def build( self, **_opts ):
        s1 = self.addSwitch('s1')
        s2 = self.addSwitch('s2')
        s3 = self.addSwitch('s3')
        h1 = self.addHost('h1',ip='10.0.0.1')
        h2 = self.addHost('h2',ip='10.0.10.2')
        h3 = self.addHost('h3',ip='10.0.11.3')
 	h4 = self.addHost('h4',ip='10.0.0.5')
        self.addLink(h1,s1)
        self.addLink(h4,s1)
        self.addLink(s1,s2,bw='10Mbps',loss=0)
        self.addLink(s1,s3,bw='10Mbps',loss=0)
        self.addLink(h2,s2)
        self.addLink(h3,s3)

def run():
    "Test linux hosts"
    topo = NetworkTopo()
    net = Mininet(topo=topo,waitConnected=True)  
    net.start()
    dumpNodeConnections(net.hosts)
    h1,h2,h3,h4 = net.get('h1','h2','h3','h4')
    CLI(net)
    net.stop()


if __name__ == '__main__':
    setLogLevel( 'info' )
    run()

