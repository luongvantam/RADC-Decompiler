#include "replace.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <algorithm>
#include <iomanip>
#include <string>

// Helper to load map from text file
static std::map<std::string, std::string> load_map(const std::string& filename) {
    std::map<std::string, std::string> mapping;
    std::ifstream f(filename);
    if (!f.is_open()) return mapping;

    std::string line;
    while (std::getline(f, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        // Find first whitespace
        size_t space_pos = line.find_first_of(" \t");
        if (space_pos != std::string::npos) {
            std::string addr = line.substr(0, space_pos);
            size_t val_start = line.find_first_not_of(" \t", space_pos);
            if (val_start != std::string::npos) {
                std::string val = line.substr(val_start);
                // uppercase address key
                std::transform(addr.begin(), addr.end(), addr.begin(), ::toupper);
                
                // trim trailing whitespace from val
                size_t val_end = val.find_last_not_of(" \t\r\n");
                if (val_end != std::string::npos) {
                    val = val.substr(0, val_end + 1);
                }
                mapping[addr] = val;
            }
        }
    }
    return mapping;
}

static std::string int_to_hex_str(uint32_t val, int width) {
    std::stringstream ss;
    ss << std::uppercase << std::hex << std::setfill('0') << std::setw(width) << val;
    return ss.str();
}

static std::string check_label_match(uint32_t addr_int, const std::string& model, 
                                     const std::map<std::string, std::string>& gadgets, 
                                     const std::map<std::string, std::string>& labels) {
    std::string addr_str = int_to_hex_str(addr_int, 5);
    
    if (gadgets.find(addr_str) != gadgets.end()) return gadgets.at(addr_str);
    if (labels.find(addr_str) != labels.end()) return labels.at(addr_str);
    
    uint32_t minus2_addr_int = addr_int - 2;
    std::string minus2_addr = int_to_hex_str(minus2_addr_int, 5);
    bool is_push_lr = false;
    
    std::string rom_path = model + "/rom.bin";
    std::ifstream rom_f(rom_path, std::ios::binary);
    if (rom_f.is_open()) {
        rom_f.seekg(minus2_addr_int, std::ios::beg);
        char buf[2];
        if (rom_f.read(buf, 2)) {
            if ((uint8_t)buf[0] == 0xCE && (uint8_t)buf[1] == 0xF8) {
                is_push_lr = true;
            }
        }
    }
    
    if (is_push_lr) {
        if (gadgets.find(minus2_addr) != gadgets.end()) return gadgets.at(minus2_addr);
        if (labels.find(minus2_addr) != labels.end()) return labels.at(minus2_addr);
    }
    
    uint32_t even_addr_int = addr_int & ~1;
    std::string even_addr = int_to_hex_str(even_addr_int, 5);
    if (gadgets.find(even_addr) != gadgets.end()) return gadgets.at(even_addr);
    if (labels.find(even_addr) != labels.end()) return labels.at(even_addr);
    
    return "";
}

static std::vector<std::string> split_lines(const std::string& text) {
    std::vector<std::string> lines;
    std::istringstream iss(text);
    std::string line;
    while (std::getline(iss, line)) {
        lines.push_back(line + "\n");
    }
    return lines;
}

std::string apply_replacements(const std::string& input_text, const std::string& model_dir) {
    // 1. Load config
    std::string labels_name = "labels.txt";
    std::string gadgets_name = "gadgets.txt";
    
    std::ifstream config_f(model_dir + "/config.json");
    if (config_f.is_open()) {
        std::string content((std::istreambuf_iterator<char>(config_f)), std::istreambuf_iterator<char>());
        // crude json parse to find labels/gadgets
        std::regex label_re("\"labels\"\\s*:\\s*\"([^\"]+)\"");
        std::regex gadget_re("\"gadgets\"\\s*:\\s*\"([^\"]+)\"");
        std::smatch m;
        if (std::regex_search(content, m, label_re)) labels_name = m[1];
        if (std::regex_search(content, m, gadget_re)) gadgets_name = m[1];
    }
    
    auto labels = load_map(model_dir + "/" + labels_name);
    auto gadgets = load_map(model_dir + "/" + gadgets_name);
    
    // Check if label exists by value
    auto label_exists_by_value = [&](const std::string& val) {
        for (const auto& kv : labels) {
            if (kv.second == val) return true;
        }
        return false;
    };
    
    auto lines = split_lines(input_text);
    
    // Pass 0
    std::vector<std::string> pass0_lines;
    std::regex hex_re("^(\\s*)hex\\s+([0-9A-Fa-f]{2})\\s+([0-9A-Fa-f]{2})\\s*\n$");
    for (size_t i = 0; i < lines.size();) {
        std::smatch m1, m2;
        if (i + 1 < lines.size() && 
            std::regex_match(lines[i], m1, hex_re) && 
            std::regex_match(lines[i+1], m2, hex_re)) {
            
            std::string indent = m1[1];
            uint32_t b1 = std::stoi(m1[2], nullptr, 16);
            uint32_t b2 = std::stoi(m1[3], nullptr, 16);
            uint32_t b3 = std::stoi(m2[2], nullptr, 16);
            uint32_t b4 = std::stoi(m2[3], nullptr, 16);
            
            uint32_t addr_int = (b4 << 24) | (b3 << 16) | (b2 << 8) | b1;
            std::string mapped = check_label_match(addr_int, model_dir, gadgets, labels);
            
            if (!mapped.empty()) {
                pass0_lines.push_back(indent + mapped + "\n");
                i += 2;
                continue;
            }
        }
        pass0_lines.push_back(lines[i]);
        i++;
    }
    
    // Pass 0.5
    std::vector<std::string> pass05_lines;
    for (const auto& line : pass0_lines) {
        std::smatch m;
        if (std::regex_match(line, m, hex_re)) {
            std::string indent = m[1];
            uint32_t b1 = std::stoi(m[2], nullptr, 16);
            uint32_t b2 = std::stoi(m[3], nullptr, 16);
            uint32_t addr_int = (b2 << 8) | b1;
            
            std::string addr_str = int_to_hex_str(addr_int, 5);
            std::string addr_str_short = int_to_hex_str(addr_int, 4);
            std::string addr_label_name = "ADDR_" + addr_str_short;
            
            std::string mapped = "";
            if (labels.find(addr_str) != labels.end()) mapped = labels[addr_str];
            else if (labels.find(addr_str_short) != labels.end()) mapped = labels[addr_str_short];
            
            if (!mapped.empty()) {
                pass05_lines.push_back(indent + "adr(" + mapped + ")\n");
                continue;
            }
            
            if (label_exists_by_value(addr_label_name) || labels.find(addr_str_short) != labels.end()) {
                pass05_lines.push_back(indent + "adr(" + addr_label_name + ")\n");
                continue;
            }
        }
        pass05_lines.push_back(line);
    }
    
    // Pass 1
    std::vector<std::string> pass1_lines;
    std::regex call_re("^(\\s*)call\\s+([0-9A-Fa-f]+)(\\s*)\n$");
    for (const auto& line : pass05_lines) {
        std::smatch m;
        if (std::regex_match(line, m, call_re)) {
            std::string indent = m[1];
            uint32_t addr_int = std::stoi(m[2], nullptr, 16);
            std::string trailing = m[3];
            
            std::string mapped = check_label_match(addr_int, model_dir, gadgets, labels);
            if (!mapped.empty()) {
                pass1_lines.push_back(indent + mapped + trailing + "\n");
            } else {
                pass1_lines.push_back(line);
            }
        } else {
            pass1_lines.push_back(line);
        }
    }
    
    // Pass 2
    std::string output = "";
    std::regex pop_re("^(\\s*)pop\\s+((er|xr|qr)\\d+)\\s*\n$");
    
    auto starts_with = [](const std::string& s, const std::string& prefix) {
        size_t first_non_space = s.find_first_not_of(" \t");
        if (first_non_space == std::string::npos) return false;
        return s.compare(first_non_space, prefix.size(), prefix) == 0;
    };
    
    for (size_t i = 0; i < pass1_lines.size();) {
        std::smatch m;
        if (std::regex_match(pass1_lines[i], m, pop_re)) {
            std::string indent = m[1];
            std::string reg = m[2];
            std::string rtype = m[3];
            
            int count = 1;
            if (rtype == "xr") count = 2;
            else if (rtype == "qr") count = 4;
            
            std::vector<std::string> ahead_lines;
            bool valid = true;
            for (int j = 0; j < count; j++) {
                if (i + 1 + j < pass1_lines.size()) {
                    std::string next_line = pass1_lines[i + 1 + j];
                    if (starts_with(next_line, "hex ") || starts_with(next_line, "adr(") || starts_with(next_line, "eval(")) {
                        // strip whitespace and newline
                        size_t first_non_space = next_line.find_first_not_of(" \t");
                        size_t last_non_space = next_line.find_last_not_of(" \t\r\n");
                        ahead_lines.push_back(next_line.substr(first_non_space, last_non_space - first_non_space + 1));
                    } else {
                        valid = false;
                        break;
                    }
                } else {
                    valid = false;
                    break;
                }
            }
            
            if (valid) {
                std::string joined = "";
                for (size_t k = 0; k < ahead_lines.size(); k++) {
                    joined += ahead_lines[k];
                    if (k < ahead_lines.size() - 1) joined += ", ";
                }
                output += indent + reg + " = " + joined + "\n";
                i += 1 + count;
                continue;
            }
        }
        output += pass1_lines[i];
        i++;
    }
    
    return output;
}
