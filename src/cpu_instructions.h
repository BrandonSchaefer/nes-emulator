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

#ifndef NES_EMULATOR_CPU_INSTRUCTIONS_H_
#define NES_EMULATOR_CPU_INSTRUCTIONS_H_

#include <array>
#include <cstdint>
#include <functional>

namespace emulator
{
class CPU;

namespace
{
uint8_t const zero_page_cycles{0};
uint8_t const one_page_cylce{1};
}

// http://wiki.nesdev.com/w/index.php/CPU_addressing_modes
enum OpMode : uint8_t
{
    zero_page_x,
    zero_page_y,
    absolute_x,
    absolute_y,
    indexed_x,
    indexed_y,
    implicit,
    accumulator,
    immediate,
    zero_page,
    absolute,
    relative,
    indirect
};

struct OpInfo
{
    std::string name;
    OpMode mode;
    uint8_t number_bytes;
    uint8_t number_cycles;
    uint8_t add_cycles_page_crossed;
    std::function<void(CPU* cpu)> func;
};

extern void adc(CPU* cpu);
extern void ahx(CPU* cpu);
extern void alr(CPU* cpu);
extern void anc(CPU* cpu);
extern void nd(CPU* cpu);
extern void arr(CPU* cpu);
extern void asl(CPU* cpu);
extern void axs(CPU* cpu);
extern void bcc(CPU* cpu);
extern void bcs(CPU* cpu);
extern void beq(CPU* cpu);
extern void bit(CPU* cpu);
extern void bmi(CPU* cpu);
extern void bne(CPU* cpu);
extern void bpl(CPU* cpu);
extern void brk(CPU* cpu);
extern void bvc(CPU* cpu);
extern void bvs(CPU* cpu);
extern void clc(CPU* cpu);
extern void cld(CPU* cpu);
extern void cli(CPU* cpu);
extern void clv(CPU* cpu);
extern void cmp(CPU* cpu);
extern void cpx(CPU* cpu);
extern void cpy(CPU* cpu);
extern void dcp(CPU* cpu);
extern void dec(CPU* cpu);
extern void dex(CPU* cpu);
extern void dey(CPU* cpu);
extern void eor(CPU* cpu);
extern void kil(CPU* cpu);
extern void inc(CPU* cpu);
extern void inx(CPU* cpu);
extern void iny(CPU* cpu);
extern void isc(CPU* cpu);
extern void jmp(CPU* cpu);
extern void jsr(CPU* cpu);
extern void las(CPU* cpu);
extern void lax(CPU* cpu);
extern void lda(CPU* cpu);
extern void ldx(CPU* cpu);
extern void ldy(CPU* cpu);
extern void lsr(CPU* cpu);
extern void nop(CPU* cpu);
extern void ora(CPU* cpu);
extern void pha(CPU* cpu);
extern void php(CPU* cpu);
extern void pla(CPU* cpu);
extern void plp(CPU* cpu);
extern void rla(CPU* cpu);
extern void rol(CPU* cpu);
extern void ror(CPU* cpu);
extern void rra(CPU* cpu);
extern void rti(CPU* cpu);
extern void rts(CPU* cpu);
extern void sax(CPU* cpu);
extern void sbc(CPU* cpu);
extern void sec(CPU* cpu);
extern void sed(CPU* cpu);
extern void sei(CPU* cpu);
extern void shx(CPU* cpu);
extern void shy(CPU* cpu);
extern void slo(CPU* cpu);
extern void sre(CPU* cpu);
extern void sta(CPU* cpu);
extern void stx(CPU* cpu);
extern void sty(CPU* cpu);
extern void tas(CPU* cpu);
extern void tax(CPU* cpu);
extern void tay(CPU* cpu);
extern void tsx(CPU* cpu);
extern void txa(CPU* cpu);
extern void txs(CPU* cpu);
extern void tya(CPU* cpu);
extern void xaa(CPU* cpu);

// http://www.oxyron.de/html/opcodes02.html
static std::array<OpInfo, 256> const instruction{{
/*         | name |  mode   | number of| number of |add cycle if  |     Function
           |      |         |  bytes   |  cycles   | page crossed |              */
/* 0x00 */ {"BRK", implicit,     1,         7,       zero_page_cycles,    brk},
/* 0x01 */ {"ORA", indexed_x,    2,         6,       zero_page_cycles,    ora},
/* 0x02 */ {"KIL", implicit,     0,         2,       zero_page_cycles,    kil},
/* 0x03 */ {"SLO", indexed_x,    0,         8,       zero_page_cycles,    slo},
/* 0x04 */ {"NOP", zero_page,    2,         3,       zero_page_cycles,    nop},
/* 0x05 */ {"ORA", zero_page,    2,         3,       zero_page_cycles,    ora},
/* 0x06 */ {"ASL", zero_page,    2,         5,       zero_page_cycles,    asl},
/* 0x07 */ {"SLO", zero_page,    0,         5,       zero_page_cycles,    slo},
/* 0x08 */ {"PHP", implicit,     1,         3,       zero_page_cycles,    php},
/* 0x09 */ {"ORA", immediate,    2,         2,       zero_page_cycles,    ora},
/* 0x0A */ {"ASL", accumulator,  1,         2,       zero_page_cycles,    asl},
/* 0x0B */ {"ANC", immediate,    0,         2,       zero_page_cycles,    anc},
/* 0x0C */ {"NOP", absolute,     3,         4,       zero_page_cycles,    nop},
/* 0x0D */ {"ORA", absolute,     3,         4,       zero_page_cycles,    ora},
/* 0x0E */ {"ASL", absolute,     3,         6,       zero_page_cycles,    asl},
/* 0x0F */ {"SLO", absolute,     0,         6,       zero_page_cycles,    slo},
/* 0x10 */ {"BPL", relative,     2,         2,       one_page_cylce,      bpl},
/* 0x11 */ {"ORA", indexed_y,    2,         5,       one_page_cylce,      ora},
/* 0x12 */ {"KIL", implicit,     0,         2,       zero_page_cycles,    kil},
/* 0x13 */ {"SLO", indexed_y,    0,         8,       zero_page_cycles,    slo},
/* 0x14 */ {"NOP", zero_page_x,  2,         4,       zero_page_cycles,    nop},
/* 0x15 */ {"ORA", zero_page_x,  2,         4,       zero_page_cycles,    ora},
/* 0x16 */ {"ASL", zero_page_x,  2,         6,       zero_page_cycles,    asl},
/* 0x17 */ {"SLO", zero_page_x,  0,         6,       zero_page_cycles,    slo},
/* 0x18 */ {"CLC", implicit,     1,         2,       zero_page_cycles,    clc},
/* 0x19 */ {"ORA", absolute_y,   3,         4,       one_page_cylce,      ora},
/* 0x1A */ {"NOP", implicit,     1,         2,       zero_page_cycles,    nop},
/* 0x1B */ {"SLO", absolute_y,   0,         7,       zero_page_cycles,    slo},
/* 0x1C */ {"NOP", absolute_x,   3,         4,       one_page_cylce,      nop},
/* 0x1D */ {"ORA", absolute_x,   3,         4,       one_page_cylce,      ora},
/* 0x1E */ {"ASL", absolute_x,   3,         7,       zero_page_cycles,    asl},
/* 0x1F */ {"SLO", absolute_x,   0,         7,       zero_page_cycles,    slo},
/* 0x20 */ {"JSR", absolute,     3,         6,       zero_page_cycles,    jsr},
/* 0x21 */ {"AND", indexed_x,    2,         6,       zero_page_cycles,    nd},
/* 0x22 */ {"KIL", implicit,     0,         2,       zero_page_cycles,    kil},
/* 0x23 */ {"RLA", indexed_x,    0,         8,       zero_page_cycles,    rla},
/* 0x24 */ {"BIT", zero_page,    2,         3,       zero_page_cycles,    bit},
/* 0x25 */ {"AND", zero_page,    2,         3,       zero_page_cycles,    nd},
/* 0x26 */ {"ROL", zero_page,    2,         5,       zero_page_cycles,    rol},
/* 0x27 */ {"RLA", zero_page,    0,         5,       zero_page_cycles,    rla},
/* 0x28 */ {"PLP", implicit,     1,         4,       zero_page_cycles,    plp},
/* 0x29 */ {"AND", immediate,    2,         2,       zero_page_cycles,    nd},
/* 0x2A */ {"ROL", accumulator,  1,         2,       zero_page_cycles,    rol},
/* 0x2B */ {"ANC", immediate,    0,         2,       zero_page_cycles,    anc},
/* 0x2C */ {"BIT", absolute,     3,         4,       zero_page_cycles,    bit},
/* 0x2D */ {"AND", absolute,     3,         4,       zero_page_cycles,    nd},
/* 0x2E */ {"ROL", absolute,     3,         6,       zero_page_cycles,    rol},
/* 0x2F */ {"RLA", absolute,     0,         6,       zero_page_cycles,    rla},
/* 0x30 */ {"BMI", relative,     2,         2,       one_page_cylce,      bmi},
/* 0x31 */ {"AND", indexed_y,    2,         5,       one_page_cylce,      nd},
/* 0x32 */ {"KIL", implicit,     0,         2,       zero_page_cycles,    kil},
/* 0x33 */ {"RLA", indexed_y,    0,         8,       zero_page_cycles,    rla},
/* 0x34 */ {"NOP", zero_page_x,  2,         4,       zero_page_cycles,    nop},
/* 0x35 */ {"AND", zero_page_x,  2,         4,       zero_page_cycles,    nd},
/* 0x36 */ {"ROL", zero_page_x,  2,         6,       zero_page_cycles,    rol},
/* 0x37 */ {"RLA", zero_page_x,  0,         6,       zero_page_cycles,    rla},
/* 0x38 */ {"SEC", implicit,     1,         2,       zero_page_cycles,    sec},
/* 0x39 */ {"AND", absolute_y,   3,         4,       one_page_cylce,      nd},
/* 0x3A */ {"NOP", implicit,     1,         2,       zero_page_cycles,    nop},
/* 0x3B */ {"RLA", absolute_y,   0,         7,       zero_page_cycles,    rla},
/* 0x3C */ {"NOP", absolute_x,   3,         4,       one_page_cylce,      nop},
/* 0x3D */ {"AND", absolute_x,   3,         4,       one_page_cylce,      nd},
/* 0x3E */ {"ROL", absolute_x,   3,         7,       zero_page_cycles,    rol},
/* 0x3F */ {"RLA", absolute_x,   0,         7,       zero_page_cycles,    rla},
/* 0x40 */ {"RTI", implicit,     1,         6,       zero_page_cycles,    rti},
/* 0x41 */ {"EOR", indexed_x,    2,         6,       zero_page_cycles,    eor},
/* 0x42 */ {"KIL", implicit,     0,         2,       zero_page_cycles,    kil},
/* 0x43 */ {"SRE", indexed_x,    0,         8,       zero_page_cycles,    sre},
/* 0x44 */ {"NOP", zero_page,    2,         3,       zero_page_cycles,    nop},
/* 0x45 */ {"EOR", zero_page,    2,         3,       zero_page_cycles,    nop},
/* 0x46 */ {"LSR", zero_page,    2,         5,       zero_page_cycles,    lsr},
/* 0x47 */ {"SRE", zero_page,    0,         5,       zero_page_cycles,    sre},
/* 0x48 */ {"PHA", implicit,     1,         3,       zero_page_cycles,    pha},
/* 0x49 */ {"EOR", immediate,    2,         2,       zero_page_cycles,    eor},
/* 0x4A */ {"LSR", accumulator,  1,         2,       zero_page_cycles,    lsr},
/* 0x4B */ {"ALR", immediate,    0,         2,       zero_page_cycles,    alr},
/* 0x4C */ {"JMP", absolute,     3,         3,       zero_page_cycles,    jmp},
/* 0x4D */ {"EOR", absolute,     3,         4,       zero_page_cycles,    eor},
/* 0x4E */ {"LSR", absolute,     3,         6,       zero_page_cycles,    lsr},
/* 0x4F */ {"SRE", absolute,     0,         6,       zero_page_cycles,    sre},
/* 0x50 */ {"BVC", relative,     2,         2,       one_page_cylce,      bvc},
/* 0x51 */ {"EOR", indexed_y,    2,         5,       one_page_cylce,      eor},
/* 0x52 */ {"KIL", implicit,     0,         2,       zero_page_cycles,    kil},
/* 0x53 */ {"SRE", indexed_y,    0,         8,       zero_page_cycles,    sre},
/* 0x54 */ {"NOP", zero_page_x,  2,         4,       zero_page_cycles,    nop},
/* 0x55 */ {"EOR", zero_page_x,  2,         4,       zero_page_cycles,    eor},
/* 0x56 */ {"LSR", zero_page_x,  2,         6,       zero_page_cycles,    lsr},
/* 0x57 */ {"SRE", zero_page_x,  0,         6,       zero_page_cycles,    sre},
/* 0x58 */ {"CLI", implicit,     1,         2,       zero_page_cycles,    cli},
/* 0x59 */ {"EOR", absolute_y,   3,         4,       one_page_cylce,      eor},
/* 0x5A */ {"NOP", implicit,     1,         2,       zero_page_cycles,    nop},
/* 0x5B */ {"SRE", absolute_y,   0,         7,       zero_page_cycles,    sre},
/* 0x5C */ {"NOP", absolute_x,   3,         4,       one_page_cylce,      nop},
/* 0x5D */ {"EOR", absolute_x,   3,         4,       one_page_cylce,      eor},
/* 0x5E */ {"LSR", absolute_x,   3,         7,       zero_page_cycles,    lsr},
/* 0x5F */ {"SRE", absolute_x,   0,         7,       zero_page_cycles,    sre},
/* 0x60 */ {"RTS", implicit,     1,         6,       zero_page_cycles,    rts},
/* 0x61 */ {"ADC", indexed_x,    2,         6,       zero_page_cycles,    adc},
/* 0x62 */ {"KIL", implicit,     0,         2,       zero_page_cycles,    kil},
/* 0x63 */ {"RRA", indexed_x,    0,         8,       zero_page_cycles,    rra},
/* 0x64 */ {"NOP", zero_page,    2,         3,       zero_page_cycles,    nop},
/* 0x65 */ {"ADC", zero_page,    2,         3,       zero_page_cycles,    adc},
/* 0x66 */ {"ROR", zero_page,    2,         5,       zero_page_cycles,    ror},
/* 0x67 */ {"RRA", zero_page,    0,         5,       zero_page_cycles,    rra},
/* 0x68 */ {"PLA", implicit,     1,         4,       zero_page_cycles,    pla},
/* 0x69 */ {"ADC", immediate,    2,         2,       zero_page_cycles,    adc},
/* 0x6A */ {"ROR", accumulator,  1,         2,       zero_page_cycles,    ror},
/* 0x6B */ {"ARR", immediate,    0,         2,       zero_page_cycles,    arr},
/* 0x6C */ {"JMP", indirect,     3,         5,       zero_page_cycles,    jmp},
/* 0x6D */ {"ADC", absolute,     3,         4,       zero_page_cycles,    adc},
/* 0x6E */ {"ROR", absolute,     3,         6,       zero_page_cycles,    ror},
/* 0x6F */ {"RRA", absolute,     0,         6,       zero_page_cycles,    rra},
/* 0x70 */ {"BVS", relative,     2,         2,       one_page_cylce,      bvs},
/* 0x71 */ {"ADC", indexed_y,    2,         5,       one_page_cylce,      adc},
/* 0x72 */ {"KIL", implicit,     0,         2,       zero_page_cycles,    kil},
/* 0x73 */ {"RRA", indexed_y,    0,         8,       zero_page_cycles,    rra},
/* 0x74 */ {"NOP", zero_page_x,  2,         4,       zero_page_cycles,    nop},
/* 0x75 */ {"ADC", zero_page_x,  2,         4,       zero_page_cycles,    adc},
/* 0x76 */ {"ROR", zero_page_x,  2,         6,       zero_page_cycles,    ror},
/* 0x77 */ {"RRA", zero_page_x,  0,         6,       zero_page_cycles,    rra},
/* 0x78 */ {"SEI", implicit,     1,         2,       zero_page_cycles,    sei},
/* 0x79 */ {"ADC", absolute_y,   3,         4,       one_page_cylce,      adc},
/* 0x7A */ {"NOP", implicit,     1,         2,       zero_page_cycles,    nop},
/* 0x7B */ {"RRA", absolute_y,   0,         7,       zero_page_cycles,    rra},
/* 0x7C */ {"NOP", absolute_x,   3,         4,       one_page_cylce,      nop},
/* 0x7D */ {"ADC", absolute_x,   3,         4,       one_page_cylce,      adc},
/* 0x7E */ {"ROR", absolute_x,   3,         7,       zero_page_cycles,    ror},
/* 0x7F */ {"RRA", absolute_x,   0,         7,       zero_page_cycles,    rra},
/* 0x80 */ {"NOP", immediate,    2,         2,       zero_page_cycles,    nop},
/* 0x81 */ {"STA", indexed_x,    2,         6,       zero_page_cycles,    sta},
/* 0x82 */ {"NOP", immediate,    0,         2,       zero_page_cycles,    nop},
/* 0x83 */ {"SAX", indexed_x,    0,         6,       zero_page_cycles,    sax},
/* 0x84 */ {"STY", zero_page,    2,         3,       zero_page_cycles,    sty},
/* 0x85 */ {"STA", zero_page,    2,         3,       zero_page_cycles,    sta},
/* 0x86 */ {"STX", zero_page,    2,         3,       zero_page_cycles,    stx},
/* 0x87 */ {"SAX", zero_page,    0,         3,       zero_page_cycles,    sax},
/* 0x88 */ {"DEY", implicit,     1,         2,       zero_page_cycles,    dey},
/* 0x89 */ {"NOP", immediate,    0,         2,       zero_page_cycles,    nop},
/* 0x8A */ {"TXA", implicit,     1,         2,       zero_page_cycles,    txa},
/* 0x8B */ {"XAA", immediate,    0,         2,       zero_page_cycles,    xaa},
/* 0x8C */ {"STY", absolute,     3,         4,       zero_page_cycles,    sty},
/* 0x8D */ {"STA", absolute,     3,         4,       zero_page_cycles,    sta},
/* 0x8E */ {"STX", absolute,     3,         4,       zero_page_cycles,    stx},
/* 0x8F */ {"SAX", absolute,     0,         4,       zero_page_cycles,    sax},
/* 0x90 */ {"BCC", relative,     2,         2,       one_page_cylce,      bcc},
/* 0x91 */ {"STA", indexed_y,    2,         6,       zero_page_cycles,    sta},
/* 0x92 */ {"KIL", implicit,     0,         2,       zero_page_cycles,    kil},
/* 0x93 */ {"AHX", indexed_y,    0,         6,       zero_page_cycles,    ahx},
/* 0x94 */ {"STY", zero_page_x,  2,         4,       zero_page_cycles,    sty},
/* 0x95 */ {"STA", zero_page_x,  2,         4,       zero_page_cycles,    sta},
/* 0x96 */ {"STX", zero_page_y,  2,         4,       zero_page_cycles,    stx},
/* 0x97 */ {"SAX", zero_page_y,  0,         4,       zero_page_cycles,    sax},
/* 0x98 */ {"TYA", implicit,     1,         2,       zero_page_cycles,    tya},
/* 0x99 */ {"STA", absolute_y,   3,         5,       zero_page_cycles,    sta},
/* 0x9A */ {"TXS", implicit,     1,         2,       zero_page_cycles,    txs},
/* 0x9B */ {"TAS", absolute_y,   0,         5,       zero_page_cycles,    tas},
/* 0x9C */ {"SHY", absolute_x,   0,         5,       zero_page_cycles,    shy},
/* 0x9D */ {"STA", absolute_x,   3,         5,       zero_page_cycles,    sta},
/* 0x9E */ {"SHX", zero_page_x,  0,         5,       zero_page_cycles,    shx},
/* 0x9F */ {"AHX", zero_page_x,  0,         5,       zero_page_cycles,    ahx},
/* 0xA0 */ {"LDY", immediate,    2,         2,       zero_page_cycles,    ldy},
/* 0xA1 */ {"LDA", indexed_x,    2,         6,       zero_page_cycles,    lda},
/* 0xA2 */ {"LDX", immediate,    2,         2,       zero_page_cycles,    ldx},
/* 0xA3 */ {"LAX", indexed_x,    0,         6,       zero_page_cycles,    lax},
/* 0xA4 */ {"LDY", zero_page,    2,         3,       zero_page_cycles,    ldy},
/* 0xA5 */ {"LDA", zero_page,    2,         3,       zero_page_cycles,    lda},
/* 0xA6 */ {"LDX", zero_page,    2,         3,       zero_page_cycles,    ldx},
/* 0xA7 */ {"LAX", zero_page,    0,         3,       zero_page_cycles,    lax},
/* 0xA8 */ {"TAY", implicit,     1,         2,       zero_page_cycles,    tay},
/* 0xA9 */ {"LDA", immediate,    2,         2,       zero_page_cycles,    lda},
/* 0xAA */ {"TAX", implicit,     1,         2,       zero_page_cycles,    tax},
/* 0xAB */ {"LAX", immediate,    0,         2,       zero_page_cycles,    lax},
/* 0xAC */ {"LDY", absolute,     3,         4,       zero_page_cycles,    ldy},
/* 0xAD */ {"LDA", absolute,     3,         4,       zero_page_cycles,    lda},
/* 0xAE */ {"LDX", absolute,     3,         4,       zero_page_cycles,    ldx},
/* 0xAF */ {"LAX", absolute,     0,         4,       zero_page_cycles,    lax},
/* 0xB0 */ {"BCS", relative,     2,         2,       one_page_cylce,      bcs},
/* 0xB1 */ {"LDA", indexed_y,    2,         5,       one_page_cylce,      lda},
/* 0xB2 */ {"KIL", implicit,     0,         2,       zero_page_cycles,    kil},
/* 0xB3 */ {"LAX", indexed_y,    0,         5,       one_page_cylce,      lax},
/* 0xB4 */ {"LDY", zero_page_x,  2,         4,       zero_page_cycles,    ldy},
/* 0xB5 */ {"LDA", zero_page_x,  2,         4,       zero_page_cycles,    lda},
/* 0xB6 */ {"LDX", zero_page_y,  2,         4,       zero_page_cycles,    ldx},
/* 0xB7 */ {"LAX", zero_page_y,  0,         4,       zero_page_cycles,    lax},
/* 0xB8 */ {"CLV", implicit,     1,         2,       zero_page_cycles,    clv},
/* 0xB9 */ {"LDA", absolute_y,   3,         4,       one_page_cylce,      lda},
/* 0xBA */ {"TSX", implicit,     1,         2,       zero_page_cycles,    tsx},
/* 0xBB */ {"LAS", absolute_y,   0,         4,       one_page_cylce,      las},
/* 0xBC */ {"LDY", absolute_x,   3,         4,       one_page_cylce,      ldy},
/* 0xBD */ {"LDA", absolute_x,   3,         4,       one_page_cylce,      lda},
/* 0xBE */ {"LDX", zero_page_x,  3,         4,       one_page_cylce,      ldx},
/* 0xBF */ {"LAX", zero_page_x,  0,         4,       one_page_cylce,      lax},
/* 0xC0 */ {"CPY", immediate,    2,         2,       zero_page_cycles,    cpy},
/* 0xC1 */ {"CMP", indexed_x,    2,         6,       zero_page_cycles,    cmp},
/* 0xC2 */ {"NOP", immediate,    0,         2,       zero_page_cycles,    nop},
/* 0xC3 */ {"DCP", indexed_x,    0,         8,       zero_page_cycles,    dcp},
/* 0xC4 */ {"CPY", zero_page,    2,         3,       zero_page_cycles,    cpy},
/* 0xC5 */ {"CMP", zero_page,    2,         3,       zero_page_cycles,    cmp},
/* 0xC6 */ {"DEC", zero_page,    2,         5,       zero_page_cycles,    dec},
/* 0xC7 */ {"DCP", zero_page,    0,         5,       zero_page_cycles,    dcp},
/* 0xC8 */ {"INY", implicit,     1,         2,       zero_page_cycles,    iny},
/* 0xC9 */ {"CMP", immediate,    2,         2,       zero_page_cycles,    cmp},
/* 0xCA */ {"DEX", implicit,     1,         2,       zero_page_cycles,    dex},
/* 0xCB */ {"AXS", immediate,    0,         2,       zero_page_cycles,    axs},
/* 0xCC */ {"CPY", absolute,     3,         4,       zero_page_cycles,    cpy},
/* 0xCD */ {"CMP", absolute,     3,         4,       zero_page_cycles,    cmp},
/* 0xCE */ {"DEC", absolute,     3,         6,       zero_page_cycles,    dec},
/* 0xCF */ {"DCP", absolute,     0,         6,       zero_page_cycles,    dcp},
/* 0xD0 */ {"BNE", relative,     2,         2,       one_page_cylce,      bne},
/* 0xD1 */ {"CMP", indexed_y,    2,         5,       one_page_cylce,      cmp},
/* 0xD2 */ {"KIL", implicit,     0,         2,       zero_page_cycles,    kil},
/* 0xD3 */ {"DCP", indexed_y,    0,         8,       zero_page_cycles,    dcp},
/* 0xD4 */ {"NOP", zero_page_x,  2,         4,       zero_page_cycles,    nop},
/* 0xD5 */ {"CMP", zero_page_x,  2,         4,       zero_page_cycles,    cmp},
/* 0xD6 */ {"DEC", zero_page_x,  2,         6,       zero_page_cycles,    dec},
/* 0xD7 */ {"DCP", zero_page_x,  0,         6,       zero_page_cycles,    dcp},
/* 0xD8 */ {"CLD", implicit,     1,         2,       zero_page_cycles,    cld},
/* 0xD9 */ {"CMP", absolute_y,   3,         4,       one_page_cylce,      cmp},
/* 0xDA */ {"NOP", implicit,     1,         2,       zero_page_cycles,    nop},
/* 0xDB */ {"DCP", absolute_y,   0,         7,       zero_page_cycles,    dcp},
/* 0xDC */ {"NOP", absolute_x,   3,         4,       one_page_cylce,      nop},
/* 0xDD */ {"CMP", absolute_x,   3,         4,       one_page_cylce,      cmp},
/* 0xDE */ {"DEC", absolute_x,   3,         7,       zero_page_cycles,    dec},
/* 0xDF */ {"DCP", absolute_x,   0,         7,       zero_page_cycles,    dcp},
/* 0xE0 */ {"CPX", immediate,    2,         2,       zero_page_cycles,    cpx},
/* 0xE1 */ {"SBC", indexed_x,    2,         6,       zero_page_cycles,    sbc},
/* 0xE2 */ {"NOP", immediate,    0,         2,       zero_page_cycles,    nop},
/* 0xE3 */ {"ISC", indexed_x,    0,         8,       zero_page_cycles,    isc},
/* 0xE4 */ {"CPX", zero_page,    2,         3,       zero_page_cycles,    cpx},
/* 0xE5 */ {"SBC", zero_page,    2,         3,       zero_page_cycles,    sbc},
/* 0xE6 */ {"INC", zero_page,    2,         5,       zero_page_cycles,    inc},
/* 0xE7 */ {"ISC", zero_page,    0,         5,       zero_page_cycles,    isc},
/* 0xE8 */ {"INX", implicit,     1,         2,       zero_page_cycles,    inx},
/* 0xE9 */ {"SBC", immediate,    2,         2,       zero_page_cycles,    sbc},
/* 0xEA */ {"NOP", implicit,     1,         2,       zero_page_cycles,    nop},
/* 0xEB */ {"SBC", immediate,    0,         2,       zero_page_cycles,    sbc},
/* 0xEC */ {"CPX", absolute,     3,         4,       zero_page_cycles,    cpx},
/* 0xED */ {"SBC", absolute,     3,         4,       zero_page_cycles,    sbc},
/* 0xEE */ {"INC", absolute,     3,         6,       zero_page_cycles,    inc},
/* 0xEF */ {"ISC", absolute,     0,         6,       zero_page_cycles,    isc},
/* 0xF0 */ {"BEQ", relative,     2,         2,       one_page_cylce,      beq},
/* 0xF1 */ {"SBC", indexed_y,    2,         5,       one_page_cylce,      sbc},
/* 0xF2 */ {"KIL", implicit,     0,         2,       zero_page_cycles,    kil},
/* 0xF3 */ {"ISC", indexed_y,    0,         8,       zero_page_cycles,    isc},
/* 0xF4 */ {"NOP", zero_page_x,  2,         4,       zero_page_cycles,    nop},
/* 0xF5 */ {"SBC", zero_page_x,  2,         4,       zero_page_cycles,    sbc},
/* 0xF6 */ {"INC", zero_page_x,  2,         6,       zero_page_cycles,    inc},
/* 0xF7 */ {"ISC", zero_page_x,  0,         6,       zero_page_cycles,    isc},
/* 0xF8 */ {"SED", implicit,     1,         2,       zero_page_cycles,    sed},
/* 0xF9 */ {"SBC", absolute_y,   3,         4,       one_page_cylce,      sbc},
/* 0xFA */ {"NOP", implicit,     1,         2,       zero_page_cycles,    nop},
/* 0xFB */ {"ISC", absolute_y,   0,         7,       zero_page_cycles,    isc},
/* 0xFC */ {"NOP", absolute_x,   3,         4,       one_page_cylce,      nop},
/* 0xFD */ {"SBC", absolute_x,   3,         4,       one_page_cylce,      sbc},
/* 0xFE */ {"INC", absolute_x,   3,         7,       zero_page_cycles,    inc},
/* 0xFF */ {"ISC", absolute_x,   0,         7,       zero_page_cycles,    isc}
}};

}

#endif /* NES_EMULATOR_CPU_INSTRUCTIONS_H_ */
