export interface IEthernetConfig {
  networkMode: string;      // 'wifi' | 'ethernet'
  ethAvailable: number;     // Hardware detection status
  ethLinkUp: number;        // Physical cable connection status
  ethConnected: number;     // IP assignment status
  ethIPv4: string;          // Current Ethernet IP
  ethMac: string;           // Ethernet MAC address
  ethUseDHCP: number;       // 1 = DHCP, 0 = static IP
  ethStaticIP: string;      // Static IP address setting
  ethGateway: string;       // Gateway IP setting
  ethSubnet: string;        // Subnet mask setting
  ethDNS: string;           // DNS server setting
}
