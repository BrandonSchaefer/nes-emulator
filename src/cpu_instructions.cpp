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


#include "cpu.h"
#include "cpu_instructions.h"

namespace
{
void update_zero(emulator::CPU* cpu, uint8_t value)
{
    cpu->update_flags([value] { return value == 0; }, emulator::zero);
}

void update_sign(emulator::CPU* cpu, uint8_t value)
{
    cpu->update_flags([value] { return value & 0x80; }, emulator::sign);
}

void update_zero_sign(emulator::CPU* cpu, uint8_t value)
{
    update_zero(cpu, value);
    update_sign(cpu, value);
}

void update_overflow(emulator::CPU* cpu, uint8_t acc, uint8_t mem, uint8_t new_value)
{
    cpu->update_flags([acc, mem, new_value] {
        return !((acc ^ mem) & 0x80) && ((acc ^ new_value) & 0x80);
    }, emulator::overflow);
}

void update_carry(emulator::CPU* cpu, uint32_t value)
{
    cpu->update_flags([value] { return value > 0xFF; }, emulator::carry);
}

}

// ADC Add Memory to Accumulator with Carry
void emulator::adc(CPU* cpu)
{
    auto a    = cpu->accumulator();
    auto mem  = cpu->read8(cpu->address_to_arguemnts());
    uint8_t c = !!(cpu->status() & carry);
    uint8_t new_value = a + mem + c;

    update_zero(cpu, new_value);

    //if (cpu->status() & decimal)
    {
        // TODO BCD
    }
    //else
    {
        update_sign(cpu, new_value);
        update_overflow(cpu, a, mem, new_value);

        auto as_int32 =
            static_cast<int>(a) +
            static_cast<int>(mem) +
            static_cast<int>(c);

        update_carry(cpu, as_int32);
    }

    printf("Yay %i + %i + %i = %i\n", a, mem, c, new_value);

    cpu->set_accumulator(new_value);
}

// AND "AND" Memory with Accumulator
void emulator::nd(CPU* cpu)
{
    auto a = cpu->accumulator();
    auto mem = cpu->read8(cpu->address_to_arguemnts());
    auto new_value = a & mem;
    cpu->set_accumulator(new_value);

    update_zero_sign(cpu, new_value);
}

// ASL Shift Left One Bit (Memory or Accumulator)
void emulator::asl(CPU* cpu)
{
    auto current_mode = cpu->current_mode();
    uint8_t new_value = 0;

    if (current_mode == emulator::accumulator)
    {
        auto a = cpu->accumulator();
        new_value = a << 1;
        cpu->set_accumulator(new_value);
    }
    else
    {
        auto address = cpu->address_to_arguemnts();
        auto mem = cpu->read8(address);
        new_value = mem << 1;
        cpu->write8(address, new_value);
    }

    update_zero_sign(cpu, new_value);
}

// BCC Branch on Carry Clear
void emulator::bcc(CPU* cpu)
{
    if ((cpu->status() & emulator::carry) == 0)
    {
        auto address = cpu->address_to_arguemnts();
        // TODO Upate cycle?
        cpu->set_program_counter(address);
    }
}

// BCS Branch on Carry Set
void emulator::bcs(CPU* cpu)
{
    if (cpu->status() & emulator::carry)
    {
        auto address = cpu->address_to_arguemnts();
        // TODO Upate cycle?
        cpu->set_program_counter(address);
    }
}

// BEQ Branch on Result Zero
void emulator::beq(CPU* cpu)
{
    if (cpu->status() & emulator::zero)
    {
        auto address = cpu->address_to_arguemnts();
        // TODO Upate cycle?
        cpu->set_program_counter(address);
    }
}

// BIT Test Bits in Memory with Accumulator
void emulator::bit(CPU* cpu)
{
    auto address = cpu->address_to_arguemnts();
    auto mem = cpu->read8(address);

    cpu->update_flags([mem] {
        return (mem >> 6) & 1;
    }, emulator::overflow);

    update_zero(cpu, mem & cpu->accumulator());
    update_sign(cpu, mem);
}

// BMI Branch on Result Minus
void emulator::bmi(CPU* cpu)
{
    if (cpu->status() & emulator::sign)
    {
        auto address = cpu->address_to_arguemnts();
        cpu->set_program_counter(address);
        // TODO Upate cycle?
        // Branch
    }
}

