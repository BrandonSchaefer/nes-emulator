//-*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
/* The MIT License (MIT)
 *
 * Copyright (c) 2017 Brandon Schaefer
 *                    brandontschaefer@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include "cpu.h"
#include "cpu_instructions.h"

#include <iostream>
#include <stdexcept>
#include <cstring>

namespace
{
std::string mode_name(emulator::OpMode mode)
{
    switch (mode)
    {
        case emulator::zero_page_x:
            return "zpx";
        case emulator::zero_page_y:
            return "zpy";
        case emulator::absolute_x:
            return "abx";
        case emulator::absolute_y:
            return "aby";
        case emulator::indexed_x:
            return "izx";
        case emulator::indexed_y:
            return "izy";
        case emulator::implicit:
            return "imp";
        case emulator::accumulator:
            return "acc";
        case emulator::immediate:
            return "imm";
        case emulator::zero_page:
            return "zp ";
        case emulator::absolute:
            return "ab ";
        case emulator::relative:
            return "rel";
        case emulator::indirect:
            return "ind";
    }

    throw std::runtime_error("Unkown OpMode");
}

// Is page crossed means we need to incremement the cycle amount
bool is_page_crossed(uint16_t a, uint16_t b)
{
    // Check if we are on different pages, each are 256 bytes
    return (a & 0xFF00) != (b & 0xFF00);
}

}

emulator::CPU::CPU(emulator::PPU const* ppu) :
    ppu(ppu)
{
    reset();

    // TESTING
    /*
    program_counter_ = 0x0600;
    auto i = program_counter_;
    memory[i] = 0x20;
    memory[i + 1] = 0x09;
    memory[i + 2] = 0x06;
    memory[i + 3] = 0x20;
    memory[i + 4] = 0x0c;
    memory[i + 5] = 0x06;
    memory[i + 6] = 0x20;
    memory[i + 7] = 0x12;
    memory[i + 8] = 0x06;
    memory[i + 9] = 0xa2;
    memory[i + 10] = 0x00;
    memory[i + 11] = 0x60;
    memory[i + 12] = 0xe8;
    memory[i + 13] = 0xe0;
    memory[i + 14] = 0x05;
    memory[i + 15] = 0xd0;
    memory[i + 16] = 0xfb;
    memory[i + 17] = 0x60;
    memory[i + 18] = 0x00;
    */
    /*
    memory[1] = 0x50;
    memory[2] = 0xFF;

    for (size_t i = 0; i < 0x100; i++)
    {

        memory[0] = i;
        auto info = instruction[memory[0]];
        info.func(this);
    */
        //std::cout << "/* 0x" << std::uppercase<< std::hex << i << std::dec << " */ ";
        /*
        print_instruction();
    }
    */
}

void emulator::CPU::reset()
{
    program_counter_ = read16(0xFFFC);
    stack_  = 0xFD;
    status_ = 0x24;
}

uint16_t emulator::CPU::program_counter() const
{
    return program_counter_;
}

uint8_t emulator::CPU::accumulator() const
{
    return accumulator_;
}

uint8_t emulator::CPU::x_register() const
{
    return x_register_;
}

uint8_t emulator::CPU::y_register() const
{
    return y_register_;
}

uint8_t emulator::CPU::stack() const
{
    return stack_;
}

uint16_t emulator::CPU::zero_page_get_address(uint8_t cpu_register)
{
    auto address = read8(program_counter_ + 1) + cpu_register;

    if (is_page_crossed(address - cpu_register, address))
    {
        current_cycles_++;
    }

    return address;
}

uint16_t emulator::CPU::absolute_get_address(uint8_t cpu_register)
{
    auto address = read16(program_counter_ + 1) + cpu_register;

    if (is_page_crossed(address - cpu_register, address))
    {
        current_cycles_++;
    }

    return address;
}

