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

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "cpu.h"
#include "mocks/ppu.h"

namespace
{
struct TestCPU : ::testing::Test
{
    TestCPU() :
        cpu(&ppu)
    {
    }

    MockPPU ppu;
    emulator::CPU cpu;
};
}

TEST_F(TestCPU, test_read8_write8)
{
    cpu.write8(0x0, 0x10);
    EXPECT_EQ(cpu.read8(0x0), 0x10);
}

TEST_F(TestCPU, test_read16_write8)
{
    cpu.write8(0x0, 0x10);
    cpu.write8(0x1, 0x05);
    EXPECT_EQ(cpu.read16(0x0), 0x10 | 0x05 << 8);
}

TEST_F(TestCPU, test_set_get_program_counter)
{
    cpu.set_program_counter(0x20);
    EXPECT_EQ(cpu.program_counter(), 0x20);
}

TEST_F(TestCPU, test_set_get_x_register)
{
    cpu.set_x_register(0x20);
    EXPECT_EQ(cpu.x_register(), 0x20);
}

TEST_F(TestCPU, test_set_get_y_register)
{
    cpu.set_y_register(0x20);
    EXPECT_EQ(cpu.y_register(), 0x20);
}

TEST_F(TestCPU, test_set_get_stack)
{
    cpu.set_stack(0x20);
    EXPECT_EQ(cpu.stack(), 0x20);
}

TEST_F(TestCPU, test_update_get_status)
{
    EXPECT_FALSE(cpu.status() & emulator::carry);
    cpu.update_flags([] { return true; }, emulator::carry);
    EXPECT_TRUE(cpu.status() & emulator::carry);
}

TEST_F(TestCPU, test_update_get_multiple_status)
{
    EXPECT_FALSE(cpu.status() & emulator::carry);
    EXPECT_FALSE(cpu.status() & emulator::overflow);
    cpu.update_flags([] { return true; }, emulator::carry | emulator::overflow);
    EXPECT_TRUE(cpu.status() & emulator::carry);
    EXPECT_TRUE(cpu.status() & emulator::overflow);
}

TEST_F(TestCPU, test_add_get_status)
{
    EXPECT_FALSE(cpu.status() & emulator::carry);
    cpu.add_flags(emulator::carry);
    EXPECT_TRUE(cpu.status() & emulator::carry);
}

TEST_F(TestCPU, test_add_multiple_status)
{
    EXPECT_FALSE(cpu.status() & emulator::carry);
    EXPECT_FALSE(cpu.status() & emulator::overflow);
    cpu.add_flags(emulator::carry | emulator::overflow);
    EXPECT_TRUE(cpu.status() & emulator::carry);
    EXPECT_TRUE(cpu.status() & emulator::overflow);
}

TEST_F(TestCPU, test_add_remove_status)
{
    EXPECT_FALSE(cpu.status() & emulator::carry);
    cpu.add_flags(emulator::carry);
    EXPECT_TRUE(cpu.status() & emulator::carry);

    cpu.remove_flags(emulator::carry);
    EXPECT_FALSE(cpu.status() & emulator::carry);
}

TEST_F(TestCPU, test_add_remove_multiple_status)
{
    EXPECT_FALSE(cpu.status() & emulator::carry);
    EXPECT_FALSE(cpu.status() & emulator::overflow);
    cpu.add_flags(emulator::carry | emulator::overflow);
    EXPECT_TRUE(cpu.status() & emulator::carry);
    EXPECT_TRUE(cpu.status() & emulator::overflow);

    cpu.remove_flags(emulator::carry | emulator::overflow);
    EXPECT_FALSE(cpu.status() & emulator::carry);
    EXPECT_FALSE(cpu.status() & emulator::overflow);
}

TEST_F(TestCPU, test_push_pop)
{
    cpu.push(0x1);
    EXPECT_EQ(cpu.pop(), 0x1);
}
