#include <iostream>
#include <memory>

enum protocol {
    UNDEFINED = 0,
    ICMP = 1,
    IGMP = 2,
    TCP = 6,
    UDP = 17
};

class MyIphdr {
public:
    #pragma pack(push, 1)
    struct Header {
        unsigned char version_ihl[1]; // 4 bits + 4 bits
        unsigned char tos[1];
        unsigned char total_length[2];
        unsigned char identification[2];
        unsigned char flags_frag_offset[2]; // 3 bits + 13 bits
        unsigned char ttl[1];
        unsigned char protocol[1];
        unsigned char hdr_checksum[2];
        unsigned char src_ip[4];
        unsigned char dst_ip[4];
    };
    #pragma pack(pop)

    MyIphdr(const unsigned char*);
    bool has_options() const { return has_options_; }
    unsigned short get_ihl() const { return ihl_; }
    size_t get_data_len() const { return data_len_; }
    std::string get_src_ip() const { return bytes_to_int_string_(header_.src_ip); }
    std::string get_dst_ip() const { return bytes_to_int_string_(header_.dst_ip); }
    protocol get_protocol() const;
    const unsigned char* get_data() const { return data_.get(); }
private:
    std::string const bytes_to_int_string_(const unsigned char*) const;
    MyIphdr::Header header_;
    std::unique_ptr<unsigned char[]> data_;
    size_t data_len_ = 0;
    bool has_options_ = false;
    unsigned int ihl_;
};