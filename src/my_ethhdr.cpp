#include "my_ethhdr.hpp"
#include <sstream>
#include <iomanip>

MyEthhdr::MyEthhdr(const unsigned char* buf, size_t total_size) {
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

std::string const MyEthhdr::bytes_to_hex_str_(const unsigned char* buf) const {
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

uint16_t MyEthhdr::get_eth_type() const {
    const uint16_t* eth_type_ptr = reinterpret_cast<const uint16_t*>(header_.eth_type);
    return ntohs(*eth_type_ptr);
}