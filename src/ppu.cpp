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

240 lines of pixels
10kb of memory
  8kb ROM
  2kb RAM

Registers:

http://wiki.nesdev.com/w/index.php/PPU_registers

    Name        Address  Bits     Nots
--------------------------------------------
    PPUCTRL   : 0x2000 : VPHP SINN :  Quite a bit see site
    PPUMASK   : 0x2001 : BGRs bMmG :     ^          ^
    PPUSTATUS : 0x2002 : VS0- ---- :     ^          ^
    OAMADDR   : 0x2003 : aaaa aaaa : OAM read/write address
    OAMDATA   : 0x2004 : dddd dddd : OAM data read/write
    PPUSCROLL : 0x2005 : xxxx xxxx : Fine scroll position (two writes: X, Y)
    PPUADDR   : 0x2006 : aaaa aaaa : PPU read/write address (two writes: MSB, LSB)
    PPUDATA   : 0x2007 : dddd dddd : PPU data read/write
    OAMDMA    : 0x4014 : aaaa aaaa : OAM DMA high address

OAMADDR OAM address 0x2003 write

OAMDATA OAM data 0x2004 read/write

PPUSCROLL Scroll 0x2005 writex2?

PPUADDR 0x2006 writex2?

PPUDATA 0x2007 read/write

OAMDMA 0x4014 write
*/

/*
   http://wiki.nesdev.com/w/index.php/PPU_memory_map

    0x3FFF ppu addressable space

    Address range     Size     Description
    ------------------------------------
    0x0000 - 0x0FFF : 0x1000 : Pattern table 0
    0x1000 - 0x1FFF : 0x1000 : Pattern table 1
    0x2000 - 0x23FF : 0x0400 : Nametable 0
    0x2400 - 0x27FF : 0x0400 : Nametable 1
    0x2800 - 0x2BFF : 0x0400 : Nametable 2
    0x2C00 - 0x2FFF : 0x0400 : Nametable 3
    0x3000 - 0x3EFF : 0x0F00 : Mirrors of 0x2000 - 0x2EFF
    0x3F00 - 0x3F1F : 0x0020 : Palette RAM indexes
    0x3F20 - 0x3FFF : 0x00E0 : Mirros of 0x3F00 - 0x3F1F

    The PPU has an additional 256 bytes of memory known as Object Attriubte Memory

    Address range         Size     Description
    ----------------------------------
    0x00 - 0x0C (0 of 4) : 0x40 : Sprite Y coordinate
    0x01 - 0x0D (1 of 4) : 0x40 : Sprite tile #
    0x02 - 0x0E (2 of 4) : 0x40 : Sprite attribute
    0x03 - 0x0F (3 of 4) : 0x40 : Sprite X coordinate
*/

#include "ppu.h"

namespace
{
uint8_t get_flag_value(uint8_t flag, uint8_t bits)
{
    return flag & bits;
}
}

void emulator::PPU::set_non_maskable_interrupt_handler(std::function<void()> const& nmi_handler_func)
{
    non_maskable_interrupt_handler = nmi_handler_func;
}

void emulator::PPU::write_register(uint16_t address, uint8_t value)
{
    last_written_value = value;
    switch (address)
    {
        case 0x2000:
            write_ctrl(value);
            break;
        default:
            throw std::runtime_error("Invalid ppu address " +
                std::to_string(address) + " of value " +
                std::to_string(value));
    }
}

uint8_t emulator::PPU::read_register(uint16_t address) const
{
    switch (address)
    {
        case 0x2002:
            return read_status();
        default:
            throw std::runtime_error("Invalid ppu address ");
    }
}

/*
 * PPUCTRL 0x2000 write
 * 7 V Generate an NMI at the start of time (0: off, 1: on)
 * 6 P PPU master/slave select (0: read backdrop from EXT pins, 1: output color on EXT pins
 * 5 H Sprite size (0: 8x8, 1: 8x16)
 * 4 B Background patter table address (0: 0x000, 1: 0x1000)
 * 3 S Sprite pattern table address for 8x8 sprite (0: 0x0000, 1:0x1000, ignored in 8x16)
 * 2 I VRAM address increment per CPU read/write of PPUDATA
 * 1 N | Base nametable address
 * 0 N | (0: 0x2000, 1: 0x2400, 2: 0x2800, 3: 0x2C00)
 */
