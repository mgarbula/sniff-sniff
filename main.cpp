#include <iostream>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <sys/time.h> // for struct timeval
#include "raw_socket.h"
#include "setup.h"

#define MAX_BUFFER_SIZE 65536

int main() {
    RawSocket socket = RawSocket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    const char* interface_name = "wlp2s0"; // to check this run `ip addr show`
    unsigned int if_index = name_to_index(interface_name);

    my_bind(socket.get_desc(), if_index);

    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    setsockopt(socket.get_desc(), SOL_SOCKET, SO_RCVTIMEO, (const char*) &tv, sizeof(tv));

    struct sockaddr sender_addr;
    socklen_t addr_len = sizeof(sender_addr);
    char buf[MAX_BUFFER_SIZE];
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
        printf("message received. len = %ld\n", nread);
    }

}