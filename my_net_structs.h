struct my_ethhdr {
    unsigned char dst_mac[6];
    unsigned char src_mac[6];
    unsigned char tag[4];
    unsigned char eth_type[2];
    unsigned char payload[1500];
};