#include "my_tcp.hpp"
#include <cstring>
#include <iostream>

MyTCP::MyTCP(const unsigned char* buf) {
    std::memcpy(&header_, buf, sizeof(MyTCP::TcpHeader));
}

bool MyTCP::three_way_handshake_send_syn() {
    std::cout << "flag value: " << (int) header_.flags[0] << std::endl;
    if (header_.flags[0] == 2) {
        return true;
    }
    return false;
}

int MyTCP::get_port(int which) const {
    int port;
    if (which == 0) {
        port = header_.source_port[0];
    } else {
        port = header_.destination_port[0];
    }
    port = port << 8;
    if (which == 0) {
        port |= header_.source_port[1];    
    } else {
        port |= header_.destination_port[1];
    }
    return port;
}

int MyTCP::get_source_port() const {
    return get_port(0);
}

int MyTCP::get_destination_port() const {
    return get_port(1);
}
