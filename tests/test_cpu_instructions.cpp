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
uint8_t const default_acc{0x32};
uint8_t const default_x{0x56};
uint8_t const default_y{0xF0};
uint8_t const default_sp{0x0F};

struct MockCPU : emulator::CPU
{
    MOCK_METHOD1(add_branch_cycle, void(uint16_t address));
};

struct TestCPUInstructions : ::testing::Test
{
    void SetUp() override
    {
        cpu.set_program_counter(default_pc);
        cpu.set_accumulator(default_acc);
        cpu.set_x_register(default_x);
        cpu.set_y_register(default_y);
        cpu.set_stack(default_sp);
    }

    MockCPU cpu;
};
}

/*
void emulator::brk(CPU* cpu)
void emulator::jsr(CPU* cpu)
void emulator::rti(CPU* cpu)
void emulator::rts(CPU* cpu)
*/

TEST_F(TestCPUInstructions, test_adc)
{
    // op code ADC - immediate
    cpu.write8(default_pc, 0x69);
    // Write 5 to the argument for ADC
    cpu.write8(default_pc + 1, 0x05);

    emulator::adc(&cpu);

    EXPECT_EQ(cpu.accumulator(), 0x5 + default_acc);
}

TEST_F(TestCPUInstructions, test_adc_with_carry)
{
    cpu.add_flags(emulator::carry);
    // op code ADC - immediate
    cpu.write8(default_pc, 0x69);
    cpu.write8(default_pc + 1, 0x05);

    emulator::adc(&cpu);

    EXPECT_EQ(cpu.accumulator(), 0x5 + default_acc + 1);
}

TEST_F(TestCPUInstructions, test_and)
{
    // op code AND - immediate
    cpu.write8(default_pc, 0x29);
    cpu.write8(default_pc + 1, 0x2);

    emulator::nd(&cpu);

    EXPECT_EQ(cpu.accumulator(), 0x2 & default_acc);
}

TEST_F(TestCPUInstructions, test_asl)
{
    // op code ASL - absolute
    cpu.write8(default_pc, 0x0E);
    // Testing non acc mode, should shift mem location 1 left
    cpu.write8(default_pc + 1, default_pc + 3);
    cpu.write8(default_pc + 3, default_acc);

    emulator::asl(&cpu);

    EXPECT_EQ(cpu.read8(default_pc + 3), default_acc << 1);
}

TEST_F(TestCPUInstructions, test_asl_acc)
{
    // Testing acc mode, should shift acc location 1 left
    // op code ASL - accumulator 
    cpu.write8(default_pc, 0x0A);

    emulator::asl(&cpu);

    EXPECT_EQ(cpu.accumulator(), default_acc << 1);
}

TEST_F(TestCPUInstructions, test_rol)
{
    // op code ROL - absolute
    cpu.write8(default_pc, 0x2E);
    cpu.write8(default_pc + 1, default_pc + 3);
    cpu.write8(default_pc + 3, 0x80);

    emulator::rol(&cpu);

    // With no carry set, we'll have 0 after the shift
    EXPECT_EQ(cpu.read8(default_pc + 3), 0);
}

TEST_F(TestCPUInstructions, test_rol_with_carry)
{
    // op code ROL - absolute
    cpu.write8(default_pc, 0x2E);
    cpu.write8(default_pc + 1, default_pc + 3);
    cpu.write8(default_pc + 3, 0x80);

    cpu.add_flags(emulator::carry);

    emulator::rol(&cpu);

    // With carry set, we'll move that into 0x1
    EXPECT_EQ(cpu.read8(default_pc + 3), 1);
}

TEST_F(TestCPUInstructions, test_rol_acc)
{
    // op code ROL - accumulator
    cpu.write8(default_pc, 0x2A);
    cpu.set_accumulator(0x80);

    emulator::rol(&cpu);

    // With no carry set, we'll have 0 after the shift
    EXPECT_EQ(cpu.accumulator(), 0);
}

TEST_F(TestCPUInstructions, test_rol_acc_with_carry)
{
    // op code ROL - accumulator
    cpu.write8(default_pc, 0x2A);
    cpu.set_accumulator(0x80);

    cpu.add_flags(emulator::carry);

    emulator::rol(&cpu);

    // With carry set, we'll move that into 0x1
    EXPECT_EQ(cpu.accumulator(), 1);
}

TEST_F(TestCPUInstructions, test_ror)
{
    // op code ROR - absolute
    cpu.write8(default_pc, 0x6E);
    cpu.write8(default_pc + 1, default_pc + 3);
    cpu.write8(default_pc + 3, 0x01);

    emulator::ror(&cpu);

    // With no carry set, we'll have 0 after the shift
    EXPECT_EQ(cpu.read8(default_pc + 3), 0);
}

