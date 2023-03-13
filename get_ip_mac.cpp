#include <linux/if.h>
#include <cstring>
#include <cstdio>
#include <ifaddrs.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <unistd.h>

namespace {
    constexpr int  IP_LEN = 20;
    constexpr char LO_INTERFACE[3] = "lo";

    int get_local_ip(char* ip_buf, int ip_buf_size, uint64_t* mac) {
        struct ifaddrs* ifaddr = nullptr;
        if (getifaddrs(&ifaddr) == -1) {
            return -1;
        }

        int res = -1;
        for (struct ifaddrs* ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
            // skip "lo" interface
            if (strcmp(ifa->ifa_name, LO_INTERFACE) == 0) {
                continue;
            }

            // IPV4
            if (ifa->ifa_addr != nullptr && ifa->ifa_addr->sa_family == AF_INET) {
                int s = getnameinfo(ifa->ifa_addr, sizeof(struct socktaddr_in),
                    ip_buf, ip_buf_size, nullptr, 0, NI_NUMERICHOST);
                
                if (s != 0) {
                    continue;
                }

                int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
                if (sock < 0) {
                    continue;
                }

                struct ifreq ifr;
                ifr.ifr_addr.sa_family = AF_INET;
                strncpy(ifr.ifr_name, ifa->ifa_name, IFNAMSIZ - 1);
                ioctl(sock, SIOCGIFHWADDR, &ifr);
                close(sock);

                *mac = *(uint64_t*)ifr.ifr_hwaddr.sa_data;
                res = 0;
                break;
            }
        }

        freeifaddrs(ifaddr);
        return res;
    }
} // namespace ("anonymous")

int main() {
    char ip[IP_LEN];
    uint64_t mac;
    
    if (get_local_ip(ip, IP_LEN, &mac) != 0) {
        return -1;
    }

    unsigned char* m = (unsigned char*)(&mac);
    printf("IP: %s, MAC: %.2X%.2X%.2X%.2X%.2X%.2X\n", ip, m[0], m[1], m[2], m[3], m[4], m[5]);
}
