#ifndef __CPPHEADER_HPP__
#define __CPPHEADER_HPP__

#include <iostream>
#include <map>

namespace cppheader {

inline const static char SEPARATOR  = '=';
inline const static char PUNCTUATOR = '\n';

class Header {
public:
    Header() = default;
    Header(const std::initializer_list<std::string> &_keys) {
        for (auto key : _keys) { m_map[key] = ""; }
    }
    Header(const std::map<std::string, std::string> &map) {
        m_map = map;
    }
    Header(Header &other) noexcept { m_map = other.m_map; }
    Header(Header &&other) noexcept { m_map = other.m_map; }
    Header& operator=(Header &other) noexcept {
        m_map = other.m_map;
        return *this;
    }
    Header& operator=(Header &&other) noexcept {
        m_map = other.m_map;
        return *this;
    }

    ~Header() {}

    void set(const std::string &key, const std::string &value) {
        if (m_map.count(key) != 0) m_map[key] = value;
        else throw std::runtime_error("given key '" + key + "' not in header structure");
    }

    std::string get(const std::string &key) {
        if (m_map.count(key) != 0) return m_map[key];
        throw std::runtime_error("given key '" + key + "' not in header structure");
    }

    std::string to_string() {
        std::string result = "";
        for (auto pair : m_map) { result += pair.first + SEPARATOR + pair.second + PUNCTUATOR; }
        return result;
    }

private:
    std::map<std::string, std::string> m_map = {};
};

Header create_header(const std::initializer_list<std::string> &_keys) {
    return Header(_keys);
}

Header parse_header(const std::string &raw_header) {
    
    std::map<std::string, std::string> values;
    size_t pos = 0;
    
    while (pos < raw_header.size()) {

        size_t end_pos = raw_header.find(PUNCTUATOR, pos);
        if (end_pos == std::string::npos) end_pos = raw_header.size();
        
        std::string line = raw_header.substr(pos, end_pos - pos);
        size_t colon_pos = line.find(SEPARATOR);
        
        if (colon_pos != std::string::npos) {
            std::string key = line.substr(0, colon_pos);
            std::string value = line.substr(colon_pos + 1);
            values[key] = value;
        }
        
        pos = end_pos + 1;
    }

    return Header(values);
}

}

#endif