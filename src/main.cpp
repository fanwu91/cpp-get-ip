#include <iostream>

#include "ip_mac_util.h"

int main() {
    using namespace IpMac;

    char ip[IP_LEN];
    uint64_t mac;

    if (get_local_ip(ip, IP_LEN, &mac) != 0) {
        return -1;
    }

    std::cout << "IP: " << ip << ", MAC: " << to_mac_str(mac) << std::endl;
    return 0;
}
