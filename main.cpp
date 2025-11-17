#include <iostream>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <sys/time.h> // for struct timeval
#include <sstream>
#include <iomanip>
#include "raw_socket.h"
#include "setup.h"
#include "my_net_structs.h"

#define MAX_BUFFER_SIZE 65536

std::string bytes_to_hex_str(unsigned char* buf) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < 6; i++) {
        ss << std::setw(2) << static_cast<unsigned int>(buf[i]);
        if (i < 5) {
            ss << ":";
        }
    }
    return ss.str();
}

int main() {
    RawSocket socket = RawSocket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    const char* interface_name = "wlp2s0"; // to check this run `ip addr show`
    unsigned int if_index = name_to_index(interface_name);

    my_bind(socket.get_desc(), if_index);

    // TODO: remove when program ready
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    setsockopt(socket.get_desc(), SOL_SOCKET, SO_RCVTIMEO, (const char*) &tv, sizeof(tv));

    struct sockaddr sender_addr;
    socklen_t addr_len = sizeof(sender_addr);
    unsigned char* buf = (unsigned char*) malloc(MAX_BUFFER_SIZE * sizeof(char));

    for (int i = 0; i < 20; i++) {
        ssize_t nread = recvfrom(
            socket.get_desc(),
            buf,
            MAX_BUFFER_SIZE,
            0,
            &sender_addr,
            &addr_len
        );

        if (nread == -1) {
            throw std::system_error(
                errno,
                std::system_category(),
                "recvfrom failed"
            );
        } else {
            struct my_ethhdr* eth = reinterpret_cast<struct my_ethhdr*>(buf);
            std::string dst_mac = bytes_to_hex_str(eth->dst_mac);
            std::string src_mac = bytes_to_hex_str(eth->src_mac);

            printf("message received. len = %ld\n", nread);
            std::cout << "dst_mac = " << dst_mac << std::endl;
            std::cout << "src_mac = " << src_mac << std::endl;
        }
    }

}