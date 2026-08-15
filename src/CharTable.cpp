#include "CharTable.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <iomanip>

static std::string next_utf8_char(const std::string& str, size_t& i) {
    if (i >= str.size()) return "";
    unsigned char c = (unsigned char)str[i];
    size_t len = 1;
    if ((c & 0x80) == 0x00) len = 1;
    else if ((c & 0xE0) == 0xC0) len = 2;
    else if ((c & 0xF0) == 0xE0) len = 3;
    else if ((c & 0xF8) == 0xF0) len = 4;
    else len = 1;
    
    if (i + len > str.size()) len = str.size() - i;
    std::string ch = str.substr(i, len);
    i += len;
    return ch;
}

static std::vector<uint8_t> hex_to_bytes(const std::string& hex) {
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        if (i + 1 < hex.length()) {
            std::string sub = hex.substr(i, 2);
            uint8_t b = (uint8_t)std::stoul(sub, nullptr, 16);
            bytes.push_back(b);
        }
    }
    return bytes;
}

CharTable& CharTable::get_instance() {
    static CharTable instance;
    return instance;
}

CharTable::CharTable() {
    init_default();
}

void CharTable::add_mapping(const std::string& utf8_char, const std::string& hex_val) {
    std::vector<uint8_t> bytes = hex_to_bytes(hex_val);
    if (bytes.empty()) return;

    m_char_to_bytes[utf8_char] = bytes;
    m_bytes_to_char[bytes] = utf8_char;
}

bool CharTable::load_from_json_string(const std::string& content) {
    // Parse key-value pairs of the form "key": "value"
    // Handle escape sequences in string literals
    size_t pos = 0;
    size_t len = content.size();
    int count = 0;

    auto parse_string = [&](size_t& p) -> std::string {
        if (p >= len || content[p] != '"') return "";
        p++; // skip opening quote
        std::string s;
        while (p < len) {
            char c = content[p++];
            if (c == '\\') {
                if (p < len) {
                    char esc = content[p++];
                    if (esc == '"') s += '"';
                    else if (esc == '\\') s += '\\';
                    else if (esc == '/') s += '/';
                    else if (esc == 'b') s += '\b';
                    else if (esc == 'f') s += '\f';
                    else if (esc == 'n') s += '\n';
                    else if (esc == 'r') s += '\r';
                    else if (esc == 't') s += '\t';
                    else s += esc;
                }
            } else if (c == '"') {
                break;
            } else {
                s += c;
            }
        }
        return s;
    };

    while (pos < len) {
        // Find next quote
        while (pos < len && content[pos] != '"') pos++;
        if (pos >= len) break;

        std::string key = parse_string(pos);
        if (key == "char_to_hex") {
            continue;
        }

        // Find colon
        while (pos < len && content[pos] != ':') pos++;
        if (pos >= len) break;
        pos++; // skip colon

        // Find value opening quote
        while (pos < len && content[pos] != '"') pos++;
        if (pos >= len) break;

        std::string val = parse_string(pos);
        if (!key.empty() && !val.empty()) {
            add_mapping(key, val);
            count++;
        }
    }

    // Ensure space ' ' is mapped to 0x20 as well
    if (m_bytes_to_char.count({0x20}) > 0) {
        m_char_to_bytes[" "] = {0x20};
        m_bytes_to_char[{0x20}] = " ";
    } else {
        m_char_to_bytes[" "] = {0x20};
        m_bytes_to_char[{0x20}] = " ";
    }

    if (count > 0) {
        m_initialized = true;
        return true;
    }
    return false;
}

bool CharTable::load_from_file(const std::string& filepath) {
    std::ifstream f(filepath);
    if (!f.is_open()) return false;
    std::string content((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    return load_from_json_string(content);
}

void CharTable::init_default() {
    // Try multiple possible locations for char_to_hex.json
    const std::vector<std::string> search_paths = {
        "src/char_to_hex.json",
        "char_to_hex.json",
        "../src/char_to_hex.json",
        "../../src/char_to_hex.json"
    };

    for (const auto& path : search_paths) {
        if (load_from_file(path)) {
            return;
        }
    }

    // Standard ASCII baseline fallback if no file is found
    for (char c = 0x20; c <= 0x7E; ++c) {
        std::string s(1, c);
        std::stringstream ss;
        ss << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << (int)(uint8_t)c;
        add_mapping(s, ss.str());
    }
    m_char_to_bytes[" "] = {0x20};
    m_bytes_to_char[{0x20}] = " ";
    m_initialized = true;
}

bool CharTable::decode_character(const std::vector<uint8_t>& bytes, size_t pos, std::string& out_char, size_t& out_bytes_consumed) const {
    if (pos >= bytes.size()) return false;

    // Try 2-byte sequence first
    if (pos + 1 < bytes.size()) {
        std::vector<uint8_t> two_bytes = {bytes[pos], bytes[pos + 1]};
        auto it = m_bytes_to_char.find(two_bytes);
        if (it != m_bytes_to_char.end()) {
            out_char = it->second;
            out_bytes_consumed = 2;
            return true;
        }
    }

    // Try 1-byte sequence
    std::vector<uint8_t> one_byte = {bytes[pos]};
    auto it = m_bytes_to_char.find(one_byte);
    if (it != m_bytes_to_char.end()) {
        out_char = it->second;
        out_bytes_consumed = 1;
        return true;
    }

    return false;
}

bool CharTable::encode_string(const std::string& utf8_str, std::vector<uint8_t>& out_bytes) const {
    size_t i = 0;
    while (i < utf8_str.size()) {
        std::string ch = next_utf8_char(utf8_str, i);
        if (ch.empty()) break;

        auto it = m_char_to_bytes.find(ch);
        if (it != m_char_to_bytes.end()) {
            out_bytes.insert(out_bytes.end(), it->second.begin(), it->second.end());
        } else if (ch.size() == 1) {
            // Fallback for unmapped ASCII byte
            out_bytes.push_back((uint8_t)ch[0]);
        } else {
            return false;
        }
    }
    return true;
}

size_t CharTable::encoded_length(const std::string& utf8_str) const {
    size_t total = 0;
    size_t i = 0;
    while (i < utf8_str.size()) {
        std::string ch = next_utf8_char(utf8_str, i);
        if (ch.empty()) break;

        auto it = m_char_to_bytes.find(ch);
        if (it != m_char_to_bytes.end()) {
            total += it->second.size();
        } else {
            total += ch.size();
        }
    }
    return total;
}

bool CharTable::has_char(const std::string& utf8_char) const {
    return m_char_to_bytes.find(utf8_char) != m_char_to_bytes.end();
}

std::vector<uint8_t> CharTable::get_bytes(const std::string& utf8_char) const {
    auto it = m_char_to_bytes.find(utf8_char);
    if (it != m_char_to_bytes.end()) {
        return it->second;
    }
    return {};
}
