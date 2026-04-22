#include <cppsocket.hpp>
#include <string>
#include <chrono>
#include <thread>
#include <fstream>

#include "cppheader.hpp"
#include "monitoring.hpp"

int main() {

    int pingTime = 10;

    std::string ip = "127.0.0.1";
    int port = 8080;

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
