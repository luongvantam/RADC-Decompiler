#include "Decoder.hpp"
#include <cstring>

OpcodeSource opcode_sources[] = {
#include "OpcodeArray.inc"
};

Decoder::Decoder() {
    opcode_dispatch = new OpcodeSource*[0x10000];
    std::memset(opcode_dispatch, 0, sizeof(OpcodeSource*) * 0x10000);

    uint16_t* permutation_buffer = new uint16_t[0x10000];
    for (size_t ix = 0; ix != sizeof(opcode_sources) / sizeof(opcode_sources[0]); ++ix) {
        OpcodeSource& handler_stub = opcode_sources[ix];

        uint16_t varying_bits = 0;
        for (size_t ox = 0; ox != 2; ++ox) {
            varying_bits |= handler_stub.operands[ox].mask << handler_stub.operands[ox].shift;
        }

        size_t permutation_count = 1;
        permutation_buffer[0] = handler_stub.opcode;
        for (uint16_t checkbit = 0x8000; checkbit; checkbit >>= 1) {
            if (varying_bits & checkbit) {
                for (size_t px = 0; px != permutation_count; ++px) {
                    permutation_buffer[px + permutation_count] = permutation_buffer[px] | checkbit;
                }
                permutation_count <<= 1;
            }
        }

        for (size_t px = 0; px != permutation_count; ++px) {
            if (opcode_dispatch[permutation_buffer[px]]) continue;
            opcode_dispatch[permutation_buffer[px]] = &handler_stub;
        }
    }
    delete[] permutation_buffer;
}

Decoder::~Decoder() {
    delete[] opcode_dispatch;
}

