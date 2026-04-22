#include <cppsocket.hpp>
#include <string>
#include <map>
#include <fstream>
#include <filesystem>

#include "cppheader.hpp"
#include "monitoring.hpp"

#define fs std::filesystem

class StatusRegistry {
public:
    StatusRegistry(const fs::path &logsPath): m_logsPath(logsPath) {}
    
    void newLog(std::string mac, cppheader::Header&& log) {
        if (m_logs.count(mac) == 0)
            m_logs.emplace(mac, std::move(log));
        else
            m_logs[mac] = std::move(log);

        writeLog(m_logsPath / mac, log.to_string());
    }

private:

    void writeLog(fs::path logPath, const std::string &text) {
        
        std::ofstream logsFile(logPath, std::ios_base::app);
        if (!logsFile) 
            throw std::runtime_error("Not able to open logsFile");

        std::string time = getSystemDateTime();
        logsFile << time << "\n" << text << "\n";
    }

    std::map<std::string, cppheader::Header> m_logs;
    fs::path m_logsPath;
};


int main() {
    fs::path logsPath = fs::current_path().parent_path() / "logs";
    fs::create_directory(logsPath);

    cppsocket::Acceptor acceptor(8080);
    StatusRegistry sr(logsPath);
    
    std::clog << "starting server\n";
    while (true) {
        auto client = acceptor.accept();

        std::string buffer;
        client.recv(buffer);

        auto header = cppheader::parse_header(buffer);
        std::clog << "recieved data from: " << header.get("mac") << "\n";

        sr.newLog(header.get("mac"), std::move(header));
    }
}
