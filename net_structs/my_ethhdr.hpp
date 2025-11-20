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

    MyEthhdr(const unsigned char*, size_t);
    const MyEthhdr::Header get_header() const { return header_; }
    const unsigned char* get_payload_data() const { return payload_.get(); }
    size_t get_payload_length() const { return payload_len_; }
    bool is_standard() const { return standard_; }
    std::string get_src_mac_str() const { return bytes_to_hex_str_(header_.src_mac); }
    std::string get_dst_mac_str() const { return bytes_to_hex_str_(header_.dst_mac); }
    uint16_t get_eth_type() const;
private:
    std::string const bytes_to_hex_str_(const unsigned char*) const;
    MyEthhdr::Header header_;
    std::unique_ptr<unsigned char[]> payload_;
    size_t payload_len_ = 0;
    bool standard_ = false;
};
