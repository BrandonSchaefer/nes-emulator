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

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "cpu.h"

namespace
{
uint16_t const default_pc{0x0};
uint8_t const default_acc{0x1};
struct TestCPUInstructions : ::testing::Test
{
    void SetUp() override
    {
        cpu.set_program_counter(default_pc);
        cpu.set_accumulator(default_acc);
    }

    emulator::CPU cpu;
};
}
/*
void emulator::nd(CPU* cpu)
void emulator::asl(CPU* cpu)
void emulator::bcc(CPU* cpu)
void emulator::bcs(CPU* cpu)
void emulator::beq(CPU* cpu)
void emulator::bit(CPU* cpu)
void emulator::bmi(CPU* cpu)
void emulator::bne(CPU* cpu)
void emulator::bpl(CPU* cpu)
void emulator::brk(CPU* cpu)
void emulator::bvc(CPU* cpu)
void emulator::bvs(CPU* cpu)
void emulator::clc(CPU* cpu)
void emulator::cld(CPU* cpu)
void emulator::cli(CPU* cpu)
void emulator::clv(CPU* cpu)
void emulator::cmp(CPU* cpu)
void emulator::cpx(CPU* cpu)
void emulator::cpy(CPU* cpu)
void emulator::dec(CPU* cpu)
void emulator::dex(CPU* cpu)
void emulator::dey(CPU* cpu)
void emulator::eor(CPU* cpu)
void emulator::inc(CPU* cpu)
void emulator::inx(CPU* cpu)
void emulator::iny(CPU* cpu)
void emulator::jmp(CPU* cpu)
void emulator::jsr(CPU* cpu)
void emulator::lda(CPU* cpu)
void emulator::ldx(CPU* cpu)
void emulator::ldy(CPU* cpu)
void emulator::lsr(CPU* cpu)
void emulator::nop(CPU* cpu)
void emulator::ora(CPU* cpu)
void emulator::pha(CPU* cpu)
void emulator::php(CPU* cpu)
void emulator::pla(CPU* cpu)
void emulator::plp(CPU* cpu)
void emulator::rol(CPU* cpu)
void emulator::ror(CPU* cpu)
void emulator::rti(CPU* cpu)
void emulator::rts(CPU* cpu)
void emulator::sbc(CPU* cpu)
void emulator::sec(CPU* cpu)
void emulator::sed(CPU* cpu)
void emulator::sei(CPU* cpu)
void emulator::sta(CPU* cpu)
void emulator::stx(CPU* cpu)
void emulator::sty(CPU* cpu)
void emulator::tax(CPU* cpu)
void emulator::tay(CPU* cpu)
void emulator::tsx(CPU* cpu)
void emulator::txa(CPU* cpu)
void emulator::txs(CPU* cpu)
void emulator::tya(CPU* cpu)
*/

TEST_F(TestCPUInstructions, test_adc)
{
    // Write to the first spot op code ADC - immediate
    cpu.write8(0x0, 0x69);
    // Write 5 to the argument for ADC
    cpu.write8(0x1, 0x05);

    emulator::adc(&cpu);

    EXPECT_EQ(cpu.accumulator(), 0x5 + default_acc);
}

TEST_F(TestCPUInstructions, test_adc_with_carry)
{
    cpu.add_flags(emulator::carry);
    // op code ADC - immediate
    cpu.write8(0x0, 0x69);
    cpu.write8(0x1, 0x05);

    emulator::adc(&cpu);

    EXPECT_EQ(cpu.accumulator(), 0x5 + default_acc + 1);
}

TEST_F(TestCPUInstructions, test_and)
{
    // op code AND - immediate
    cpu.write8(0x0, 0x29);
    cpu.write8(0x1, 0x2);

    emulator::nd(&cpu);

    EXPECT_EQ(cpu.accumulator(), 0x2 & default_acc);
}

TEST_F(TestCPUInstructions, test_asl)
{
    // Testing non acc mode, should shift mem location 1 left
    // op code ASL - absolute
    cpu.write8(0x0, 0x0E);
    cpu.write8(0x1, 0x2);

    emulator::asl(&cpu);

    EXPECT_EQ(cpu.read8(0x1), 0x2 << 1);
}

TEST_F(TestCPUInstructions, test_asl_acc)
{
    // Testing acc mode, should shift acc location 1 left
    // op code ASL - accumulator 
    cpu.write8(0x0, 0x0A);

    emulator::asl(&cpu);

    EXPECT_EQ(cpu.accumulator(), default_acc << 1);
}