uint16_t emulator::CPU::address_to_arguemnts()
{
    auto op   = read8(program_counter_);
    auto info = instruction[op];

    switch (info.mode)
    {
        case OpMode::zero_page_x:
        {
            return zero_page_get_address(x_register_);
        }
        case OpMode::zero_page_y:
        {
            return zero_page_get_address(y_register_);
        }
        case OpMode::absolute_x:
        {
            return absolute_get_address(x_register_);
        }
        case OpMode::absolute_y:
        {
            return absolute_get_address(y_register_);
        }
        case OpMode::indexed_x:
        {
            return read16(read8(program_counter_ + 1) + x_register_);
        }
        case OpMode::indexed_y:
        {
            return read16(read8(program_counter_ + 1) + y_register_);
        }
        case OpMode::implicit:
        {
            return 0;
        }
        case OpMode::accumulator:
        {
            return 0;
        }
        case OpMode::immediate:
        {
            return program_counter_ + 1;
        }
        case OpMode::zero_page:
        {
            return read8(program_counter_ + 1);
        }
        case OpMode::absolute:
        {
            return read16(program_counter_ + 1);
        }
        case OpMode::relative:
        {
            uint16_t offset = read8(program_counter_ + 1);

            // Negative
            if (offset & 0x80)
            {
                return program_counter_ + 2 + offset - 0x100;
            }
            else
            {
                return program_counter_ + 2 + offset;
            }
        }
        case OpMode::indirect:
            return read16(read8(program_counter_ + 1));
    }

    throw std::runtime_error("Invalid mode for op code");
}

void emulator::CPU::set_program_counter(uint16_t address)
{
    program_counter_ = address;
}

void emulator::CPU::set_accumulator(uint8_t a)
{
    accumulator_ = a;
}

void emulator::CPU::set_x_register(uint8_t x)
{
    x_register_ = x;
}

void emulator::CPU::set_y_register(uint8_t y)
{
    y_register_ = y;
}

void emulator::CPU::set_stack(uint8_t sp)
{
    stack_ = sp;
}

uint8_t emulator::CPU::status() const
{
    return status_;
}

void emulator::CPU::update_flags(std::function<bool()> const& f, uint8_t flags)
{
    if (f())
    {
        add_flags(flags);
    }
    else
    {
        remove_flags(flags);
    }
}

void emulator::CPU::add_flags(uint8_t flags)
{
    status_ |= flags;
}

void emulator::CPU::remove_flags(uint8_t flags)
{
    status_ &= ~flags;
}

bool emulator::CPU::carry() const
{
    return status_ & emulator::carry;
}

bool emulator::CPU::zero() const
{
    return status_ & emulator::zero;
}

bool emulator::CPU::interrupt() const
{
    return status_ & emulator::interrupt;
}

bool emulator::CPU::decimal() const
{
    return status_ & emulator::decimal;
}

bool emulator::CPU::break_interrupt() const
{
    return status_ & emulator::brk_inter;
}

bool emulator::CPU::overflow() const
{
    return status_ & emulator::overflow;
}

bool emulator::CPU::sign() const
{
    return status_ & emulator::sign;
}

emulator::OpMode emulator::CPU::current_mode() const
{
    auto op = read8(program_counter_);
    auto info = instruction[op];

    return info.mode;
}

/*
    http://wiki.nesdev.com/w/index.php/CPU_memory_map

    0xFFFF cpu addressable space

    Address range     Size     Device
    ------------------------------------
    0x0000 - 0x07FF : 0x0800 :  2KB internal ram
    0x0800 - 0x0FFF : 0x0800 :  Mirrors of 0x0000 - 0x07FF
    0x1000 - 0x17FF : 0x0800 :      ^           ^
    0x1800 - 0x1FFF : 0x0800 :      ^           ^
    0x2000 - 0x2007 : 0x0008 : NES PPU Registers
    0x2008 - 0x3FFF : 0x1FF8 : Mirros of 0x2000 - 0x2007 (repeats every 8 bytes)
    0x4000 - 0x4017 : 0x0018 : NES APU and I/O Registers
    0x4018 - 0x401F : 0x0008 : APU and I/O Functionality that is normally disabled
    0x4020 - 0xFFFF : 0xBFE0 : Catridge space: PRG ROM, PRG RAM, and mapper registers
*/
uint8_t emulator::CPU::read8(uint16_t address) const
{
    if (address < 0x2000)
    {
        return memory.read8(address % 0x0800);
    }
    else if (address < 0x4000)
    {
        // Since the memory repeats every 8 bytes lets just use the same 8 byte location
        return memory.read8(0x2000 + address % 8);
    }

    return memory.read8(address);
}

