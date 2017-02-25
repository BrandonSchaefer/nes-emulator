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

#include "memory.h"

namespace
{
uint8_t const default_value{0xAB};
}

struct TestMemory : public ::testing::Test
{
    emulator::Memory<0xFF> memory;
};

TEST_F(TestMemory, write8_read8)
{
    memory.write8(0x0, default_value);
    EXPECT_EQ(static_cast<uint16_t>(memory.read8(0x0)), default_value);
}

TEST_F(TestMemory, write16_read8)
{
    memory.write16(0x0, default_value | default_value << 8);
    EXPECT_EQ(static_cast<uint16_t>(memory.read8(0x0)), default_value);
}

TEST_F(TestMemory, write8_read16)
{
    memory.write8(0x0, 0x0);
    memory.write8(0x1, default_value);

    // High byte is in 0x1
    EXPECT_EQ(memory.read16(0x0), default_value << 8);
}

TEST_F(TestMemory, write16_read16)
{
    memory.write16(0x0, default_value << 8);

    EXPECT_EQ(memory.read16(0x0), default_value << 8);
}
