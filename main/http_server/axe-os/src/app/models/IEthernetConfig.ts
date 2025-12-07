export interface IEthernetConfig {
  networkMode: string;      // 'wifi' | 'ethernet'
  ethAvailable: number;     // 1 if W5500 detected, 0 otherwise
  ethLinkUp: number;        // 1 if PHY link up, 0 if cable disconnected
  ethConnected: number;     // 1 if has IP address, 0 otherwise
  ethIPv4: string;          // Ethernet IP address
  ethMac: string;           // Ethernet MAC address
  ethUseDHCP: number;       // 1 = DHCP, 0 = static IP
  ethStaticIP: string;      // Static IP address setting
  ethGateway: string;       // Gateway IP setting
  ethSubnet: string;        // Subnet mask setting
  ethDNS: string;           // DNS server setting
}
