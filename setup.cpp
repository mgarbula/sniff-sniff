#include <net/if.h> // for if_nametoindex
#include <string.h> // for memset
#include <linux/if_packet.h> // for struct sockaddr_ll
#include <system_error>
#include <net/ethernet.h>
#include <sys/socket.h>
#include <arpa/inet.h>

unsigned int name_to_index(const char* interface_name) {
    unsigned int if_index = if_nametoindex(interface_name);
    if (if_index == 0) {
        throw std::system_error(
            errno,
            std::system_category(),
            "ERROR during determining interface index"
        );
    }
    return if_index;
}

void my_bind(int sock_desc, unsigned int if_index) {
    struct sockaddr_ll sll;
    memset(&sll, 0, sizeof(sll));
    sll.sll_family = AF_PACKET;
    sll.sll_protocol = htons(ETH_P_ALL);
    sll.sll_ifindex = if_index;

    if (bind(sock_desc, (sockaddr*) &sll, sizeof(sll)) == -1) {
        throw std::system_error(
            errno,
            std::system_category(),
            "ERROR on binding"
        );
    }
}