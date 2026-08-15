#include <set>
#include "radc.hpp"
#include "Decoder.hpp"
#include "CharTable.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <regex>
#include <algorithm>
#include <map>

Radc::Radc(const std::vector<uint8_t>& rom_buffer) : m_rom(rom_buffer) {}

std::vector<uint8_t> Radc::parse_hex(const std::string& hex_str) {
    std::string clean;
    for (char c : hex_str) {
        if (!std::isspace(c)) {
            clean += c;
        }
    }
    std::vector<uint8_t> result;
    if (clean.empty()) return result;

    for (size_t i = 0; i < clean.length(); i += 2) {
        if (i + 1 < clean.length()) {
            std::string byteString = clean.substr(i, 2);
            uint8_t byte = (uint8_t) strtol(byteString.c_str(), NULL, 16);
            result.push_back(byte);
        }
    }
    return result;
}

void Radc::parse_user_input(const std::vector<std::string>& lines, uint32_t& out_src, uint32_t& out_dest, std::vector<uint8_t>& out_bytes) {
    out_src = 0xe9e0;
    out_dest = 0xd730;
    std::string full_hex_str;

    std::regex src_regex("\\[\\s*src\\s*:\\s*(0x[0-9a-fA-F]+|\\d+)\\s*\\]", std::regex_constants::icase);
    std::regex dest_regex("\\[\\s*dest\\s*:\\s*(0x[0-9a-fA-F]+|\\d+)\\s*\\]", std::regex_constants::icase);
    std::regex bracket_regex("\\[.*?\\]");

    for (std::string line : lines) {
        std::smatch match;
        if (std::regex_search(line, match, src_regex)) {
            out_src = std::stoul(match[1].str(), nullptr, 0);
            continue;
        }
        if (std::regex_search(line, match, dest_regex)) {
            out_dest = std::stoul(match[1].str(), nullptr, 0);
            continue;
        }

        line = std::regex_replace(line, bracket_regex, "");
        if (!line.empty()) {
            full_hex_str += line;
        }
    }
    out_bytes = parse_hex(full_hex_str);
}

void Radc::pass0_prescan(const std::vector<uint8_t>& bytes_list, uint32_t start_addr_dest, uint32_t start_addr_src) {
    known_labels.insert(start_addr_dest);
}

