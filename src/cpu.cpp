//-*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
/* The MIT License (MIT)
 *
 * Copyright (c) 2016 Brandon Schaefer
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
    program_counter_ = 0x0;
    memory[1] = 0x50;
    memory[2] = 0xFF;

    for (size_t i = 0; i < 0x100; i++)
    {
        memory[0] = i;
        std::cout << "/* 0x" << std::uppercase<< std::hex << i << std::dec << " */ ";
        print_instruction();
    }
}

uint16_t emulator::CPU::read16(uint16_t address) const
{
    // TODO handle other type of memory... ram/ppu/apu
    auto low  = memory[address];
    auto high = memory[address + 1];

    return low | high << 8;
}

uint8_t emulator::CPU::read8(uint16_t address) const
{
    return memory[address];
}

void emulator::CPU::write8(uint16_t address, uint8_t value)
{
    memory[address] = value;
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
        std::cout << " " << std::hex << (int)a1 << " ";

        if (info.number_args > 2)
        {
            auto a2 = read8(program_counter_ + 2);
            std::cout << std::hex << (int)a2 << " ";
        }
    }
    std::cout << std::dec << std::endl;
}