uint16_t emulator::CPU::read16(uint16_t address) const
{
    // TODO handle other type of memory... ram/ppu/apu?
    auto low  = read8(address);
    auto high = read8(address + 1);

    return low | high << 8;
}

void emulator::CPU::write8(uint16_t address, uint8_t value)
{
    if (address < 0x2000)
    {
        memory.write8(address % 0x0800, value);
    }
    else if (address < 0x4000)
    {
        // TODO Figure out how to best get the ppu registers from the ppu memory
        //uint8_t ppu_address = 0x2000 + address % 8;
        //ppu_registers[ppu_address] = value;
    }

    memory.write8(address, value);
}

void emulator::CPU::push(uint8_t byte)
{
    write8(stack_--, byte);
}

uint8_t emulator::CPU::pop()
{
    return read8(++stack_);
}

void emulator::CPU::add_branch_cycle(uint16_t address)
{
    current_cycles_++;
    if (is_page_crossed(program_counter_, address))
    {
        current_cycles_++;
    }
}

void emulator::CPU::handle_non_maskable_interrupt()
{
    nmi_interrupt = true;
}

void emulator::CPU::handle_interrupt_request()
{
    if (nmi_interrupt == false)
    {
        irq_interrupt = true;
    }
}

void emulator::CPU::check_for_interrupt()
{
    if (nmi_interrupt)
    {
        nmi(this);
        nmi_interrupt = false;
        current_cycles_ += 7;
    }
    else if (irq_interrupt)
    {
        irq(this);
        irq_interrupt = false;
        current_cycles_ += 7;
    }
}

uint8_t emulator::CPU::step()
{
    auto pc = program_counter_;
    auto op = instruction[read8(pc)];
    current_cycles_ = op.number_cycles;

    check_for_interrupt();

    print_instruction();

    op.func(this);

    // No op moved the pc, so lets move up ourselfs
    if (pc == program_counter_)
    {
        program_counter_ += op.number_bytes;
    }

    cycles_ += current_cycles_;
    return current_cycles_;
}

void emulator::CPU::print_instruction() const
{
    auto op = read8(program_counter_);
    auto info = instruction[op];
    std::string mode_str = mode_name(static_cast<OpMode>(info.mode));

    std::cout << "PC: " << std::hex << "0x" << program_counter_ << " "
              << "A: 0x" << (int)accumulator_ << " "
              << "X: 0x" << (int)x_register_ << " "
              << "Y: 0x" << (int)y_register_ << " "
              << "SP: 0x" << (int)stack_ << std::endl;

    std::cout << mode_str << " " << info.name
        << "(0x" << std::hex << (int)op << std::dec << ")";

    if (info.number_bytes > 1)
    {
        auto a1 = read8(program_counter_ + 1);
        std::cout << " " << std::hex << "0x" << (int)a1 << " ";

        if (info.number_bytes > 2)
        {
            auto a2 = read8(program_counter_ + 2);
            std::cout << std::hex << "0x" << (int)a2 << " ";
        }
    }

    std::cout << std::endl;
    /*

    std::cout << std::hex
              << " C 0x" << (status_ & emulator::carry)
              << " Z 0x" << (status_ & emulator::zero)
              << " I 0x" << (status_ & emulator::interrupt)
              << " D 0x" << (status_ & emulator::decimal)
              << " B 0x" << (status_ & emulator::brk_inter)
              << " V 0x" << (status_ & emulator::overflow)
              << " Z 0x" << (status_ & emulator::sign)
              << std::endl;
              */
}

// FIXME DEBUG ONLY
void emulator::CPU::dump_ram() const
{

    for (auto i = 0u; i < 0x1000; i++)
    {
        if (memory.read8(i) <= 0xF)
        {
            std::cout << 0;
        }
        std::cout << std::hex << (int)memory.read8(i);

        if ((i + 1) % 2 == 0)
        {
            std::cout << " ";
        }

        //if ((i + 1) % 16 == 0)
        if ((i + 1) % 32 == 0)
        {
            std::cout << std::endl;
        }
    }
}
