#ifndef CHAR_TABLE_HPP
#define CHAR_TABLE_HPP

#include <string>
#include <vector>
#include <map>
#include <cstdint>

class CharTable {
public:
    static CharTable& get_instance();

    CharTable();
    bool load_from_file(const std::string& filepath);
    bool load_from_json_string(const std::string& json_content);

    // Decode a character at position pos in the byte array.
    // Tries multi-byte matches first, then single-byte.
    bool decode_character(const std::vector<uint8_t>& bytes, size_t pos, std::string& out_char, size_t& out_bytes_consumed) const;

    // Encode a full UTF-8 string into byte vector according to the character table.
    bool encode_string(const std::string& utf8_str, std::vector<uint8_t>& out_bytes) const;

    // Calculate total encoded byte length for a UTF-8 string.
    size_t encoded_length(const std::string& utf8_str) const;

    // Direct lookups
    bool has_char(const std::string& utf8_char) const;
    std::vector<uint8_t> get_bytes(const std::string& utf8_char) const;

private:
    std::map<std::string, std::vector<uint8_t>> m_char_to_bytes;
    std::map<std::vector<uint8_t>, std::string> m_bytes_to_char;
    bool m_initialized = false;

    void init_default();
    void add_mapping(const std::string& utf8_char, const std::string& hex_val);
};

#endif // CHAR_TABLE_HPP