void Radc::pass1_dryrun(const std::vector<uint8_t>& bytes_list, uint32_t start_addr_dest, uint32_t start_addr_src, bool disable_offsets) {
    if (disable_offsets) start_addr_src = 0;
    std::set<uint32_t> suppressed_labels;
    uint32_t max_dest_addr = start_addr_dest + bytes_list.size();
    m_gadgets.clear();
    m_is_pad.assign(bytes_list.size(), false);
    
    Decoder decoder;
    VirtualCPU global_cpu;
    const uint32_t min_valid_call_addr = 0x8000;
    
    // Greedy scan: try every 2-byte aligned position as potential 4-byte gadget address
    size_t i = 0;
    while (i + 3 < bytes_list.size()) {
        uint8_t aa = bytes_list[i];
        uint8_t bb = bytes_list[i+1];
        
        // Skip FFFF (brk)
        if (aa == 0xFF && bb == 0xFF) {
            i += 2;
            continue;
        }
        
        uint8_t cc = bytes_list[i+2];
        uint8_t dd = bytes_list[i+3];
        
        uint32_t raw_addr = ((cc & 0x0F) << 16) | (bb << 8) | aa;
        
        bool is_in_payload = (raw_addr >= start_addr_dest && raw_addr < max_dest_addr);
        
        if (!is_in_payload && raw_addr >= min_valid_call_addr && raw_addr != 0x03030) {
            int align_addr = raw_addr & ~1;
            
            // Only try if address is within ROM
            if (align_addr >= 0 && align_addr < (int)m_rom.size()) {
                VirtualCPU cpu_state = global_cpu;
                cpu_state.sp = start_addr_dest + i + 4;
                cpu_state.stack_pivoted = false;
                cpu_state.sp_valid = true;
                
                std::vector<std::string> new_labels;
                std::vector<uint32_t> dyn_addrs;
                uint32_t pivot_sp = 0;
                
                bool is_gadget = decoder.AnalyzeGadget(cpu_state, m_rom.data(), align_addr, m_rom.size(), bytes_list.data(), bytes_list.size(), start_addr_dest, new_labels, dyn_addrs, nullptr, &pivot_sp);
                
                if (is_gadget) {
                    global_cpu = cpu_state;
                    uint32_t pad = cpu_state.pad_bytes_consumed;
                    
                    GadgetInfo g;
                    g.start_offset = i;
                    g.pops = pad;
                    g.end_offset = i + 4 + pad;
                    m_gadgets.push_back(g);
                    
                    uint32_t max_src_addr = start_addr_src + bytes_list.size();
                    if (!disable_offsets && pivot_sp != 0 && cpu_state.pivot_base_val != 0) {
                        suppressed_labels.insert(cpu_state.pivot_base_val);
                        m_suppressed_pivot_bases[cpu_state.pivot_base_val] = pivot_sp;
                    }
                    if (pivot_sp >= start_addr_dest && pivot_sp < max_dest_addr) {
                        known_labels.insert(pivot_sp);
                    } else if (start_addr_src != 0 && pivot_sp >= start_addr_src && pivot_sp < max_src_addr) {
                        known_backup_labels.insert(pivot_sp - start_addr_src + start_addr_dest);
                    }
                    
                    for (uint32_t addr : dyn_addrs) {
                        if (addr >= start_addr_dest && addr < max_dest_addr) {
                            if (addr % 2 != 0) {
                                printf("ODD DYN_ADDR: %x\n", addr);
                            }
                            known_labels.insert(addr);
                        } else if (start_addr_src != 0 && addr >= start_addr_src && addr < max_src_addr) {
                            known_backup_labels.insert(addr - start_addr_src + start_addr_dest);
                        }
                    }
                    
                    // Advance past gadget address (4) + padding bytes consumed by pops
                    i += 4 + pad;
                    continue;
                }
            }
        }
        
        // Not a gadget — advance by 2 bytes
        i += 2;
    }

    // Pass 1.5: Identify 2-byte sequences in all data that point to valid payload addresses
    for (size_t k = 0; k + 1 < bytes_list.size(); k += 2) {
        uint32_t val = bytes_list[k] | (bytes_list[k+1] << 8);
        uint32_t max_src_addr = start_addr_src + bytes_list.size();
        if (val >= start_addr_dest && val < max_dest_addr) {
            known_labels.insert(val);
        } else if (start_addr_src != 0 && val >= start_addr_src && val < max_src_addr) {
            known_backup_labels.insert(val - start_addr_src + start_addr_dest);
        }
    }
    for (uint32_t l : suppressed_labels) {
        uint32_t max_src_addr = start_addr_src + bytes_list.size();
        if (l >= start_addr_dest && l < max_dest_addr) {
            known_labels.erase(l);
        } else if (start_addr_src != 0 && l >= start_addr_src && l < max_src_addr) {
            known_backup_labels.erase(l - start_addr_src + start_addr_dest);
        }
    }
    all_known_labels = known_labels;
    all_known_backup_labels = known_backup_labels;
}

