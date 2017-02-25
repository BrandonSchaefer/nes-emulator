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

#ifndef NES_EMULATOR_MEMORY_H_
#define NES_EMULATOR_MEMORY_H_

#include <string.h>

namespace emulator
{

template <uint64_t Size>
class Memory
{
public:
    Memory();

    uint8_t  read8 (uint16_t address) const;
    uint16_t read16(uint16_t address) const;

    void write8 (uint16_t address, uint8_t value);
    void write16(uint16_t address, uint16_t value);

private:
    std::array<uint8_t, Size> memory;
};

template <uint64_t Size>
Memory<Size>::Memory()
{
    memset(memory.data(), 0, memory.size());
}

template <uint64_t Size>
uint8_t Memory<Size>::read8(uint16_t address) const
{
    return memory[address];
}

template <uint64_t Size>
uint16_t Memory<Size>::read16(uint16_t address) const
{
    auto low  = read8(address);
    auto high = read8(address + 1);

    return low | high << 8;
}

template <uint64_t Size>
void Memory<Size>::write8(uint16_t address, uint8_t value)
{
    memory[address] = value;
}

template <uint64_t Size>
void Memory<Size>::write16(uint16_t address, uint16_t value)
{
    write8(address,     value & 0xFF);
    write8(address + 1, value >> 8 & 0xFF);
}

}

#endif /* NES_EMULATOR_MEMORY_H_ */
