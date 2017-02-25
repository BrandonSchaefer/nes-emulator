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

#ifndef NES_EMULATOR_TESTS_MOCK_CPU_H_
#define NES_EMULATOR_TESTS_MOCK_CPU_H_

#include "mocks/ppu.h"

struct MockCPU : emulator::CPU
{
    MockPPU ppu;

    MockCPU() :
        CPU(&ppu)
    {
    }

    // In case someone wants to override the ppu with a different mock
    MockCPU(emulator::PPU const* ppu) :
        CPU(ppu)
    {
    }

    MOCK_METHOD0(reset, void());
    MOCK_CONST_METHOD1(read8, uint8_t(uint16_t address));
    MOCK_CONST_METHOD1(read16, uint16_t(uint16_t address));

    MOCK_METHOD2(write8, void(uint16_t address, uint8_t value));

    MOCK_METHOD0(address_to_arguemnts, uint16_t());

    MOCK_CONST_METHOD0(program_counter, uint16_t());
    MOCK_CONST_METHOD0(accumulator, uint8_t());
    MOCK_CONST_METHOD0(x_register, uint8_t());
    MOCK_CONST_METHOD0(y_register, uint8_t());
    MOCK_CONST_METHOD0(stack, uint8_t());

    MOCK_METHOD1(set_program_counter, void(uint16_t address));
    MOCK_METHOD1(set_accumulator, void(uint8_t a));
    MOCK_METHOD1(set_x_register, void(uint8_t x));
    MOCK_METHOD1(set_y_register, void(uint8_t y));
    MOCK_METHOD1(set_stack, void(uint8_t sp));

    MOCK_METHOD0(status, uint8_t());
    MOCK_METHOD2(update_flags, void(std::function<bool()> const& f, uint8_t flags));
    MOCK_METHOD1(add_flags, void(uint8_t flags));
    MOCK_METHOD1(remove_flags, void(uint8_t flags));

    MOCK_CONST_METHOD0(carry, bool());
    MOCK_CONST_METHOD0(zero, bool());
    MOCK_CONST_METHOD0(interrupt, bool());
    MOCK_CONST_METHOD0(decimal, bool());
    MOCK_CONST_METHOD0(break_interrupt, bool());
    MOCK_CONST_METHOD0(overflow, bool());
    MOCK_CONST_METHOD0(sign, bool());

    MOCK_CONST_METHOD0(current_mode, emulator::OpMode());
    MOCK_METHOD1(push, void(uint8_t byte));
    MOCK_METHOD0(pop, uint8_t());

    MOCK_CONST_METHOD2(is_page_crossed, bool(uint16_t a, uint16_t b));

    MOCK_METHOD0(step, uint8_t());
    MOCK_CONST_METHOD0(print_instruction, void());

    MOCK_METHOD1(add_branch_cycle, void(uint16_t address));
};

#endif /* NES_EMULATOR_TESTS_MOCK_CPU_H_ */
