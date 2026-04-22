#include <cppsocket.hpp>
#include <string>
#include <chrono>
#include <thread>
#include <fstream>

#include "cppheader.hpp"
#include "monitoring.hpp"

int main() {

    std::ifstream config("config.txt");
    if (!config) {
        std::cerr << "Could not open config file\n";
        return 1;
    }

    std::string ip;
    uint16_t port;
    int pingTime;
    config >> ip >> port >> pingTime;

    cppheader::Header log { "mac", "ip", "hasInternet"};
    
    auto mac = getMACAddress();
    log.set("mac", mac);
    log.set("ip", getIPAddress());

    while (true) {
        log.set("hasInternet", std::to_string(hasInternetAccess()));
        
        std::clog << "sending data to server at: " << ip << ":" << port << "\n";
        
        try {
            auto server = cppsocket::connect(ip, port);
            std::string buffer = log.to_string();
            server.send(buffer);
        } catch (std::exception &e) {
            std::cerr << e.what();
        }

        sleep(pingTime);
    }
}
