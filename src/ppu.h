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


/*

The CPU will create this with a list of 8 address pointers



*/

#ifndef NES_EMULATOR_PPU_H_
#define NES_EMULATOR_PPU_H_

#include "memory.h"

#include <functional>

namespace emulator
{

enum ControlFlag : uint8_t
{
    nametable          = 1 << 0 | 1 << 1, // first two bits
    increment          = 1 << 2,
    sprite_pattern     = 1 << 3,
    background_pattern = 1 << 4,
    sprite_size        = 1 << 5,
    master_slave       = 1 << 6,
    nmi_on_vblank      = 1 << 7
};

enum MaskFlag : uint8_t
{
    greyscale            = 1 << 0,
    show_left_background = 1 << 1,
    show_left_sprite     = 1 << 2,
    show_background      = 1 << 3,
    show_sprite          = 1 << 4,
    emphasize_red        = 1 << 5,
    emphasize_green      = 1 << 6,
    emphasize_blue       = 1 << 7
};

class PPU
{
public:
    void write_register(uint16_t address, uint8_t value);

    uint8_t read_register(uint16_t address) const;

    void set_non_maskable_interrupt_handler(std::function<void()> const& nmi_handler);

    void step();

private:
    // 0x2000 PPUCTRL
    void write_ctrl(uint8_t value);

    // 0x2001 PPUMASK
    void write_mask(uint8_t value);

    // 0x2002 PPUSTATUS
    uint8_t read_status() const;

    uint8_t nametable() const;
    uint8_t increment() const;
    uint8_t sprite_pattern() const;
    uint8_t background_pattern() const;
    uint8_t sprite_size() const;
    uint8_t master_slave() const;
    uint8_t nmi_on_vblank() const;

    uint8_t greyscale() const;
    uint8_t show_left_background() const;
    uint8_t show_left_sprite() const;
    uint8_t show_background() const;
    uint8_t show_sprite() const;
    uint8_t emphasize_red() const;
    uint8_t emphasize_green() const;
    uint8_t emphasize_blue() const;

    uint8_t control_flags{0};
    uint8_t mask_flags{0};

    uint8_t last_written_value{0};

    uint16_t vram{0};
    uint16_t temp_vram{0};
    uint8_t fine_x_scroll{0};
    uint8_t write_toggle{0};

    std::function<void()> non_maskable_interrupt_handler;

    // 10KB of memory
    Memory<163840> memory;
    // 256B of Object Attribute Memory
    Memory<256> oam;
};

}

#endif /* NES_EMULATOR_PPU_H_ */