bool Decoder::DecodeInstruction(const uint8_t* memory, int memory_size, int& pc, int& out_instruction_length, int& out_pops, VirtualCPU& cpu, const uint8_t* payload_bytes, int payload_size, uint32_t start_addr_dest, std::vector<std::string>& out_labels, std::vector<uint32_t>& out_dynamic_addrs, std::vector<bool>* out_is_padding) {
    if (pc + 1 >= memory_size) {
        out_instruction_length = 0;
        out_pops = 0;
        return true;
    }

    uint16_t impl_opcode = memory[pc] | (memory[pc + 1] << 8);
    OpcodeSource* handler = opcode_dispatch[impl_opcode];

    if (!handler) {
        out_instruction_length = 2;
        out_pops = 0;
        pc += out_instruction_length;
        return true;
    }

    int current_pc = pc + 2;
    int impl_long_imm = 0;

    if (handler->hint & H_TI) {
        if (current_pc + 1 < memory_size) {
            impl_long_imm = memory[current_pc] | (memory[current_pc + 1] << 8);
            current_pc += 2;
        }
    }

    out_instruction_length = current_pc - pc; debug_type = handler->type;
    out_pops = 0;
    bool is_return = false;

    // Decode operands from opcode bits
    Operand impl_operands[2];
    for (int i = 0; i < 2; i++) {
        impl_operands[i].register_size = handler->operands[i].register_size;
        uint16_t real_mask = handler->operands[i].mask << handler->operands[i].shift;
        impl_operands[i].value = (impl_opcode & real_mask) >> handler->operands[i].shift;
        impl_operands[i].register_index = impl_operands[i].value;
        // If this operand is a register, read its current value from CPU state
        if (impl_operands[i].register_size > 0) {
            impl_operands[i].value = 0;
            for (size_t bx = 0; bx < impl_operands[i].register_size; ++bx) {
                if (impl_operands[i].register_index + bx < 16) {
                    impl_operands[i].value |= ((uint64_t)cpu.reg_r[impl_operands[i].register_index + bx]) << (bx * 8);
                }
            }
        }
    }

    // Validity tracking
    bool dest_valid = true;
    bool src_valid = true;

    if (impl_operands[0].register_size > 0) {
        for (size_t bx = 0; bx < impl_operands[0].register_size; ++bx) {
            if (impl_operands[0].register_index + bx < 16) {
                if (!cpu.reg_valid[impl_operands[0].register_index + bx]) dest_valid = false;
            }
        }
    }

    if (impl_operands[1].register_size > 0) {
        for (size_t bx = 0; bx < impl_operands[1].register_size; ++bx) {
            if (impl_operands[1].register_index + bx < 16) {
                if (!cpu.reg_valid[impl_operands[1].register_index + bx]) src_valid = false;
            }
        }
    }

    // -- ALU operations --
    if (handler->type == OP_TYPE_MOV || handler->type == OP_TYPE_MOV16) {
        if (impl_operands[0].register_size > 0) {
            if (src_valid) {
                impl_operands[0].value = impl_operands[1].value;
                dest_valid = true;
            } else {
                dest_valid = false;
            }
        }
    } else if (handler->type == OP_TYPE_ADD || handler->type == OP_TYPE_ADD16 || handler->type == OP_TYPE_ADDC) {
        if (impl_operands[0].register_size > 0) {
            if (dest_valid && src_valid) {
                impl_operands[0].value += impl_operands[1].value;
            } else {
                dest_valid = false;
            }
        }
    } else if (handler->type == OP_TYPE_MUL) {
        if (impl_operands[0].register_size > 0) {
            if (dest_valid && src_valid) {
                impl_operands[0].value *= impl_operands[1].value;
            } else {
                dest_valid = false;
            }
        }
    } else if (handler->type == OP_TYPE_SUB || handler->type == OP_TYPE_SUBC) {
        if (impl_operands[0].register_size > 0) {
            if (dest_valid && src_valid) {
                impl_operands[0].value -= impl_operands[1].value;
            } else {
                dest_valid = false;
            }
        }
    } else if (handler->type == OP_TYPE_AND) {
        if (impl_operands[0].register_size > 0) {
            if (dest_valid && src_valid) {
                impl_operands[0].value &= impl_operands[1].value;
            } else {
                dest_valid = false;
            }
        }
    } else if (handler->type == OP_TYPE_OR) {
        if (impl_operands[0].register_size > 0) {
            if (dest_valid && src_valid) {
                impl_operands[0].value |= impl_operands[1].value;
            } else {
                dest_valid = false;
            }
        }
    } else if (handler->type == OP_TYPE_XOR) {
        if (impl_operands[0].register_size > 0) {
            if (dest_valid && src_valid) {
                impl_operands[0].value ^= impl_operands[1].value;
            } else {
                dest_valid = false;
            }
        }
    } else if (handler->type == OP_TYPE_ADDSP) {
        // SP += imm
        if (cpu.sp_valid) {
            cpu.sp += (int32_t)((int16_t)impl_operands[1].value); // Assuming operand 1 holds the immediate offset
        }
    }

    // -- Memory operations --
    if (handler->type == OP_TYPE_LS_EA || handler->type == OP_TYPE_LS_R || handler->type == OP_TYPE_CR_EA || handler->type == OP_TYPE_DEC_EA || handler->type == OP_TYPE_INC_EA) {
        // Operand 1 usually points to memory in Load (LD ER0, [ER1]), or Operand 0 points to memory in Store (ST [ER1], ER0)
        // Wait, handler structure might have register_size for memory ops as well.
        // If operand register is valid, we consider it a dynamic label.
        for (int i = 0; i < 2; i++) {
            // Check if this operand represents memory address, heuristic: it's used as an address.
            // For now, any register that is read during a load/store could be a memory address.
            if (impl_operands[i].register_size > 0 && cpu.reg_valid[impl_operands[i].register_index]) {
                // If it's a store/load, the register value might be the target.
                // We don't have perfect AST, but we can just log all valid register values as dynamic addresses.
                // Actually, only log if it looks like a payload address to avoid noise?
                // The caller (Radc) will filter out addresses that don't belong to the payload.
                if (impl_operands[i].value % 2 == 0) {
                    out_dynamic_addrs.push_back(impl_operands[i].value);
                }
            }
        }
    }

    // -- Writeback ALU result to registers (before POP which overwrites separately) --
    bool did_alu_writeback = false;
    if (handler->hint & H_WB && impl_operands[0].register_size > 0) {
        if (handler->type != OP_TYPE_POP && handler->type != OP_TYPE_POPL) {
            did_alu_writeback = true;
            for (size_t bx = 0; bx < impl_operands[0].register_size; ++bx) {
                if (impl_operands[0].register_index + bx < 16) {
                    cpu.reg_r[impl_operands[0].register_index + bx] = (uint8_t)(impl_operands[0].value >> (bx * 8));
                    cpu.reg_valid[impl_operands[0].register_index + bx] = dest_valid;
                }
            }
        }
    }

    // -- Stack/Control operations --
    if (handler->type == OP_TYPE_POP || handler->type == OP_TYPE_POPL) {
        auto pop_val = [&](int bytes, bool is_pad) -> uint64_t {
            uint64_t val = 0;
            bool above_entry = (cpu.sp >= cpu.entry_sp);
            if (cpu.sp_valid) {
                int offset = cpu.sp - start_addr_dest;
                if (offset >= 0 && offset + bytes <= payload_size) {
                    for (int bx = 0; bx < bytes; ++bx) {
                        val |= ((uint64_t)payload_bytes[offset + bx]) << (8 * bx);
                        if (out_is_padding && is_pad && above_entry) {
                            (*out_is_padding)[offset + bx] = true;
                        }
                    }
                }
            }
            // Count pad bytes consumed from payload (not function-saved regs)
            if (is_pad && above_entry && !cpu.stack_pivoted) {
                cpu.pad_bytes_consumed += bytes;
            }
            out_pops += bytes;
            cpu.sp += bytes;
            return val;
        };

        if (handler->type == OP_TYPE_POP) {
            int bytes = (int)impl_operands[0].register_size;
            if (bytes == 1) bytes = 2;
            uint64_t popped = pop_val(bytes, true);
            // Write popped value byte-by-byte into reg_r
            int reg_idx = impl_operands[0].register_index;
            int write_bytes = (int)impl_operands[0].register_size;
            for (int bx = 0; bx < write_bytes && (reg_idx + bx) < 16; ++bx) {
                cpu.reg_r[reg_idx + bx] = (uint8_t)((popped >> (8 * bx)) & 0xFF);
                cpu.reg_valid[reg_idx + bx] = !cpu.stack_pivoted;
            }
        } else if (handler->type == OP_TYPE_POPL) {
            // POPL pops multiple registers based on bitmask in operand
            uint16_t mask = impl_operands[0].value;
            if (mask & 0x01) pop_val(2, true); // reg_ea
            if (mask & 0x08) pop_val(4, false); // reg_lr + reg_lcsr
            if (mask & 0x04) pop_val(2, true); // reg_psw

            if (mask & 0x02) {
                // POP PC — return
                pop_val(4, false);
                if (!cpu.call_stack.empty()) {
                    pc = cpu.call_stack.back();
                    cpu.call_stack.pop_back();
                    if (cpu.call_stack.empty()) {
                        // Returned from outermost BL — gadget done
                        pc += out_instruction_length; // advance past the return site
                        return true;
                    }
                    return false; // still inside nested BL
                }
                is_return = true;
                cpu.popped_pc = true;
            } else {
                is_return = true;
            }
        }
    } else if (handler->type == OP_TYPE_PUSH) {
        int bytes = (int)impl_operands[0].register_size;
        if (bytes == 0) bytes = (int)impl_operands[1].register_size;
        if (bytes == 1) bytes = 2;
        cpu.sp -= bytes;
    } else if (handler->type == OP_TYPE_PUSHL) {
        uint16_t mask = impl_operands[0].value;
        if (mask & 0x02) cpu.sp -= 4; // PC/LR
        if (mask & 0x04) cpu.sp -= 2; // PSW
        if (mask & 0x08) cpu.sp -= 4; // LR+LCSR
        if (mask & 0x01) cpu.sp -= 2; // EA
    } else if (handler->type == OP_TYPE_CTRL) {
        // hint >> 8 == 11 means MOV SP, ERn
        if ((handler->hint >> 8) == 11) {
            bool src_ok = true;
            for (size_t bx = 0; bx < impl_operands[1].register_size; ++bx) {
                if (impl_operands[1].register_index + bx < 16) {
                    if (!cpu.reg_valid[impl_operands[1].register_index + bx]) src_ok = false;
                }
            }
            if (cpu.call_stack.empty()) {
                cpu.sp = src_ok ? (uint32_t)impl_operands[1].value : 0;
                cpu.sp_valid = src_ok;
                cpu.stack_pivoted = true;
                cpu.pivot_base_val = cpu.sp;
            }
        } else if ((handler->hint >> 8) == 10) {
            // MOV ERn, SP? (Not implemented here but could be)
        }
    } else if (handler->type == OP_TYPE_RT) {
        if (!cpu.call_stack.empty()) {
            pc = cpu.call_stack.back();
            cpu.call_stack.pop_back();
            return false;
        }
        cpu.sp += 4;
        out_pops += 4;
        is_return = true;
        cpu.popped_pc = true;
    } else if (handler->type == OP_TYPE_B || handler->type == OP_TYPE_BL) {
        if (handler->type == OP_TYPE_BL) {
            cpu.call_stack.push_back(pc + out_instruction_length);
        }

        if (handler->hint & H_TI) {
            // Long branch: jump directly to target
            pc = (impl_operands[1].value << 16) | impl_long_imm;
            return false;
        } else {
            // Short branch via register
            if (handler->operands[1].register_size > 0) {
                int reg_idx = impl_operands[1].register_index;
                if (reg_idx < 16 && cpu.reg_valid[reg_idx]) {
                    pc = cpu.reg_r[reg_idx];
                    return false;
                }
            }
            // Unknown branch target — stop tracing
            is_return = true;
        }
    } else if (handler->type == OP_TYPE_BC) {
        // Conditional branch: for ROP analysis, ignore and fall through sequentially
    }

    // Normal instruction flow, advance PC
    pc += out_instruction_length;

    return is_return;
}

