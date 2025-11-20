#include "my_iphdr.hpp"
#include <sstream>
#include <iomanip>
#include <cstring>

MyIphdr::MyIphdr(const unsigned char* buf) {
    std::memcpy(&header_, buf, sizeof(MyIphdr::Header));
    ihl_ = header_.version_ihl[0] & 15;
    if (ihl_ > 5) {
        has_options_ = true;
    } else {
        // dropping packets with options (https://en.wikipedia.org/wiki/IPv4#Options)
        // that packets are probably dropped by routers anyway
        data_len_ = ((unsigned int) header_.total_length[0] << 8) | (unsigned int) header_.total_length[1];
        data_ = std::make_unique<unsigned char[]>(data_len_);
        std::memcpy(data_.get(), buf + sizeof(MyIphdr::Header), data_len_);
    }
}

std::string const MyIphdr::bytes_to_int_string_(const unsigned char* buf) const {
    std::stringstream ss;
    ss << std::dec << std::setfill('0');
    for (int i = 0; i < 4; i++) {
        ss << static_cast<unsigned int>(buf[i]);
        if (i < 3) {
            ss << ".";
        }
    }
    return ss.str();
}