TEST_F(TestCPUInstructions, test_ror_with_carry)
{
    // op code ROR - absolute
    cpu.write8(default_pc, 0x6E);
    cpu.write8(default_pc + 1, default_pc + 3);
    cpu.write8(default_pc + 3, 0x01);

    cpu.add_flags(emulator::carry);

    emulator::ror(&cpu);

    // With carry set, we'll move it to 0x80
    EXPECT_EQ(cpu.read8(default_pc + 3), 0x80);
}

TEST_F(TestCPUInstructions, test_ror_acc)
{
    // op code ROR - accumulator
    cpu.write8(default_pc, 0x6A);
    cpu.set_accumulator(0x1);

    emulator::ror(&cpu);

    // With no carry set, we'll have 0 after the shift
    EXPECT_EQ(cpu.accumulator(), 0);
}

TEST_F(TestCPUInstructions, test_ror_acc_with_carry)
{
    // op code ROR - accumulator
    cpu.write8(default_pc, 0x6A);
    cpu.set_accumulator(0x1);

    cpu.add_flags(emulator::carry);

    emulator::ror(&cpu);

    // With carry set, we'll move it to 0x80
    EXPECT_EQ(cpu.accumulator(), 0x80);
}

TEST_F(TestCPUInstructions, test_clc)
{
    cpu.add_flags(emulator::carry);

    emulator::clc(&cpu);

    EXPECT_FALSE(cpu.status() & emulator::carry);
}

TEST_F(TestCPUInstructions, test_cld)
{
    cpu.add_flags(emulator::decimal);

    emulator::cld(&cpu);

    EXPECT_FALSE(cpu.status() & emulator::decimal);
}

TEST_F(TestCPUInstructions, test_cli)
{
    cpu.add_flags(emulator::interrupt);

    emulator::cli(&cpu);

    EXPECT_FALSE(cpu.status() & emulator::interrupt);

}

TEST_F(TestCPUInstructions, test_clv)
{
    cpu.add_flags(emulator::overflow);

    emulator::clv(&cpu);

    EXPECT_FALSE(cpu.status() & emulator::overflow);

}

TEST_F(TestCPUInstructions, test_sec)
{
    emulator::sec(&cpu);

    EXPECT_TRUE(cpu.status() & emulator::carry);
}

TEST_F(TestCPUInstructions, test_sed)
{
    emulator::sed(&cpu);

    EXPECT_TRUE(cpu.status() & emulator::decimal);
}

TEST_F(TestCPUInstructions, test_sei)
{
    emulator::sei(&cpu);

    EXPECT_TRUE(cpu.status() & emulator::interrupt);
}

TEST_F(TestCPUInstructions, test_sta)
{
    // op code STA - absolute
    cpu.write8(default_pc, 0x8D);
    cpu.write8(default_pc + 1, default_pc + 3);
    cpu.write8(default_pc + 3, 0x1);

    emulator::sta(&cpu);

    EXPECT_EQ(cpu.read8(default_pc + 3), default_acc);
}

TEST_F(TestCPUInstructions, test_stx)
{
    // op code STX - absolute
    cpu.write8(default_pc, 0x8E);
    cpu.write8(default_pc + 1, default_pc + 3);
    cpu.write8(default_pc + 3, 0x1);

    emulator::stx(&cpu);

    EXPECT_EQ(cpu.read8(default_pc + 3), default_x);
}

TEST_F(TestCPUInstructions, test_sty)
{
    // op code STY - absolute
    cpu.write8(default_pc, 0x8C);
    cpu.write8(default_pc + 1, default_pc + 3);
    cpu.write8(default_pc + 3, 0x1);

    emulator::sty(&cpu);

    EXPECT_EQ(cpu.read8(default_pc + 3), default_y);
}

TEST_F(TestCPUInstructions, test_tax)
{
    emulator::tax(&cpu);

    EXPECT_EQ(cpu.x_register(), default_acc);
}

TEST_F(TestCPUInstructions, test_tay)
{
    emulator::tay(&cpu);

    EXPECT_EQ(cpu.y_register(), default_acc);
}

TEST_F(TestCPUInstructions, test_tsx)
{
    emulator::tsx(&cpu);

    EXPECT_EQ(cpu.x_register(), default_sp);
}

TEST_F(TestCPUInstructions, test_txa)
{
    emulator::txa(&cpu);

    EXPECT_EQ(cpu.accumulator(), default_x);
}

TEST_F(TestCPUInstructions, test_txs)
{
    emulator::txs(&cpu);

    EXPECT_EQ(cpu.stack(), default_x);
}

TEST_F(TestCPUInstructions, test_tya)
{
    emulator::tya(&cpu);

    EXPECT_EQ(cpu.accumulator(), default_y);
}