void emulator::PPU::write_ctrl(uint8_t value)
{
    control_flags = value;
}

uint8_t emulator::PPU::nametable() const
{
    return get_flag_value(control_flags, ControlFlag::nametable);
}

uint8_t emulator::PPU::increment() const
{
    return get_flag_value(control_flags, ControlFlag::increment);
}

uint8_t emulator::PPU::sprite_pattern() const
{
    return get_flag_value(control_flags, ControlFlag::sprite_pattern);
}

uint8_t emulator::PPU::background_pattern() const
{
    return get_flag_value(control_flags, ControlFlag::background_pattern);
}

uint8_t emulator::PPU::sprite_size() const
{
    return get_flag_value(control_flags, ControlFlag::sprite_size);
}

uint8_t emulator::PPU::master_slave() const
{
    return get_flag_value(control_flags, ControlFlag::master_slave);
}

uint8_t emulator::PPU::nmi_on_vblank() const
{
    return get_flag_value(control_flags, ControlFlag::nmi_on_vblank);
}

/*
 * PPUMASK 0x2001 write
 * 7 B Emphasize blue
 * 6 G Emphasize green
 * 5 R Emphasize red
 * 4 s 1: Show sprites
 * 3 b 1: Show background
 * 2 M 1: Show sprites in leftmost 8 pixel of screen, 0: hide
 * 1 m 1: Show background in leftmost 8 pixels of screen, 0: hide
 * 0 G Greyscale (0: normal color, 1: produce a greyscale display)
 *
 * Greyscale, bitwise AND 0x30 on any read from 0x3F00 - 0x3FFF
 */
void emulator::PPU::write_mask(uint8_t value)
{
    mask_flags = value;
}

uint8_t emulator::PPU::greyscale() const
{
    return get_flag_value(mask_flags, MaskFlag::greyscale);
}

uint8_t emulator::PPU::show_left_background() const
{
    return get_flag_value(control_flags, MaskFlag::show_left_background);
}

uint8_t emulator::PPU::show_left_sprite() const
{
    return get_flag_value(control_flags, MaskFlag::show_left_sprite);
}

uint8_t emulator::PPU::show_background() const
{
    return get_flag_value(control_flags, MaskFlag::show_background);
}

uint8_t emulator::PPU::show_sprite() const
{
    return get_flag_value(control_flags, MaskFlag::show_sprite);
}

uint8_t emulator::PPU::emphasize_red() const
{
    return get_flag_value(control_flags, MaskFlag::emphasize_red);
}

uint8_t emulator::PPU::emphasize_green() const
{
    return get_flag_value(control_flags, MaskFlag::emphasize_green);
}

uint8_t emulator::PPU::emphasize_blue() const
{
    return get_flag_value(control_flags, MaskFlag::emphasize_blue);
}

/*
 * PPUSTATUS 0x2002 read
 * 7 V VB has started (0: no in vblank, 1: in vblank)
 * 6 S Sprite 0 Hit. read more on page
 * 5 O Sprite overflow. read more on page
 * 4 - | Least significant bits previously wirtten into PPU register
 * 3 - | (due to registers not being updated for this address)
 * 2 - |
 * 1 - |
 * 0 - |
 */
uint8_t emulator::PPU::read_status() const
{
    uint8_t status;

    // Least sig bits of last written value into the ppu
    status = last_written_value & 0x1F;

    // TODO
    //status |= overflow << 5;
    //status |= zero_hit << 6;
    //status |= vblank_started << 7;

    return status;
}

void emulator::PPU::step()
{
    // nmi inturrupt
    if (non_maskable_interrupt_handler)
    {
        non_maskable_interrupt_handler();
    }
}
