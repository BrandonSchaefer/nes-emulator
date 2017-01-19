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


/*

6502 CPU:

Accumulator Register
X Register
Y Register
Status Register
     Bit No.       7   6   5   4   3   2   1   0
                   S   V       B   D   I   Z   C

     C Carry
     Z Zero if any arithmetic or logical operator produces a zero result
     I enable/disable interrupt
     D Decimal mode status flag. BCD?
     B Set when a softwar einterrupt is executed BRK instruction
     V Overflow flag
     S Sign flag

Program counter
    Contains the address of the current machine language instruction
    being exected. 
Stack Pointer
    Contains the location of the first empty place of the stack.


Addressing Modes

Immediate
Absolute
Zero Page absolute
Implied
Accumlator
Index X
Zero page indexed X
Index Y
Zero page indexed Y
Indirect
Pre-index indriect
    Dont forget to wrap (0xFF + 0x02) % 0xFF! Only X is used this mode
Post-index indriect
    Only Y is used in this mode
Relative


*/

#ifndef NES_EMULATOR_CPU_H_
#define NES_EMULATOR_CPU_H_ 

#include <cmath>
#include <cstdint>
#include <array>

namespace emulator
{

class CPU
{
public:
    CPU();

    uint8_t  read8 (uint16_t address) const;
    uint16_t read16(uint16_t address) const;

    void write8(uint16_t address, uint8_t value);

private:
    void print_instruction() const;

    uint16_t program_counter_{0x34};
    uint8_t a_{0};
    uint8_t x_{0};
    uint8_t y_{0};
    uint8_t stack_{0};
    uint8_t status_{0xFD};

    // 0xFFFF addressable space
    std::array<uint8_t, 65536> memory;
};

}

#endif /* NES_EMULATOR_CPU_H_ */
