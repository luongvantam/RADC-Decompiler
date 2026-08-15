#include "compiler.hpp"
#include "CharTable.hpp"
#include <sstream>
#include <iostream>
#include <fstream>
#include <regex>
#include <algorithm>

static std::map<std::string, uint32_t> load_map_reversed(const std::string& filename) {
    std::map<std::string, uint32_t> mapping;
    std::ifstream f(filename);
    if (!f.is_open()) return mapping;

    std::string line;
    while (std::getline(f, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        size_t space_pos = line.find_first_of(" \t");
        if (space_pos != std::string::npos) {
            std::string addr_str = line.substr(0, space_pos);
            size_t val_start = line.find_first_not_of(" \t", space_pos);
            if (val_start != std::string::npos) {
                std::string val = line.substr(val_start);
                size_t val_end = val.find_last_not_of(" \t\r\n");
                if (val_end != std::string::npos) {
                    val = val.substr(0, val_end + 1);
                }
                uint32_t addr = std::stoul(addr_str, nullptr, 16);
                mapping[val] = addr;
            }
        }
    }
    return mapping;
}

void RopCompiler::load_gadgets(const std::map<uint32_t, std::string>& gadgets_by_addr) {
    for (const auto& pair : gadgets_by_addr) {
        m_gadgets[pair.second] = pair.first;
    }
}

void RopCompiler::load_labels(const std::map<uint32_t, std::string>& labels_by_addr) {
    for (const auto& pair : labels_by_addr) {
        m_labels[pair.second] = pair.first;
    }
}

void RopCompiler::load_gadgets_file(const std::string& path) {
    auto m = load_map_reversed(path);
    m_gadgets.insert(m.begin(), m.end());
}

void RopCompiler::load_labels_file(const std::string& path) {
    auto m = load_map_reversed(path);
    m_labels.insert(m.begin(), m.end());
}

static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

static void emit_u32(std::vector<uint8_t>& buf, uint32_t val) {
    buf.push_back(val & 0xFF);
    buf.push_back((val >> 8) & 0xFF);
    buf.push_back((val >> 16) & 0xFF);
    buf.push_back((val >> 24) & 0xFF);
}

static void emit_u16(std::vector<uint8_t>& buf, uint16_t val) {
    buf.push_back(val & 0xFF);
    buf.push_back((val >> 8) & 0xFF);
}

std::vector<uint8_t> RopCompiler::compile(const std::string& source) {
    std::vector<uint8_t> result;
    
    std::istringstream iss(source);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(iss, line)) {
        trim(line);
        if (!line.empty()) {
            lines.push_back(line);
        }
    }
    
    std::vector<std::string> flat_lines;
    for (auto& l : lines) {
        if (m_gadgets.count(l) || m_labels.count(l)) {
            flat_lines.push_back(l);
            continue;
        }

        size_t eq_pos = l.find(" = ");
        if (eq_pos != std::string::npos) {
            std::string reg = l.substr(0, eq_pos);
            trim(reg);
            if (std::regex_match(reg, std::regex("^(er|xr|qr)\\d+$"))) {
                flat_lines.push_back("call pop " + reg);
                std::string rhs = l.substr(eq_pos + 3);
                
                int paren_depth = 0;
                std::string current_part = "";
                for (char c : rhs) {
                    if (c == '(') paren_depth++;
                    else if (c == ')') paren_depth--;
                    else if (c == ',' && paren_depth == 0) {
                        trim(current_part);
                        if (!current_part.empty()) flat_lines.push_back(current_part);
                        current_part = "";
                        continue;
                    }
                    current_part += c;
                }
                trim(current_part);
                if (!current_part.empty()) flat_lines.push_back(current_part);
            } else {
                flat_lines.push_back(l);
            }
        } else {
            flat_lines.push_back(l);
        }
    }
    
    uint32_t section_start = 0;
    uint32_t section_backup = 0;
    uint32_t global_offset = 0;
    
    // Pass 1: Parse sections and labels
    uint32_t current_offset = 0;
    for (auto& l : flat_lines) {
        if (l.rfind("@section.main at 0x", 0) == 0) {
            std::regex sec_re("@section\\.main at 0x([0-9a-fA-F]+) backup 0x([0-9a-fA-F]+)");
            std::smatch match;
            if (std::regex_search(l, match, sec_re)) {
                section_start = std::stoul(match[1].str(), nullptr, 16);
                section_backup = std::stoul(match[2].str(), nullptr, 16);
            }
        } else if (l.rfind("@offset 0x", 0) == 0) {
            global_offset = std::stoul(l.substr(10), nullptr, 16);
        } else if (l.rfind("lbl ", 0) == 0) {
            std::string lbl_name = l.substr(4);
            trim(lbl_name);
            m_local_labels[lbl_name] = section_start + current_offset;
        } else if (l.rfind("hex ", 0) == 0) {
            std::string hexes = l.substr(4);
            std::istringstream hss(hexes);
            std::string hb;
            while (hss >> hb) {
                current_offset++;
            }
        } else if (l.rfind("call ", 0) == 0) {
            current_offset += 4;
        } else if (l.rfind("adr(", 0) == 0) {
            current_offset += 2;
        } else if (l.rfind("eval(", 0) == 0) {
            current_offset += 2;
        } else if (l.length() >= 2 && l.front() == '"' && l.back() == '"') {
            std::string str_val = l.substr(1, l.length() - 2);
            current_offset += CharTable::get_instance().encoded_length(str_val);
        } else if (m_gadgets.count(l) || m_labels.count(l)) {
            current_offset += 4;
        } else {
            // Unknown line, assume 4-byte gadget for offsets
            current_offset += 4;
        }
    }
    
    auto resolve_label = [&](const std::string& name) -> uint32_t {
        if (m_local_labels.count(name)) return m_local_labels[name];
        if (m_labels.count(name)) return m_labels[name];
        if (name.rfind("ADDR_", 0) == 0) {
            return std::stoul(name.substr(5), nullptr, 16);
        }
        return 0;
    };
    
    // Pass 2: Emit bytes
    for (auto& l : flat_lines) {
        if (l.rfind("@section", 0) == 0) continue;
        // if (l.rfind("@offset", 0) == 0) continue;
        if (l.rfind("lbl ", 0) == 0) continue;
        
        if (l.rfind("hex ", 0) == 0) {
            std::string hexes = l.substr(4);
            std::istringstream hss(hexes);
            std::string hb;
            while (hss >> hb) {
                result.push_back((uint8_t)std::stoul(hb, nullptr, 16));
            }
        } else if (l.rfind("call ", 0) == 0) {
            std::string cmd = l.substr(5);
            trim(cmd);
            uint32_t addr = 0;
            if (m_gadgets.count(cmd)) {
                addr = m_gadgets[cmd] + global_offset;
            } else {
                try {
                    addr = std::stoul(cmd, nullptr, 16) + global_offset;
                } catch(...) {}
            }
            emit_u32(result, addr);
        } else if (l.rfind("adr(", 0) == 0) {
            std::regex adr_re("adr\\(([^)]+)\\)");
            std::smatch match;
            if (std::regex_search(l, match, adr_re)) {
                uint32_t val = resolve_label(match[1].str());
                emit_u16(result, val & 0xFFFF);
            } else {
                emit_u16(result, 0);
            }
        } else if (l.rfind("eval(", 0) == 0) {
            std::regex eval_re("eval\\(adr\\(([^)]+)\\)\\s*([+-])\\s*(0x[0-9a-fA-F]+|dist\\.main)\\)");
            std::smatch match;
            if (std::regex_search(l, match, eval_re)) {
                uint32_t base = resolve_label(match[1].str());
                std::string op = match[2].str();
                std::string val_str = match[3].str();
                uint32_t val = 0;
                if (val_str == "dist.main") {
                    val = section_backup - section_start;
                } else {
                    val = std::stoul(val_str, nullptr, 16);
                }
                uint32_t final_val = (op == "+") ? (base + val) : (base - val);
                emit_u16(result, final_val & 0xFFFF);
            } else {
                emit_u16(result, 0);
            }
        } else if (l.length() >= 2 && l.front() == '"' && l.back() == '"') {
            std::string str_val = l.substr(1, l.length() - 2);
            std::vector<uint8_t> encoded;
            if (CharTable::get_instance().encode_string(str_val, encoded)) {
                for (uint8_t b : encoded) {
                    result.push_back(b);
                }
            } else {
                for (char c : str_val) {
                    result.push_back((uint8_t)c);
                }
            }
        } else if (m_gadgets.count(l)) {
            emit_u32(result, m_gadgets[l]);
        } else if (m_labels.count(l)) {
            uint32_t addr = m_labels[l];
            if (!m_model_dir.empty()) {
                std::ifstream rom_f(m_model_dir + "/rom.bin", std::ios::binary);
                if (rom_f.is_open()) {
                    rom_f.seekg(addr, std::ios::beg);
                    char buf[2];
                    if (rom_f.read(buf, 2)) {
                        if ((uint8_t)buf[0] == 0xCE && (uint8_t)buf[1] == 0xF8) {
                            addr += 2;
                            std::cout << "[EMIT] ADDED +2 FOR PUSH LR! Label: " << l << " New Addr: " << std::hex << addr << "\n";
                        }
                    }
                }
            }
            emit_u32(result, addr);
        } else {
            std::cerr << "[Compiler] Unrecognized line during emit: " << l << "\n";
            emit_u32(result, 0);
        }
    }
    
    return result;
}