// BNE Branch on Result not Zero
void emulator::bne(CPU* cpu)
{
    if ((cpu->status() & emulator::zero) == 0)
    {
        auto address = cpu->address_to_arguemnts();
        cpu->set_program_counter(address);
        // TODO Upate cycle?
        // Branch
    }

}

// BPL Branch on Result Plus
void emulator::bpl(CPU* cpu)
{
    if ((cpu->status() & emulator::sign) == 0)
    {
        auto address = cpu->address_to_arguemnts();
        cpu->set_program_counter(address);
        // TODO Upate cycle?
        // Branch
    }
}

// BRK Force Break
void emulator::brk(CPU* cpu)
{
    // TODO
    // PC + 2 ToS P ToS
}

// BVC Branch on Overflow Clear
void emulator::bvc(CPU* cpu)
{
    if ((cpu->status() & emulator::overflow) == 0)
    {
        auto address = cpu->address_to_arguemnts();
        cpu->set_program_counter(address);
        // TODO Upate cycle?
        // Branch
    }
}

// BVS Branch on Overflow Set
void emulator::bvs(CPU* cpu)
{
    if (cpu->status() & emulator::overflow)
    {
        auto address = cpu->address_to_arguemnts();
        cpu->set_program_counter(address);
        // TODO Upate cycle?
        // Branch
    }
}

// CLC Clear Carry Flag
void emulator::clc(CPU* cpu)
{
    cpu->remove_flags(carry);
}

// CLD Clear Decimal Mode
void emulator::cld(CPU* cpu)
{
    cpu->remove_flags(decimal);
}

// CLI Clear interrupt Disable Bit
void emulator::cli(CPU* cpu)
{
    cpu->remove_flags(interrupt);
}

// CLV Clear Overflow Flag
void emulator::clv(CPU* cpu)
{
    cpu->remove_flags(overflow);
}

// CMP Compare Memory and Accumulator
void emulator::cmp(CPU* cpu)
{
    auto mem = cpu->read8(cpu->address_to_arguemnts());
    mem = cpu->accumulator() - mem;
    update_carry(cpu, mem);
    update_zero_sign(cpu, mem);
}

// CPX Compare Memory and Index X
void emulator::cpx(CPU* cpu)
{
    auto mem = cpu->read8(cpu->address_to_arguemnts());
    mem = cpu->x_register() - mem;
    update_carry(cpu, mem);
    update_zero_sign(cpu, mem);
}

// CPY Compare Memory and Index Y
void emulator::cpy(CPU* cpu)
{
    auto mem = cpu->read8(cpu->address_to_arguemnts());
    mem = cpu->y_register() - mem;
    update_carry(cpu, mem);
    update_zero_sign(cpu, mem);
}

// DEC Decrement Memory by One
void emulator::dec(CPU* cpu)
{
    auto address = cpu->address_to_arguemnts();
    auto mem = cpu->read8(address);
    mem = (mem - 1) & 0xFF;
    update_zero_sign(cpu, mem);
    cpu->write8(address, mem);
}

// DEX Decrement Index X by One
void emulator::dex(CPU* cpu)
{
    auto x = cpu->x_register();
    cpu->set_x_register(x - 1);
    update_zero_sign(cpu, x);
}

// DEY Decrement Index Y by One
void emulator::dey(CPU* cpu)
{
    auto y = cpu->y_register();
    cpu->set_y_register(y - 1);
    update_zero_sign(cpu, y);
}

// EOR "Exclusive-Or" Memory with Accumulator
void emulator::eor(CPU* cpu)
{
    auto address = cpu->address_to_arguemnts();
    auto mem = cpu->read8(address);
    cpu->set_accumulator(mem ^ cpu->accumulator());
}

// INC Increment Memory by One
void emulator::inc(CPU* cpu)
{
    auto address = cpu->address_to_arguemnts();
    auto mem = cpu->read8(address);
    cpu->write8(address, (mem + 1) & 0xFF);
    update_zero_sign(cpu, mem);
}

// INX Increment Index X by One
void emulator::inx(CPU* cpu)
{
    auto x = cpu->x_register();
    cpu->set_x_register((x + 1) & 0xFF);
    update_zero_sign(cpu, x);
}

// INY Increment Index Y by One
void emulator::iny(CPU* cpu)
{
    auto y = cpu->x_register();
    cpu->set_y_register((y + 1) & 0xFF);
    update_zero_sign(cpu, y);
}

// JMP Jump to New Location
void emulator::jmp(CPU* cpu)
{
    auto address = cpu->address_to_arguemnts();
    cpu->set_program_counter(address);
}

