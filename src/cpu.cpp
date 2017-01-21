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

}

emulator::CPU::CPU()
{
    memset(memory.data(), 0, memory.size());
    reset();

    // TESTING
    /*
    program_counter_ = 0x0;
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
    stack_ = 0xFD;
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

uint16_t emulator::CPU::address_to_arguemnts() const
{
    auto op   = read8(program_counter_);
    auto info = instruction[op];

    switch (info.mode)
    {
        case OpMode::zero_page_x:
            break;
        case OpMode::zero_page_y:
            break;
        case OpMode::absolute_x:
            break;
        case OpMode::absolute_y:
            break;
        case OpMode::indexed_x:
            break;
        case OpMode::indexed_y:
            break;
        case OpMode::implicit:
            break;
        case OpMode::accumulator:
            break;
        case OpMode::immediate:
            break;
        case OpMode::zero_page:
            break;
        case OpMode::absolute:
            break;
        case OpMode::relative:
            break;
        case OpMode::indirect:
            break;
    }

    // TODO Go throught he modes, for now assume immediate
    return program_counter_ + 1;
}

void emulator::CPU::set_program_counter(uint16_t address)
{
    program_counter_ = address;
}

void emulator::CPU::set_accumulator(uint8_t value)
{
    accumulator_ = value;
}

void emulator::CPU::set_x_register(uint8_t value)
{
    x_register_ = value;
}

void emulator::CPU::set_y_register(uint8_t value)
{
    y_register_ = value;
}

uint8_t emulator::CPU::status() const
{
    return status_;
}

void emulator::CPU::update_flags(std::function<bool()> const& f, CPUFlag flags)
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

void emulator::CPU::add_flags(CPUFlag flags)
{
    status_ |= flags;
}

void emulator::CPU::remove_flags(CPUFlag flags)
{
    status_ &= ~flags;
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
        return memory[address % 0x0800];
    }
    else if (address < 0x4000)
    {
        return memory[address % 8];
    }

    return memory[address];
}

uint16_t emulator::CPU::read16(uint16_t address) const
{
    // TODO handle other type of memory... ram/ppu/apu
    auto low  = read8(address);
    auto high = read8(address + 1);

    return low | high << 8;
}

void emulator::CPU::write8(uint16_t address, uint8_t value)
{
    if (address < 0x2000)
    {
        memory[address % 0x0800] = value;
    }
    else if (address < 0x4000)
    {
        memory[address % 8] = value;
    }

    memory[address] = value;
}

void emulator::CPU::push(uint8_t byte)
{
    stack_++;
    write8(stack_, byte);
}

uint8_t emulator::CPU::pop()
{
    stack_--;
    return read8(stack_);
}

void emulator::CPU::print_instruction() const
{
    auto op = read8(program_counter_);
    auto info = instruction[op];
    std::string mode_str = mode_name(static_cast<OpMode>(info.mode));

    std::cout << mode_str << " " << info.name
        << "(0x" << std::hex << (int)op << std::dec << ")";

    if (info.number_args > 1)
    {
        auto a1 = read8(program_counter_ + 1);
        std::cout << " " << std::hex << "0x" << (int)a1 << " ";

        if (info.number_args > 2)
        {
            auto a2 = read8(program_counter_ + 2);
            std::cout << std::hex << "0x" << (int)a2 << " ";
        }
    }

    std::cout << std::endl;

    std::cout << std::hex
              << " C 0x" << (status_ & carry)
              << " Z 0x" << (status_ & zero)
              << " I 0x" << (status_ & interrupt)
              << " D 0x" << (status_ & decimal)
              << " B 0x" << (status_ & brk_inter)
              << " V 0x" << (status_ & overflow)
              << " Z 0x" << (status_ & sign)
              << std::endl;
}
