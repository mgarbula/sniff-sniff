#include <iostream>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <sys/time.h> // for struct timeval
#include "raw_socket.hpp"
#include "setup.hpp"
#include "my_ethhdr.hpp"
#include "my_iphdr.hpp"

#define MAX_BUFFER_SIZE 65536

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
            MyEthhdr eth(buf, nread);
            if (!eth.is_standard()) {
                std::cerr << "WARNING: get vlan frame, dropping\n";
                continue;
            }

            // printf("message received. len = %ld\n", nread);
            // std::cout << "src_mac = " << eth.get_src_mac_str() << std::endl;
            // std::cout << "dst_mac = " << eth.get_dst_mac_str() << std::endl;
            if (eth.get_eth_type() != 0x0800) {
                std::cerr << "WARNING: IPv6 packet, dropping\n";
                continue;
            }
            MyIphdr ip(eth.get_payload_data());
            if (ip.has_options())  {
                std::cerr << "WARNING: IP packet has options, dropping\n";
                continue;
            }
            std::cout << "src_ip = " << ip.get_src_ip() << std::endl;
            std::cout << "dst_ip = " << ip.get_dst_ip() << std::endl;
            std::cout << "data_len = " << ip.get_data_len() << std::endl;
            std::cout << "=========================\n";
        }
    }

}