// JSR Jump to New Location Saving Return Address
void emulator::jsr(CPU* cpu)
{
}

// LDA Load Accumulator with Memory
void emulator::lda(CPU* cpu)
{
    auto mem = cpu->read8(cpu->address_to_arguemnts());
    cpu->set_accumulator(mem);
    update_zero_sign(cpu, mem);
}

// LDX Load Index X with Memory
void emulator::ldx(CPU* cpu)
{
    auto mem = cpu->read8(cpu->address_to_arguemnts());
    cpu->set_x_register(mem);
    update_zero_sign(cpu, mem);
}

// LDY Load Index Y with Memory
void emulator::ldy(CPU* cpu)
{
    auto mem = cpu->read8(cpu->address_to_arguemnts());
    cpu->set_y_register(mem);
    update_zero_sign(cpu, mem);
}

// LSR Shift Right One Bit (Memory or Accumulator)
void emulator::lsr(CPU* cpu)
{
}

// NOP No Operation
void emulator::nop(CPU* cpu)
{
}

// ORA "OR" Memory with Accumulator
void emulator::ora(CPU* cpu)
{
}

// PHA Push Accumulator on Stack
void emulator::pha(CPU* cpu)
{
}

// PHP Push Processor Status on Stack
void emulator::php(CPU* cpu)
{
}

// PLA Pull Accumulator from Stack
void emulator::pla(CPU* cpu)
{
}

// PLP Pull Processor Status from Stack
void emulator::plp(CPU* cpu)
{
}

// ROL Rotate One Bit Left (Memory or Accumulator)
void emulator::rol(CPU* cpu)
{
}

// ROR Rotate One Bit Right (Memory or Accumulator)
void emulator::ror(CPU* cpu)
{
}

// RTI Return from Interrupt
void emulator::rti(CPU* cpu)
{
}

// RTS Return from Subroutine
void emulator::rts(CPU* cpu)
{
}

// SBC Subtract Memory from Accumulator with Borrow
void emulator::sbc(CPU* cpu)
{
}

// SEC Set Carry Flag
void emulator::sec(CPU* cpu)
{
    cpu->add_flags(carry);
}

// SED Set Decimal Mode
void emulator::sed(CPU* cpu)
{
    cpu->add_flags(decimal);
}

// SEI Set Interrupt Disable Status
void emulator::sei(CPU* cpu)
{
    cpu->add_flags(interrupt);
}

// STA Store Accumulator in Memory
void emulator::sta(CPU* cpu)
{
}

// STX Store Index X in Memory
void emulator::stx(CPU* cpu)
{
}

// STY Store Index Y in Memory
void emulator::sty(CPU* cpu)
{
}

// TAX Transfer Accumulator to Index X
void emulator::tax(CPU* cpu)
{
}

// TAY Transfer Accumulator to Index Y
void emulator::tay(CPU* cpu)
{
}

// TSX Transfer Stack Pointer to Index X
void emulator::tsx(CPU* cpu)
{
}

// TXA Transfer Index X to Accumulator
void emulator::txa(CPU* cpu)
{
}

// TXS Transfer Index X to Stack Pointer
void emulator::txs(CPU* cpu)
{
}

// TYA Transfer Index Y to Accumulator
void emulator::tya(CPU* cpu)
{
}

// Unoffical opcodes V
void emulator::ahx(CPU* /*cpu*/)
{
}

void emulator::alr(CPU* /*cpu*/)
{
}

void emulator::anc(CPU* /*cpu*/)
{
}

void emulator::arr(CPU* /*cpu*/)
{
}

void emulator::axs(CPU* /*cpu*/)
{
}

void emulator::dcp(CPU* /*cpu*/)
{
}

void emulator::isc(CPU* /*cpu*/)
{
}

void emulator::kil(CPU* /*cpu*/)
{
}

void emulator::las(CPU* /*cpu*/)
{
}

void emulator::lax(CPU* /*cpu*/)
{
}

void emulator::rla(CPU* /*cpu*/)
{
}

void emulator::rra(CPU* /*cpu*/)
{
}

void emulator::sax(CPU* /*cpu*/)
{
}

void emulator::shx(CPU* /*cpu*/)
{
}

void emulator::shy(CPU* /*cpu*/)
{
}

void emulator::slo(CPU* /*cpu*/)
{
}

void emulator::sre(CPU* /*cpu*/)
{
}

void emulator::tas(CPU* /*cpu*/)
{
}

void emulator::xaa(CPU* /*cpu*/)
{
}
