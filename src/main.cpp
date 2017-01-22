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

#include <bitset>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

#include "cpu.h"

namespace
{
uint32_t const expected_magic_nes_header{0x1a53454e};
uint32_t const kilobyte{1024};
}

std::vector<uint8_t> read_in_file(std::string const& path)
{
    std::ifstream is(path, std::ifstream::binary);
    is >> std::noskipws;
    return std::vector<uint8_t>(std::istream_iterator<uint8_t>(is), {});
}

struct RomHeader
{
    uint32_t nes_magic;
    uint8_t  prg_size;
    uint8_t  chr_size;
    uint8_t  flags_six;
    uint8_t  flags_seven;
    uint8_t  prg_ram_size;
    uint8_t  flags_nine;
};

std::ostream& operator<<(std::ostream& os, RomHeader const& header)
{
    return os << "PRG ROM: " << std::hex << "0x" << (int)header.prg_size << std::endl
              << "CHR ROM: " << std::hex << "0x" << (int)header.chr_size << std::endl
              << "Flgas 6: " << std::bitset<8>(header.flags_six) << std::endl
              << "Flgas 7: " << std::bitset<8>(header.flags_seven) << std::endl
              << "PRG RAM: " << std::hex << "0x" << (int)header.prg_ram_size << std::endl
              << "Flgas 9: " << std::bitset<8>(header.flags_nine);
}

int main()
{
    emulator::CPU cpu;

    auto raw_rom = read_in_file("../super_mario.nes");
    uint32_t current_byte = 0x10;

    if (raw_rom.size() < 0xF)
    {
        std::cerr << "Invalid size, less then the expect" << std::endl;
        return -1;
    }

    RomHeader header = *reinterpret_cast<RomHeader*>(raw_rom.data());

    if (header.nes_magic != expected_magic_nes_header)
    {
        std::cerr << "Incorrect file type, expect *.nes" << std::endl;
        return -1;
    }

    auto lower_nibble = header.flags_six >> 4;
    auto upper_nibble = header.flags_seven >> 4;
    auto mapper = lower_nibble | upper_nibble << 4;

    auto mirror_mode    = header.flags_six & 0x1;
    auto disable_mirror = (header.flags_six >> 3) & 0x1;

    auto battery = (header.flags_six >> 1) & 0x1;

    auto trainer = (header.flags_six >> 2) & 0x1;

    // TODO
    if (trainer)
    {
        current_byte += 0x200;
    }

    auto prg_byte_size = header.prg_size * kilobyte * 16;
    auto chr_byte_size = header.chr_size * kilobyte * 8;

    if (raw_rom.size() != current_byte + prg_byte_size + chr_byte_size)
    {
        // TODO need to account for extra things like PlayerChoice10
        std::cerr << "Assumption on file size. We to many or to few bytes in the *.nes file!" << std::endl;
    }

    std::vector<uint8_t> prg(raw_rom.data() + current_byte, raw_rom.data() + current_byte + prg_byte_size);
    current_byte += prg_byte_size;

    std::vector<uint8_t> chr(raw_rom.data() + current_byte, raw_rom.data() + current_byte + chr_byte_size);
    current_byte += chr_byte_size;

    std::cout << header << " " << std::endl
              << "Mapper number: "  << std::hex << "0x" << mapper << std::endl << std::dec
              << "MirrorMode: " << (mirror_mode ? "Vertical" : "Horizontal") << std::endl
              << "DisableMirror: " << disable_mirror << std::endl
              << "Battery: " << battery << std::endl
              << "Trainer: " << trainer << std::endl
              << "PrgBytes: " << prg.size() << std::endl
              << "ChrBytes: " << chr.size() << std::endl;

    //std::copy(prg.begin(), prg.end(), cpu.memory.begin() + 0x8000);

    for (auto i = 0u; i < 22; i++)
    {
        //printf("PC: %i\n", cpu.program_counter());
        printf("%i\n", (int)cpu.step());
    }

    return 0;
}