bool Decoder::AnalyzeGadget(VirtualCPU& cpu, const uint8_t* memory, int pc, int memory_size, const uint8_t* payload_bytes, int payload_size, uint32_t start_addr_dest, std::vector<std::string>& out_labels, std::vector<uint32_t>& out_dynamic_addrs, std::vector<bool>* out_is_padding, uint32_t* out_pivot_sp) {
    if (pc < 0 || pc >= memory_size) return 0;

    int total_pops = 0;


    cpu.call_stack.clear();
    cpu.entry_sp = cpu.sp; // Remember where SP started for pad detection
    cpu.pad_bytes_consumed = 0;
    cpu.popped_pc = false;
    cpu.pivot_base_val = 0;

    if (out_pivot_sp) *out_pivot_sp = 0;

    int max_instructions = 256;
    while (max_instructions-- > 0) {
        int instr_len = 0;
        int pops = 0;

        bool should_stop = DecodeInstruction(memory, memory_size, pc, instr_len, pops, cpu, payload_bytes, payload_size, start_addr_dest, out_labels, out_dynamic_addrs, out_is_padding);

        total_pops += pops;

        if (should_stop) {
            if (cpu.stack_pivoted && cpu.sp_valid) {
                if (out_pivot_sp) *out_pivot_sp = cpu.popped_pc ? (cpu.sp - 4) : cpu.sp;
            }
            return true;
        }
    }

    return false; // Reached max instructions without return
}
