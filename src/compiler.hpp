#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdint>

class RopCompiler {
public:
    void load_gadgets(const std::map<uint32_t, std::string>& gadgets_by_addr);
    void load_labels(const std::map<uint32_t, std::string>& labels_by_addr);
    
    void load_gadgets_file(const std::string& path);
    void load_labels_file(const std::string& path);
    
    void set_model_dir(const std::string& dir) { m_model_dir = dir; }
    
    std::vector<uint8_t> compile(const std::string& source);

private:
    std::string m_model_dir;
    std::map<std::string, uint32_t> m_gadgets;
    std::map<std::string, uint32_t> m_labels; // User-defined or static labels
    std::map<std::string, uint32_t> m_local_labels; // Labels defined in the source
};