std::vector<RadcItem> Radc::pass2_decomp(const std::vector<uint8_t>& bytes_list, uint32_t start_addr_dest, uint32_t start_addr_src, bool disable_offsets) {
    if (disable_offsets) start_addr_src = 0;
    std::vector<RadcItem> items;
    
    for (uint32_t l : known_backup_labels) {
        known_labels.insert(l);
    }
    // We don't need known_backup_labels to emit labels anymore
    
    std::map<uint32_t, std::string> label_names;
    for (uint32_t l : known_labels) {
        std::stringstream ss;
        ss << "ADDR_" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << l;
        label_names[l] = ss.str();
    }

    
    auto emit_labels = [&](uint32_t current_addr) {
        std::vector<uint32_t> to_emit_normal;
        for (auto l : known_labels) {
            if (current_addr >= l) {
                to_emit_normal.push_back(l);
            } else {
                break;
            }
        }
        for (uint32_t l : to_emit_normal) {
            std::stringstream ss;
            ss << "ADDR_" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << l;
            items.push_back({"label", {}, 0, l, 0, 0, "", false, ss.str(), false, 0, false});
            known_labels.erase(l);
        }
        

    };
    
    size_t i = 0;
    size_t gadget_idx = 0;

    while (i < bytes_list.size()) {
        uint32_t ram_addr_dest = start_addr_dest + i;
        uint32_t ram_addr_src = start_addr_src + i;
        
        emit_labels(ram_addr_dest);
        
        if (gadget_idx < m_gadgets.size() && m_gadgets[gadget_idx].start_offset == i) {
            bool has_internal_label = false;
            for (int offset = 1; offset < 4; offset++) {
                if (known_labels.count(ram_addr_dest + offset) || known_backup_labels.count(ram_addr_dest + offset)) {
                    has_internal_label = true;
                    break;
                }
            }
            if (has_internal_label) {
                // If a label exists strictly inside this gadget, do not output it as 'call'.
                // Fall through to output raw hex so the label is correctly placed before the raw bytes.
                gadget_idx++;
            } else {
                uint8_t aa = bytes_list[i];
                uint8_t bb = bytes_list[i+1];
                uint8_t cc = bytes_list[i+2];
                uint8_t dd = bytes_list[i+3];

                uint32_t raw_addr = ((cc & 0x0F) << 16) | (bb << 8) | aa;
                
                RadcItem item;
                item.type = "call";
                item.hexBytes = {aa, bb, cc, dd};
                item.addr = raw_addr;
                
                std::stringstream ss;
                ss << "call " << std::hex << std::uppercase << std::setfill('0') << std::setw(5) << raw_addr;
                item.name = ss.str();
                
                item.ramAddrDest = ram_addr_dest;
                item.ramAddrSrc = ram_addr_src;
                item.hasLabelRef = false;
                item.labelSecond = false;
                
                items.push_back(item);
                
                i += 4;
                gadget_idx++;
                continue;
            }
        }

        // Output raw data in 2-byte chunks
        if (i + 1 < bytes_list.size()) {
            // Check if this is a string
            if (known_labels.count(ram_addr_dest) || known_backup_labels.count(ram_addr_dest)) {
                // If it looks like a string (sequence of characters in CharTable until 0x00)
                std::string extracted_str = "";
                size_t j = i;
                bool is_null_terminated = false;
                bool valid_chars = true;
                size_t char_count = 0;
                while (j < bytes_list.size()) {
                    uint32_t current_addr = ram_addr_dest + (j - i);
                    if (j > i && (known_labels.count(current_addr) || known_backup_labels.count(current_addr))) {
                        break;
                    }
                    uint8_t c = bytes_list[j];
                    if (c == 0x00) {
                        is_null_terminated = true;
                        break;
                    }
                    std::string decoded_ch;
                    size_t consumed = 0;
                    if (CharTable::get_instance().decode_character(bytes_list, j, decoded_ch, consumed)) {
                        extracted_str += decoded_ch;
                        char_count++;
                        j += consumed;
                    } else if (c == '\n' || c == '\r' || c == '\t') {
                        extracted_str += (char)c;
                        char_count++;
                        j++;
                    } else {
                        valid_chars = false;
                        break;
                    }
                }
                
                // If we found a string of length >= 2
                if (valid_chars && is_null_terminated && char_count >= 2) {
                    RadcItem str_it;
                    str_it.type = "data";
                    str_it.isString = true;
                    str_it.stringVal = extracted_str;
                    str_it.ramAddrDest = ram_addr_dest;
                    str_it.ramAddrSrc = ram_addr_src;
                    str_it.hasLabelRef = false;
                    str_it.hasEvalStr = false;
                    str_it.labelSecond = false;
                    for (size_t k = i; k < j; k++) {
                        str_it.hexBytes.push_back(bytes_list[k]);
                    }
                    items.push_back(str_it);
                    i = j;
                    continue;
                }
            }

            if (ram_addr_dest % 2 != 0) {
                RadcItem it0 = {"data", {bytes_list[i]}, bytes_list[i], ram_addr_dest, ram_addr_src, 0, "", false, "", false, 0, false, 0, 0};
                items.push_back(it0);
                i += 1;
                continue;
            }

            uint32_t val = bytes_list[i] | (bytes_list[i+1] << 8);
            if (m_suppressed_pivot_bases.count(val)) {
                uint32_t p_sp = m_suppressed_pivot_bases[val];
                bool label_exists = known_labels.count(p_sp) > 0 || known_backup_labels.count(p_sp) > 0;
                if (!label_exists) {
                    // Do not emit eval for non-existent labels
                    // fallthrough to regular data output
                } else {
                std::stringstream eval_ss;
                eval_ss << "eval(adr(ADDR_" << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << p_sp << ") - 0x" << (p_sp - val) << ")";
                RadcItem it0;
                it0.type = "data";
                it0.hexBytes = {bytes_list[i]};
                it0.value = bytes_list[i];
                it0.ramAddrDest = ram_addr_dest;
                it0.ramAddrSrc = ram_addr_src;
                it0.hasEvalStr = true;
                it0.evalStr = eval_ss.str();
                it0.originalValue = val;
                
                RadcItem it1;
                it1.type = "data";
                it1.hexBytes = {bytes_list[i+1]};
                it1.value = bytes_list[i+1];
                it1.ramAddrDest = ram_addr_dest + 1;
                it1.ramAddrSrc = ram_addr_src + 1;
                it1.labelSecond = true;
                
                items.push_back(it0);
                items.push_back(it1);
                i += 2;
                continue;
                }
            }
            if (label_names.find(val) != label_names.end()) {
                RadcItem it0 = {"data", {bytes_list[i]}, bytes_list[i], ram_addr_dest, ram_addr_src, 0, "", true, label_names[val], false, val, false, 0, 0};
                RadcItem it1 = {"data", {bytes_list[i+1]}, bytes_list[i+1], ram_addr_dest + 1, ram_addr_src + 1, 0, "", false, "", true, 0, false, 0, 0};
                items.push_back(it0);
                items.push_back(it1);
                i += 2;
                continue;
            }
            // Check if val is a pointer to the backup region
            if (start_addr_src != 0) {
                uint32_t possible_main_val = val - start_addr_src + start_addr_dest;
                if (known_backup_labels.count(possible_main_val)) {
                    std::stringstream ss;
                    ss << "eval(adr(ADDR_" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << possible_main_val << ") + dist.main)";
                    RadcItem it0;
                    it0.type = "data";
                    it0.hexBytes = {bytes_list[i]};
                    it0.value = bytes_list[i];
                    it0.ramAddrDest = ram_addr_dest;
                    it0.ramAddrSrc = ram_addr_src;
                    it0.hasEvalStr = true;
                    it0.evalStr = ss.str();
                    it0.originalValue = val;
                    it0.labelSecond = false;
                    
                    RadcItem it1;
                    it1.type = "data";
                    it1.hexBytes = {bytes_list[i+1]};
                    it1.value = bytes_list[i+1];
                    it1.ramAddrDest = ram_addr_dest + 1;
                    it1.ramAddrSrc = ram_addr_src + 1;
                    it1.labelSecond = true;
                    items.push_back(it0);
                    items.push_back(it1);
                    i += 2;
                    continue;
                }
            }
            
            // Check if there is a label at the next byte (ram_addr_dest + 1).
            // If so, we MUST NOT consume 2 bytes, otherwise the label will be emitted AFTER the next byte.
            if (known_labels.count(ram_addr_dest + 1) || known_backup_labels.count(ram_addr_dest + 1)) {
                RadcItem it0 = {"data", {bytes_list[i]}, bytes_list[i], ram_addr_dest, ram_addr_src, 0, "", false, "", false, 0, false, 0, 0};
                items.push_back(it0);
                i += 1;
                continue;
            }
            
            // Check if consuming 2 bytes would eat the first byte of a valid pointer
            bool next_is_pointer = false;
            if (i + 2 < bytes_list.size()) {
                uint32_t next_val = bytes_list[i+1] | (bytes_list[i+2] << 8);
                if (label_names.find(next_val) != label_names.end()) {
                    next_is_pointer = true;
                }
            }
            
            if (next_is_pointer) {
                RadcItem it0 = {"data", {bytes_list[i]}, bytes_list[i], ram_addr_dest, ram_addr_src, 0, "", false, "", false, 0, false, 0, 0};
                items.push_back(it0);
                i += 1;
                continue;
            }

            // Generic 2-byte output
            RadcItem it0 = {"data", {bytes_list[i]}, bytes_list[i], ram_addr_dest, ram_addr_src, 0, "", false, "", false, 0, false, 0, 0};
            RadcItem it1 = {"data", {bytes_list[i+1]}, bytes_list[i+1], ram_addr_dest + 1, ram_addr_src + 1, 0, "", false, "", false, 0, false, 0, 0};
            items.push_back(it0);
            items.push_back(it1);
            i += 2;
            continue;
        }

        // Fallback for single byte if payload has odd size (should not happen in proper payloads)
        uint8_t b = bytes_list[i];
        items.push_back({"data", {b}, b, ram_addr_dest, ram_addr_src, 0, "", false, "", false, 0, false, 0, 0});
        i += 1;
    }
    
    emit_labels(start_addr_dest + bytes_list.size());

    return items;
}

