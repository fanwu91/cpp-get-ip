#ifndef _IP_MAC_UTIL_H_
#define _IP_MAC_UTIL_H_

#include <linux/if.h>
#include <cstring>
#include <cstdio>
#include <ifaddrs.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <string>
#include <unistd.h>

namespace IpMac {

inline constexpr int IP_LEN = 20;
inline constexpr char LO_INTERFACE[3] = "lo";

int get_local_ip(char* ip_buf, int ip_buf_size, uint64_t* mac);
std::string to_mac_str(uint64_t val);

} // namespace IpMac

#endif // _IP_MAC_UTIL_H_
