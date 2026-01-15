struct ethernet_header {
    unsigned char dest_mac[6];
    unsigned char src_mac[6];
    unsigned short ethertype;
};

struct ethernet_frame {
    struct ethernet_header header;
    unsigned char payload[1500];
    unsigned char frame_check_sequence[4];
};