TEST_F(TestCPUInstructions, test_lda)
{
    // op code LDA - immediate
    cpu.write8(default_pc, 0xA9);
    cpu.write8(default_pc + 1, 0x56);

    emulator::lda(&cpu);

    EXPECT_EQ(cpu.accumulator(), 0x56);
}

TEST_F(TestCPUInstructions, test_ldx)
{
    // op code LDX - immediate
    cpu.write8(default_pc, 0xA2);
    cpu.write8(default_pc + 1, 0x56);

    emulator::ldx(&cpu);

    EXPECT_EQ(cpu.x_register(), 0x56);
}

TEST_F(TestCPUInstructions, test_ldy)
{
    // op code LDY - immediate
    cpu.write8(default_pc, 0xA0);
    cpu.write8(default_pc + 1, 0x56);

    emulator::ldy(&cpu);

    EXPECT_EQ(cpu.y_register(), 0x56);
}

TEST_F(TestCPUInstructions, test_pha)
{
    emulator::pha(&cpu);

    EXPECT_EQ(cpu.pop(), default_acc);
}

TEST_F(TestCPUInstructions, test_php)
{
    cpu.add_flags(0x75);

    emulator::php(&cpu);

    EXPECT_EQ(cpu.pop(), 0x75);
}

TEST_F(TestCPUInstructions, test_pla)
{
    cpu.push(default_acc + 1);

    emulator::pla(&cpu);
    
    EXPECT_EQ(cpu.accumulator(), default_acc + 1);
}

TEST_F(TestCPUInstructions, test_plp)
{
    cpu.push(0x75);

    emulator::plp(&cpu);

    EXPECT_EQ(cpu.status(), 0x75);
}

TEST_F(TestCPUInstructions, test_ora)
{
    cpu.write8(default_pc + 1, 0x2);

    emulator::ora(&cpu);

    EXPECT_EQ(cpu.accumulator(), default_acc | 0x2);
}

TEST_F(TestCPUInstructions, test_lsr)
{
    // op code LSR - absolute
    cpu.write8(default_pc, 0x4E);
    cpu.write8(default_pc + 1, default_pc + 3);
    cpu.write8(default_pc + 3, default_acc);

    emulator::lsr(&cpu);

    EXPECT_EQ(cpu.read8(default_pc + 3), default_acc >> 1);
}

TEST_F(TestCPUInstructions, test_lsr_acc)
{
    // op code LSR - accumulator
    cpu.write8(default_pc, 0x4A);

    emulator::lsr(&cpu);

    EXPECT_EQ(cpu.accumulator(), default_acc >> 1);
}

TEST_F(TestCPUInstructions, test_cmp)
{
    // op code CMP - immediate
    cpu.write8(default_pc, 0xC9);
    cpu.write8(default_pc + 1, default_acc);

    emulator::cmp(&cpu);

    EXPECT_TRUE(cpu.status() & emulator::zero);
}

TEST_F(TestCPUInstructions, test_cpx)
{
    // op code CPX - immediate
    cpu.write8(default_pc, 0xE0);
    cpu.write8(default_pc + 1, default_x);

    emulator::cpx(&cpu);

    EXPECT_TRUE(cpu.status() & emulator::zero);
}

TEST_F(TestCPUInstructions, test_cpy)
{
    // op code CPY - immediate
    cpu.write8(default_pc, 0xC0);
    cpu.write8(default_pc + 1, default_y);

    emulator::cpy(&cpu);

    EXPECT_TRUE(cpu.status() & emulator::zero);
}

TEST_F(TestCPUInstructions, test_inc)
{
    // op code INC - absolute
    cpu.write8(default_pc, 0xEE);
    cpu.write8(default_pc + 1, default_pc + 3);
    cpu.write8(default_pc + 3, default_acc);

    emulator::inc(&cpu);

    EXPECT_EQ(cpu.read8(default_pc + 3), default_acc + 1);
}

TEST_F(TestCPUInstructions, test_inx)
{
    emulator::inx(&cpu);

    EXPECT_EQ(cpu.x_register(), default_x + 1);
}

TEST_F(TestCPUInstructions, test_iny)
{
    emulator::iny(&cpu);

    EXPECT_EQ(cpu.y_register(), default_y + 1);
}

TEST_F(TestCPUInstructions, test_jmp)
{
    // op code JMP - absolute
    cpu.write8(default_pc, 0x4C);
    cpu.write8(default_pc + 1, default_pc + 3);

    emulator::jmp(&cpu);

    EXPECT_EQ(cpu.program_counter(), default_pc + 3);
}

TEST_F(TestCPUInstructions, test_bcc)
{
    using namespace ::testing;

    EXPECT_CALL(cpu, add_branch_cycle(_))
        .Times(1);

    emulator::bcc(&cpu);
}

