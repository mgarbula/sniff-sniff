# Simple packet sniffer

Currently it doesn't do much work. It simply creates raw socket and waits until message comes.

```
make
sudo ./sniff
```
(`sudo` needed to create raw socket)

The goal is to create a packet sniffer which will print the summary of all incoming packets (from who, sizes, etc.)

## Currently implemented
* Receiving frame from layer 2 (ethernet)
* Receiving packet from layer 3 (IP)

## Limitations
* Dropping VLAN frames
* Droppping IPv6 packets
* Dropping packets with options (those are probably dropped by routers anyway, https://en.wikipedia.org/wiki/IPv4#Options)