bool Radc::pass3_verify(const std::vector<uint8_t>& original_bytes, std::vector<RadcItem>& items, uint32_t start_addr_dest, uint32_t start_addr_src) {
    std::map<uint32_t, int> offset_freq;
    for (const auto& it : items) {
        if (it.type == "call" && it.hexBytes.size() == 4) {
            uint32_t raw_addr = it.addr;
            uint32_t full_word = it.hexBytes[0] | (it.hexBytes[1] << 8) | (it.hexBytes[2] << 16) | (it.hexBytes[3] << 24);
            uint32_t offset = full_word - raw_addr;
            offset_freq[offset]++;
        }
    }
    uint32_t best_offset = 0;
    int max_freq = -1;
    for (const auto& pair : offset_freq) {
        if (pair.second > max_freq) {
            max_freq = pair.second;
            best_offset = pair.first;
        }
    }

    bool has_mismatch = false;
    std::vector<RadcItem> new_items;
    
    for (size_t i = 0; i < items.size(); ++i) {
        auto& it = items[i];
        
        bool is_valid = true;
        if (it.type == "call" && it.hexBytes.size() == 4) {
            uint32_t expected_val = it.addr + best_offset;
            uint32_t actual_val = it.hexBytes[0] | (it.hexBytes[1] << 8) | (it.hexBytes[2] << 16) | (it.hexBytes[3] << 24);
            if (expected_val != actual_val) {
                is_valid = false;
            }
        }
        
        if (!is_valid) {
            has_mismatch = true;
            // Re-evaluate broken call gadgets for label references
            for (size_t j = 0; j < it.hexBytes.size(); ) {
                if (j + 1 < it.hexBytes.size()) {
                    uint32_t val = it.hexBytes[j] | (it.hexBytes[j+1] << 8);
                    
                    std::string labelName = "";
                    bool isBackup = false;
                    uint32_t possible_main_val = 0;
                    
                    if (all_known_labels.count(val)) {
                        std::stringstream ss;
                        ss << "ADDR_" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << val;
                        labelName = ss.str();
                    } else if (start_addr_src != 0) {
                        possible_main_val = val - start_addr_src + start_addr_dest;
                        if (all_known_backup_labels.count(possible_main_val)) {
                            isBackup = true;
                        }
                    }
                    
                    if (!labelName.empty()) {
                        RadcItem it0 = {"data", {it.hexBytes[j]}, it.hexBytes[j], (uint32_t)(it.ramAddrDest + j), (uint32_t)(it.ramAddrSrc + j), 0, "", true, labelName, false, val, false, 0, 0};
                        RadcItem it1 = {"data", {it.hexBytes[j+1]}, it.hexBytes[j+1], (uint32_t)(it.ramAddrDest + j + 1), (uint32_t)(it.ramAddrSrc + j + 1), 0, "", false, "", true, 0, false, 0, 0};
                        new_items.push_back(it0);
                        new_items.push_back(it1);
                        j += 2;
                        continue;
                    } else if (isBackup) {
                        std::stringstream eval_ss;
                        eval_ss << "eval(adr(ADDR_" << std::uppercase << std::setfill('0') << std::setw(4) << std::hex << possible_main_val << ") + dist.main)";
                        RadcItem it0 = {"data", {it.hexBytes[j]}, it.hexBytes[j], (uint32_t)(it.ramAddrDest + j), (uint32_t)(it.ramAddrSrc + j), 0, "", false, "", false, val, false, 0, 0};
                        it0.hasEvalStr = true;
                        it0.evalStr = eval_ss.str();
                        
                        RadcItem it1 = {"data", {it.hexBytes[j+1]}, it.hexBytes[j+1], (uint32_t)(it.ramAddrDest + j + 1), (uint32_t)(it.ramAddrSrc + j + 1), 0, "", false, "", true, 0, false, 0, 0};
                        
                        new_items.push_back(it0);
                        new_items.push_back(it1);
                        j += 2;
                        continue;
                    }
                }
                
                RadcItem d_it;
                d_it.type = "data";
                d_it.hexBytes = { it.hexBytes[j] };
                d_it.addr = it.hexBytes[j];
                d_it.ramAddrDest = it.ramAddrDest + j;
                d_it.ramAddrSrc = it.ramAddrSrc + j;
                d_it.hasLabelRef = false;
                d_it.labelSecond = false;
                new_items.push_back(d_it);
                j++;
            }
        } else {
            new_items.push_back(it);
        }
    }
    
    items = new_items;
    
    return !has_mismatch;
}

