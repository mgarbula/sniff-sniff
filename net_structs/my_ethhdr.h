#include <memory>
#include <stdexcept>
#include <cstring>
#include <arpa/inet.h>

class MyEthhdr {
public:
    #pragma pack(push, 1)
    struct Header {
        unsigned char dst_mac[6];
        unsigned char src_mac[6];
        unsigned char eth_type[2];
    };
    #pragma pack(pop)

    MyEthhdr(const unsigned char* buf, size_t total_size) {
        unsigned char potential_tag[2];
        std::memcpy(potential_tag, buf + 12, 2);
        uint16_t eth_type_val = ntohs(*(uint16_t*) potential_tag);
        if (eth_type_val == 0x8100) {
            // VLAN
            // TODO: proper handling
            standard_ = false;
        } else {
            standard_ = true;
            if (total_size < sizeof(MyEthhdr::Header)) {
                throw std::runtime_error("Received frame is to small");
            }
            std::memcpy(&header_, buf, sizeof(MyEthhdr::Header));
            payload_len_ = total_size - sizeof(MyEthhdr::Header);
            payload_ = std::make_unique<unsigned char[]>(payload_len_);
            std::memcpy(payload_.get(), buf + sizeof(MyEthhdr::Header), payload_len_);
        }
    }

    const MyEthhdr::Header get_header() const { return header_; }
    const unsigned char* get_payload_data() const { return payload_.get(); }
    size_t get_payload_length() const { return payload_len_; }
    bool is_standard() const { return standard_; }
private:
    MyEthhdr::Header header_;
    std::unique_ptr<unsigned char[]> payload_;
    size_t payload_len_ = 0;
    bool standard_ = false;
};
