#include "ip_mac_util.h"

#include <iomanip>
#include <sstream>

namespace IpMac {

int get_local_ip(char* ip_buf, int ip_buf_size, uint64_t* mac) {
    struct ifaddrs* ifaddr = nullptr;
    if (getifaddrs(&ifaddr) == -1) {
        return -1;
    }

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock < 0) {
        return -1;
    }

    int res = -1;
    struct ifreq ifr;
    for (struct ifaddrs* ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        // skip "lo" interface
        if (strcmp(ifa->ifa_name, LO_INTERFACE) == 0) {
            continue;
        }

        // IPV4 only
        if (ifa->ifa_addr == nullptr || ifa->ifa_addr->sa_family != AF_INET) {
            continue;
        }

        if (getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
            ip_buf, ip_buf_size, nullptr, 0, NI_NUMERICHOST) != 0) {
            continue;
        }

        ifr.ifr_addr.sa_family = AF_INET;
        strncpy(ifr.ifr_name, ifa->ifa_name, IFNAMSIZ - 1);

        if (ioctl(sock, SIOCGIFHWADDR, &ifr) != 0) {
            continue;
        }

        *mac = *(uint64_t*)ifr.ifr_hwaddr.sa_data;
        res = 0;
        break;
    }

    close(sock);
    freeifaddrs(ifaddr);

    return res;
}

std::string to_mac_str(uint64_t value) {
    uint8_t bytes[6];

    for (int i = 0; i < 6; ++i) {
        bytes[i] = value & 0xFF;
        value >>= 8;
    }

    std::stringstream ss;
    ss << std::uppercase << std::hex << std::setfill('0');

    for (int i = 0; i < 6; ++i) {
        ss << std::setw(2) << static_cast<int>(bytes[i]);
    }

    return ss.str();
}
} // namespace IpMac
