
class MyTCP {
public:
    #pragma pack(push, 1)
    struct TcpHeader {
        unsigned char source_port[2];
        unsigned char destination_port[2];
        unsigned char sequence_number[4];
        unsigned char acknowledgement_number[4];
        unsigned char offset_reserved[1];
        unsigned char flags[1];
        unsigned char window[2];
        unsigned char checksum[2];
        unsigned char urg_ptr[2];
    };
    #pragma pack(pop)

    MyTCP(const unsigned char*);

    int get_source_port() const;
    int get_destination_port() const;

    bool three_way_handshake_send_syn();
    bool three_way_handshake_receive_syn_ack();
    bool three_way_handshake_send_ack();
private:
    MyTCP::TcpHeader header_;
    int get_port(int) const;
};