TEST_F(TestCPUInstructions, test_bcs)
{
    using namespace ::testing;

    cpu.add_flags(emulator::carry);

    EXPECT_CALL(cpu, add_branch_cycle(_))
        .Times(1);

    emulator::bcs(&cpu);
}

TEST_F(TestCPUInstructions, test_beq)
{
    using namespace ::testing;

    cpu.add_flags(emulator::zero);

    EXPECT_CALL(cpu, add_branch_cycle(_))
        .Times(1);

    emulator::beq(&cpu);
}

TEST_F(TestCPUInstructions, test_bmi)
{
    using namespace ::testing;

    cpu.add_flags(emulator::sign);

    EXPECT_CALL(cpu, add_branch_cycle(_))
        .Times(1);

    emulator::bmi(&cpu);
}

TEST_F(TestCPUInstructions, test_bne)
{
    using namespace ::testing;

    EXPECT_CALL(cpu, add_branch_cycle(_))
        .Times(1);

    emulator::bne(&cpu);
}

TEST_F(TestCPUInstructions, test_bpl)
{
    using namespace ::testing;

    EXPECT_CALL(cpu, add_branch_cycle(_))
        .Times(1);

    emulator::bpl(&cpu);
}

TEST_F(TestCPUInstructions, test_bvc)
{
    using namespace ::testing;

    EXPECT_CALL(cpu, add_branch_cycle(_))
        .Times(1);

    emulator::bvc(&cpu);
}

TEST_F(TestCPUInstructions, test_bvs)
{
    using namespace ::testing;

    cpu.add_flags(emulator::overflow);

    EXPECT_CALL(cpu, add_branch_cycle(_))
        .Times(1);

    emulator::bvs(&cpu);
}

TEST_F(TestCPUInstructions, test_sbc)
{
    // op code SBC - immediate 
    cpu.write8(default_pc, 0xE9);
    cpu.write8(default_pc + 1, default_acc - 1);

    emulator::sbc(&cpu);

    EXPECT_EQ(cpu.accumulator(), default_acc - default_acc + 1);
}

TEST_F(TestCPUInstructions, test_sbc_carry)
{
    // op code SBC - immediate 
    cpu.write8(default_pc, 0xE9);
    cpu.write8(default_pc + 1, default_acc - 1);

    cpu.add_flags(emulator::carry);

    emulator::sbc(&cpu);

    // Carry will add an extra -1
    EXPECT_EQ(cpu.accumulator(), default_acc - default_acc);
}

TEST_F(TestCPUInstructions, test_bit)
{
    // op code SBC - immediate
    cpu.write8(default_pc, 0x2C);
    cpu.write8(default_pc + 1, default_pc + 3);
    cpu.write8(default_pc + 3, 0xFF);

    emulator::bit(&cpu);

    EXPECT_TRUE(cpu.status() & emulator::sign);
    EXPECT_TRUE(cpu.status() & emulator::overflow);
}

TEST_F(TestCPUInstructions, test_bit_zero)
{
    // op code SBC - immediate
    cpu.write8(default_pc, 0x2C);
    cpu.write8(default_pc + 1, default_pc + 3);
    cpu.write8(default_pc + 3, 0x0);

    emulator::bit(&cpu);

    EXPECT_TRUE(cpu.status() & emulator::zero);
}

TEST_F(TestCPUInstructions, test_dec)
{
    // op code DEC - absolute
    cpu.write8(default_pc, 0xCE);
    cpu.write8(default_pc + 1, default_pc + 3);
    cpu.write8(default_pc + 3, default_acc);

    emulator::dec(&cpu);

    EXPECT_EQ(cpu.read8(default_pc + 3), default_acc - 1);
}

TEST_F(TestCPUInstructions, test_eor)
{
    // op code EOR - immediate
    cpu.write8(default_pc, 0x49);
    cpu.write8(default_pc + 1, default_acc);

    emulator::eor(&cpu);

    EXPECT_EQ(cpu.accumulator(), 0x0);
}

TEST_F(TestCPUInstructions, test_dex)
{
    emulator::dex(&cpu);

    EXPECT_EQ(cpu.x_register(), default_x - 1);
}

TEST_F(TestCPUInstructions, test_dey)
{
    emulator::dey(&cpu);

    EXPECT_EQ(cpu.y_register(), default_y - 1);
}

TEST_F(TestCPUInstructions, test_cpu_step_adc_immediate)
{
    // Write to the first spot op code ADC - immediate
    cpu.write8(default_pc, 0x69);
    // Write 5 to the argument for ADC
    cpu.write8(default_pc + 1, 0x05);

    cpu.step();

    EXPECT_EQ(cpu.accumulator(), 0x5 + default_acc);
}
