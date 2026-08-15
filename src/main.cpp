#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "Decoder.hpp"
#include "radc.hpp"
#include "replace.hpp"
#include "compiler.hpp"
#include "CharTable.hpp"
#include <regex>

int main(int argc, char** argv) {
    std::vector<std::string> args;
    bool do_recompile = true;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--no-recompile") {
            do_recompile = false;
        } else {
            args.push_back(arg);
        }
    }

    if (args.size() < 3) {
        std::cerr << "Usage: " << argv[0] << " [--no-recompile] <model> <finput> <foutput>" << std::endl;
        return 1;
    }

    std::string model = args[0];
    std::string input_path = args[1];
    std::string output_path = args[2];

    std::string config_path = model + "/config.json";
    std::string rom_name = "rom.bin";
    std::string labels_name = "labels.txt";
    std::string gadgets_name = "gadgets.txt";
    std::string char_to_hex_name = "char_to_hex.json";
    std::ifstream config_f(config_path);
    if (config_f.is_open()) {
        std::string content((std::istreambuf_iterator<char>(config_f)), std::istreambuf_iterator<char>());
        std::regex label_re("\"labels\"\\s*:\\s*\"([^\"]+)\"");
        std::regex gadget_re("\"gadgets\"\\s*:\\s*\"([^\"]+)\"");
        std::regex rom_re("\"rom\"\\s*:\\s*\"([^\"]+)\"");
        std::regex char_re("\"char_to_hex\"\\s*:\\s*\"([^\"]+)\"");
        std::smatch m;
        if (std::regex_search(content, m, label_re)) labels_name = m[1];
        if (std::regex_search(content, m, gadget_re)) gadgets_name = m[1];
        if (std::regex_search(content, m, rom_re)) rom_name = m[1];
        if (std::regex_search(content, m, char_re)) char_to_hex_name = m[1];
    }

    // Load char table if model-specific or root file exists
    if (!CharTable::get_instance().load_from_file(model + "/" + char_to_hex_name)) {
        if (!CharTable::get_instance().load_from_file(char_to_hex_name)) {
            CharTable::get_instance().load_from_file("src/" + char_to_hex_name);
        }
    }

    std::ifstream input_file(input_path);
    if (!input_file.is_open()) {
        std::cerr << "Failed to open input file: " << input_path << std::endl;
        return 1;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(input_file, line)) {
        lines.push_back(line);
    }

    std::string rom_path = model + "/" + rom_name;
    std::ifstream rom_file(rom_path, std::ios::binary | std::ios::ate);
    if (!rom_file.is_open()) {
        rom_file.open(rom_name, std::ios::binary | std::ios::ate);
    }
    
    if (!rom_file.is_open()) {
        std::cerr << "Failed to open rom file: " << rom_path << std::endl;
        return 1;
    }
    
    std::streamsize size = rom_file.tellg();
    rom_file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> rom_buffer(size);
    if (!rom_file.read((char*)rom_buffer.data(), size)) {
        std::cerr << "Failed to read rom.bin" << std::endl;
        return 1;
    }
    
    Radc radc(rom_buffer);
    
    uint32_t start_src, start_dest;
    std::vector<uint8_t> hex_bytes;
    radc.parse_user_input(lines, start_src, start_dest, hex_bytes);
    
    if (hex_bytes.empty()) {
        std::cerr << "Error: Input file hex payload stream is blank or invalid." << std::endl;
        return 1;
    }
    
    radc.pass0_prescan(hex_bytes, start_dest, start_src);
    radc.pass1_dryrun(hex_bytes, start_dest, start_src, !do_recompile);
    auto items = radc.pass2_decomp(hex_bytes, start_dest, start_src, !do_recompile);
    
    if (do_recompile) {
        if (!radc.pass3_verify(hex_bytes, items, start_dest, start_src)) {
            std::cerr << "Warning: Verification failed during decompilation pass!\n";
        }
    }
    
    std::string output = radc.process_program(items, start_dest, start_src);
    
    if (do_recompile) {
        RopCompiler compiler;
        compiler.load_gadgets_file(model + "/" + gadgets_name);
        compiler.load_labels_file(model + "/" + labels_name);
        compiler.set_model_dir(model);
        std::vector<uint8_t> compiled = compiler.compile(output);
        
        bool verify_ok = true;
        if (compiled.size() != hex_bytes.size()) {
            std::cerr << "End-to-End Compile Verification FAILED: Size mismatch (" << compiled.size() << " vs " << hex_bytes.size() << ")\n";
            verify_ok = false;
        } else {
            for (size_t i = 0; i < compiled.size(); i++) {
                if (compiled[i] != hex_bytes[i]) {
                    std::cerr << "End-to-End Compile Verification FAILED: Mismatch at offset " << i 
                              << " (expected " << std::hex << (int)hex_bytes[i] << ", got " << (int)compiled[i] << ")\n";
                    verify_ok = false;
                    break;
                }
            }
        }
        
        if (verify_ok) {
            std::cerr << "End-to-End Compile Verification: SUCCESS!\n";
        }
    }
    
    output = apply_replacements(output, model);
    
    if (argc >= 4) {
        std::ofstream out_file(output_path);
        if (out_file.is_open()) {
            out_file << output << "\n";
        } else {
            std::cerr << "Failed to open output file: " << output_path << std::endl;
        }
    } else {
        std::cout << output << "\n";
    }

    return 0;
}
