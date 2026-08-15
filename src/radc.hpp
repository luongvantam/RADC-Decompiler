#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <map>
#include <set>
#include <cstdint>
#include <map>

struct RadcItem {
    std::string type; // "data", "call", "label"
    std::vector<uint8_t> hexBytes;
    uint32_t value;
    uint32_t ramAddrDest;
    uint32_t ramAddrSrc;
    uint32_t addr;
    std::string name;
    
    // Label ref info
    bool hasLabelRef;
    std::string labelName;
    bool isSrcRef;
    uint32_t originalValue;
    
    bool labelSecond;
    uint32_t pivotSP; // Non-zero if this call performed a stack pivot, and this is the number of bytes popped on the new stack
    uint32_t pivotTargetSP; // The exact absolute label target address calculated via CPU state simulation
    bool hasEvalStr;
    std::string evalStr;
    bool isString = false;
    std::string stringVal;
};

struct GadgetInfo {
    uint32_t start_offset;
    uint32_t end_offset; // start_offset + 4 + pops
    uint32_t pops;
    bool invalid = false;
};

class Radc {
public:
    Radc(const std::vector<uint8_t>& rom_buffer);
    
    std::vector<uint8_t> parse_hex(const std::string& hex_str);
    void parse_user_input(const std::vector<std::string>& lines, uint32_t& out_src, uint32_t& out_dest, std::vector<uint8_t>& out_bytes);
    
    void pass0_prescan(const std::vector<uint8_t>& bytes_list, uint32_t start_addr_dest, uint32_t start_addr_src);
    void pass1_dryrun(const std::vector<uint8_t>& bytes_list, uint32_t start_addr_dest, uint32_t start_addr_src, bool disable_offsets = false);
    std::vector<RadcItem> pass2_decomp(const std::vector<uint8_t>& bytes_list, uint32_t start_addr_dest, uint32_t start_addr_src, bool disable_offsets = false);
    bool pass3_verify(const std::vector<uint8_t>& original_bytes, std::vector<RadcItem>& items, uint32_t start_addr_dest, uint32_t start_addr_src);
    std::string process_program(const std::vector<RadcItem>& items, uint32_t start_addr_dest, uint32_t start_addr_src);

public:
    std::vector<uint8_t> m_rom;
    std::map<uint32_t, uint32_t> m_pivot_targets; // ramAddrDest -> pivotSP, populated by tokenize()
    std::map<uint32_t, uint32_t> m_suppressed_pivot_bases;
    std::set<uint32_t> known_labels;
    std::set<uint32_t> known_backup_labels;
    std::set<uint32_t> all_known_labels;
    std::set<uint32_t> all_known_backup_labels;
    std::vector<GadgetInfo> m_gadgets;
    std::vector<bool> m_is_pad;
};
