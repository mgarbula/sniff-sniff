# Simple packet sniffer

Currently it doesn't do much work. It simply creates raw socket and waits until message comes.

```
make
sudo ./sniff
```
(`sudo` needed to create raw socket)

The goal is to create a packet sniffer which will print the summary of all incoming packets (from who, sizes, etc.)
