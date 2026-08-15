#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>

enum OpcodeType {
    OP_TYPE_OTHER = 0,
    OP_TYPE_ADD,
    OP_TYPE_ADD16,
    OP_TYPE_ADDC,
    OP_TYPE_ADDSP,
    OP_TYPE_AND,
    OP_TYPE_B,
    OP_TYPE_BC,
    OP_TYPE_BITMOD,
    OP_TYPE_BL,
    OP_TYPE_BRK,
    OP_TYPE_CMP16,
    OP_TYPE_CPLC,
    OP_TYPE_CR_EA,
    OP_TYPE_CR_R,
    OP_TYPE_CTRL,
    OP_TYPE_DAA,
    OP_TYPE_DAS,
    OP_TYPE_DEC_EA,
    OP_TYPE_DIV,
    OP_TYPE_DSR,
    OP_TYPE_EXTBW,
    OP_TYPE_INC_EA,
    OP_TYPE_LEA,
    OP_TYPE_LS_BP,
    OP_TYPE_LS_EA,
    OP_TYPE_LS_FP,
    OP_TYPE_LS_I,
    OP_TYPE_LS_I_R,
    OP_TYPE_LS_R,
    OP_TYPE_MOV,
    OP_TYPE_MOV16,
    OP_TYPE_MUL,
    OP_TYPE_NEG,
    OP_TYPE_NOP,
    OP_TYPE_OR,
    OP_TYPE_POP,
    OP_TYPE_POPL,
    OP_TYPE_PSW_AND,
    OP_TYPE_PSW_OR,
    OP_TYPE_PUSH,
    OP_TYPE_PUSHL,
    OP_TYPE_RT,
    OP_TYPE_RTI,
    OP_TYPE_SLL,
    OP_TYPE_SLLC,
    OP_TYPE_SRA,
    OP_TYPE_SRL,
    OP_TYPE_SRLC,
    OP_TYPE_SUB,
    OP_TYPE_SUBC,
    OP_TYPE_SWI,
    OP_TYPE_XOR,
};

#define H_IE 0x0001
#define H_ST 0x0002
#define H_DW 0x0004
#define H_DS 0x0008
#define H_IA 0x0010
#define H_TI 0x0020
#define H_WB 0x0040

struct OpcodeOperand {
    size_t register_size;
    uint16_t mask, shift;
};

struct OpcodeSource {
    OpcodeType type;
    size_t hint;
    uint16_t opcode;
    OpcodeOperand operands[2];
};

struct Operand {
    size_t register_size;
    uint16_t register_index;
    uint16_t value;
};

struct VirtualCPU {
    uint8_t reg_r[16];
    bool reg_valid[16]; // True if the register has been initialized from payload
    
    uint32_t sp;
    uint32_t entry_sp; // SP at gadget entry, for distinguishing pad from function-saved data
    bool sp_valid;
    bool stack_pivoted; // True if we are reading from the new stack
    bool popped_pc; // True if the gadget ended by popping PC
    uint32_t pivot_base_val; // Value of SP immediately after pivot, before pad pops
    uint32_t pad_bytes_consumed; // Bytes consumed by register pops (not PC/LR) above entry_sp
    
    std::vector<int> call_stack; // For BL and RT tracking
    
    VirtualCPU() {
        for (int i = 0; i < 16; i++) {
            reg_r[i] = 0;
            reg_valid[i] = false;
        }
        sp = 0;
        entry_sp = 0;
        sp_valid = false;
        stack_pivoted = false;
        popped_pc = false;
        pad_bytes_consumed = 0;
    }
};

class Decoder {
public:
    Decoder();
    ~Decoder();

    // Now takes payload_bytes and payload_size to bind to VirtualCPU during execution
    // out_pivot_sp: if >0, a stack pivot occurred and this is the new SP value (= address of next gadget start)
    bool AnalyzeGadget(VirtualCPU& cpu, const uint8_t* memory, int pc, int memory_size, const uint8_t* payload_bytes, int payload_size, uint32_t start_addr_dest, std::vector<std::string>& out_labels, std::vector<uint32_t>& out_dynamic_addrs, std::vector<bool>* out_is_padding = nullptr, uint32_t* out_pivot_sp = nullptr);

public:
    int debug_type = -1; int debug_hint = -1;
    bool DecodeInstruction(const uint8_t* memory, int memory_size, int& pc, int& out_instruction_length, int& out_pops, VirtualCPU& cpu, const uint8_t* payload_bytes, int payload_size, uint32_t start_addr_dest, std::vector<std::string>& out_labels, std::vector<uint32_t>& out_dynamic_addrs, std::vector<bool>* out_is_padding);
    const OpcodeSource* GetHandler(const uint8_t* memory, int size);

    OpcodeSource** opcode_dispatch;
};
