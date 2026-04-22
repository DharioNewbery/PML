#ifndef _MONITORING_HPP_
#define _MONITORING_HPP_

#include <iostream>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h> 
#include <cstring>
#include <iomanip>
#include <netpacket/packet.h>

std::string getIPAddress() {
    struct ifaddrs *interfaces = nullptr;
    struct ifaddrs *temp_addr = nullptr;
    std::string ipAddress = "Unknown";

    // Retrieve the current interfaces
    if (getifaddrs(&interfaces) == 0) {
        temp_addr = interfaces;
        while (temp_addr != nullptr) {
            // Check if it is IPv4 (AF_INET) and not a loopback interface
            if (temp_addr->ifa_addr->sa_family == AF_INET) {
                if (std::string(temp_addr->ifa_name) != "lo") {
                    ipAddress = inet_ntoa(((struct sockaddr_in*)temp_addr->ifa_addr)->sin_addr);
                    break; 
                }
            }
            temp_addr = temp_addr->ifa_next;
        }
    }
    freeifaddrs(interfaces);
    return ipAddress;
}



#include <ifaddrs.h>
#include <net/if.h>

std::string getMACAddress() {
    struct ifaddrs *ifaddr, *ifa;
    std::string macAddr = "00:00:00:00:00:00";

    if (getifaddrs(&ifaddr) == -1) return macAddr;

    // Loop through all interfaces
    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        // UP and not Loopback
        if (ifa->ifa_addr == nullptr || (ifa->ifa_flags & IFF_LOOPBACK))
            continue;

        // Check for AF_PACKET (this contains the MAC address on Linux)
        if (ifa->ifa_addr->sa_family == AF_PACKET) {
            struct sockaddr_ll *s = (struct sockaddr_ll *)ifa->ifa_addr;
            if (s->sll_halen == 6) { // MAC addresses are 6 bytes
                std::stringstream ss;
                for (int i = 0; i < 6; ++i) {
                    ss << std::hex << std::setw(2) << std::setfill('0') 
                       << (int)s->sll_addr[i]
                       << (i < 5 ? ":" : "");
                }
                macAddr = ss.str();
                break;
            }

            if (std::string(ifa->ifa_name).find("eth") != std::string::npos || 
                std::string(ifa->ifa_name).find("enp") != std::string::npos || 
                std::string(ifa->ifa_name).find("wlan") != std::string::npos) {
                // This is likely a physical hardware interface
            }
        }
    }

    freeifaddrs(ifaddr);
    std::cout << macAddr << std::endl;
    return macAddr;
}

bool hasInternetAccess() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return false;

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(53); // DNS Port
    inet_pton(AF_INET, "8.8.8.8", &serv_addr.sin_addr);

    // Set a timeout so the function doesn't hang indefinitely
    struct timeval timeout;      
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout));

    bool connected = (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == 0);
    
    close(sock);
    return connected;
}



std::string getSystemDateTime() {
    auto now = std::chrono::system_clock::now();
    
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    
    std::tm* localTime = std::localtime(&currentTime);

    std::stringstream ss;
    ss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
    
    return ss.str();
}

#endif