std::string Radc::process_program(const std::vector<RadcItem>& items, uint32_t start_addr_dest, uint32_t start_addr_src) {
    std::stringstream ss;
    ss << "@section.main at 0x" << std::nouppercase << std::setfill('0') << std::setw(4) << std::hex << start_addr_dest 
       << " backup 0x" << std::setfill('0') << std::setw(4) << start_addr_src << "\n";

    std::map<uint32_t, int> offset_freq;
    for (const auto& it : items) {
        if (it.type == "call" && it.hexBytes.size() == 4) {
            uint32_t raw_addr = it.addr;
            uint32_t full_word = it.hexBytes[0] | (it.hexBytes[1] << 8) | (it.hexBytes[2] << 16) | (it.hexBytes[3] << 24);
            uint32_t offset = full_word - raw_addr;
            offset_freq[offset]++;
        }
    }
    uint32_t best_offset = 0;
    int max_freq = -1;
    for (const auto& pair : offset_freq) {
        if (pair.second > max_freq) {
            max_freq = pair.second;
            best_offset = pair.first;
        }
    }
    // ss << "@offset 0x" << std::nouppercase << std::setfill('0') << std::setw(8) << std::hex << best_offset << "\n";


    size_t i = 0;
    while (i < items.size()) {
        const auto& it = items[i];
        if (it.type == "label") {
            ss << "\nlbl " << it.labelName << "\n";
            i++;
            continue;
        }
        if (it.type == "call") {
            ss << "   " << it.name << "\n";
            i++;
            continue;
        }

        if (it.isString) {
            ss << "   \"" << it.stringVal << "\"\n";
            i++;
            continue;
        }

        if (it.hasEvalStr) {
            if (i + 1 < items.size()) {
                ss << "   " << it.evalStr << "\n";
            }
            i += 2;
            continue;
        }
        if (it.hasLabelRef) {
            if (i + 1 < items.size()) {
                ss << it.labelName << "\n";
            }
            i += 2;
            continue;
        }
        if (it.labelSecond) {
            i++;
            continue;
        }

        if (i + 1 < items.size() && (items[i+1].type == "data" || items[i+1].type == "padding") && !items[i+1].labelSecond && !items[i+1].hasLabelRef && (it.ramAddrDest % 2 == 0)) {
            ss << "   hex " << std::setfill('0') << std::setw(2) << std::hex << (int)it.hexBytes[0] 
               << " " << std::setw(2) << (int)items[i+1].hexBytes[0] << "\n";
            i += 2;
            continue;
        }

        ss << "   hex " << std::setfill('0') << std::setw(2) << std::hex << (int)it.hexBytes[0] << "\n";
        i++;
    }
    return ss.str();
}
