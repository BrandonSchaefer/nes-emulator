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

void branch_if_cond(std::function<bool()> const& f, emulator::CPU* cpu)
{
    if (f())
    {
        auto address = cpu->address_to_arguemnts();
        cpu->set_program_counter(address);
        cpu->add_branch_cycle(address);
    }
}

}

// ADC Add Memory to Accumulator with Carry
void emulator::adc(CPU* cpu)
{
    auto a            = cpu->accumulator();
    auto mem          = cpu->read8(cpu->address_to_arguemnts());
    uint8_t c         = cpu->carry();
    uint8_t new_value = a + mem + c;

    update_zero(cpu, new_value);

    if (cpu->decimal())
    {
        // TODO BCD
    }
    else
    {
        update_sign(cpu, new_value);
        update_overflow(cpu, a, mem, new_value);

        auto as_int32 =
            static_cast<int>(a) +
            static_cast<int>(mem) +
            static_cast<int>(c);

        update_carry(cpu, as_int32);
    }

    cpu->set_accumulator(new_value);
}

// AND "AND" Memory with Accumulator
void emulator::nd(CPU* cpu)
{
    auto a         = cpu->accumulator();
    auto mem       = cpu->read8(cpu->address_to_arguemnts());
    auto new_value = a & mem;
    cpu->set_accumulator(new_value);

    update_zero_sign(cpu, new_value);
}

// ASL Shift Left One Bit (Memory or Accumulator)
void emulator::asl(CPU* cpu)
{
    auto current_mode  = cpu->current_mode();
    uint16_t new_value = 0;

    if (current_mode == emulator::accumulator)
    {
        new_value = cpu->accumulator() << 1;
        cpu->set_accumulator(new_value);
    }
    else
    {
        auto address = cpu->address_to_arguemnts();
        new_value    = cpu->read8(address) << 1;
        cpu->write8(address, new_value);
    }

    update_zero_sign(cpu, new_value);
}

// BCC Branch on Carry Clear
void emulator::bcc(CPU* cpu)
{
    branch_if_cond([cpu]{ return (cpu->carry()) == 0; }, cpu);
}

// BCS Branch on Carry Set
void emulator::bcs(CPU* cpu)
{
    branch_if_cond([cpu]{ return cpu->carry(); }, cpu);
}

// BEQ Branch on Result Zero
void emulator::beq(CPU* cpu)
{
    branch_if_cond([cpu]{ return cpu->zero(); }, cpu);
}

// BIT Test Bits in Memory with Accumulator
void emulator::bit(CPU* cpu)
{
    auto address = cpu->address_to_arguemnts();
    auto mem     = cpu->read8(address);

    cpu->update_flags([mem] {
        return (mem >> 6) & 1;
    }, emulator::overflow);

    update_zero(cpu, mem & cpu->accumulator());
    update_sign(cpu, mem);
}

// BMI Branch on Result Minus
void emulator::bmi(CPU* cpu)
{
    branch_if_cond([cpu]{ return cpu->sign(); }, cpu);
}

// BNE Branch on Result not Zero
void emulator::bne(CPU* cpu)
{
    branch_if_cond([cpu]{ return (cpu->zero()) == 0; }, cpu);

}

// BPL Branch on Result Plus
void emulator::bpl(CPU* cpu)
{
    branch_if_cond([cpu]{ return (cpu->sign()) == 0; }, cpu);
}

// BRK Force Break
void emulator::brk(CPU* cpu)
{
    auto pc = cpu->program_counter() + 2;
    cpu->push(pc >> 8 & 0xFF);
    cpu->push(pc & 0xFF);
    cpu->add_flags(emulator::brk_inter);
    cpu->push(cpu->status());
    cpu->add_flags(interrupt);
    cpu->set_program_counter(cpu->read8(0xFFFE) | cpu->read8(0xFFFF) << 8);
}

// BVC Branch on Overflow Clear
void emulator::bvc(CPU* cpu)
{
    branch_if_cond([cpu]{ return (cpu->overflow()) == 0; }, cpu);
}

// BVS Branch on Overflow Set
void emulator::bvs(CPU* cpu)
{
    branch_if_cond([cpu]{ return cpu->overflow(); }, cpu);
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
    mem      = cpu->accumulator() - mem;
    update_carry(cpu, mem);
    update_zero_sign(cpu, mem);
}

// CPX Compare Memory and Index X
void emulator::cpx(CPU* cpu)
{
    auto mem = cpu->read8(cpu->address_to_arguemnts());
    mem      = cpu->x_register() - mem;
    update_carry(cpu, mem);
    update_zero_sign(cpu, mem);
}

// CPY Compare Memory and Index Y
void emulator::cpy(CPU* cpu)
{
    auto mem = cpu->read8(cpu->address_to_arguemnts());
    mem      = cpu->y_register() - mem;
    update_carry(cpu, mem);
    update_zero_sign(cpu, mem);
}

// DEC Decrement Memory by One
void emulator::dec(CPU* cpu)
{
    auto address = cpu->address_to_arguemnts();
    auto mem     = cpu->read8(address);
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
    auto mem     = cpu->read8(address);
    cpu->set_accumulator(mem ^ cpu->accumulator());
}

// INC Increment Memory by One
void emulator::inc(CPU* cpu)
{
    auto address = cpu->address_to_arguemnts();
    auto mem     = cpu->read8(address);
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
    auto y = cpu->y_register();
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
    // Since JSR + args is 3 bytes, and we want to move to next op - 1
    auto pc = cpu->program_counter() + 2;
    cpu->push(pc >> 8 & 0xFF);
    cpu->push(pc & 0xFF);
    auto address = cpu->address_to_arguemnts();
    cpu->set_program_counter(address);
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
    auto current_mode = cpu->current_mode();
    // 16 incase we have a carry
    uint16_t new_value = 0;

    if (current_mode == emulator::accumulator)
    {
        new_value = cpu->accumulator() >> 1;
        cpu->set_accumulator(new_value);
    }
    else
    {
        auto address = cpu->address_to_arguemnts();
        new_value    = cpu->read8(address) >> 1;
        cpu->write8(address, new_value);
    }

    update_zero_sign(cpu, new_value);
}

// NOP No Operation
void emulator::nop(CPU* cpu)
{
}

// ORA "OR" Memory with Accumulator
void emulator::ora(CPU* cpu)
{
    auto mem = cpu->read8(cpu->address_to_arguemnts());
    cpu->set_accumulator(mem | cpu->accumulator());
    update_zero_sign(cpu, mem);
}

// PHA Push Accumulator on Stack
void emulator::pha(CPU* cpu)
{
    cpu->push(cpu->accumulator());
}

// PHP Push Processor Status on Stack
void emulator::php(CPU* cpu)
{
    cpu->push(cpu->status());
}

// PLA Pull Accumulator from Stack
void emulator::pla(CPU* cpu)
{
    cpu->set_accumulator(cpu->pop());
    update_zero_sign(cpu, cpu->accumulator());
}

// PLP Pull Processor Status from Stack
void emulator::plp(CPU* cpu)
{
    auto top_of_stack = cpu->pop();
    cpu->add_flags(top_of_stack);
    update_zero_sign(cpu, top_of_stack);
}

// ROL Rotate One Bit Left (Memory or Accumulator)
void emulator::rol(CPU* cpu)
{
    auto current_mode = cpu->current_mode();
    uint16_t new_value = 0;

    if (current_mode == emulator::accumulator)
    {
        new_value = cpu->accumulator() << 1;
        if (cpu->carry())
        {
            new_value |= 0x1;
        }

        cpu->set_accumulator(new_value);
    }
    else
    {
        auto address = cpu->address_to_arguemnts();
        new_value = cpu->read8(address) << 1;
        if (cpu->carry())
        {
            new_value |= 0x1;
        }

        cpu->write8(address, new_value);
    }

    update_carry(cpu, new_value);
    update_zero_sign(cpu, new_value);
}

// ROR Rotate One Bit Right (Memory or Accumulator)
void emulator::ror(CPU* cpu)
{
    auto current_mode  = cpu->current_mode();
    uint16_t new_value = 0;

    if (current_mode == emulator::accumulator)
    {
        new_value = cpu->accumulator();
        if (cpu->carry())
        {
            new_value |= 0x100;
        }

        // If we are about to shift the least bit we will need to carry
        cpu->update_flags([new_value] { return new_value & 0x1; }, emulator::carry);

        new_value >>= 1;

        cpu->set_accumulator(new_value);
    }
    else
    {
        auto address = cpu->address_to_arguemnts();
        new_value    = cpu->read8(address);
        if (cpu->carry())
        {
            new_value |= 0x100;
        }

        // If we are about to shift the least bit we will need to carry
        cpu->update_flags([new_value] { return new_value & 0x1; }, emulator::carry);

        new_value >>= 1;

        cpu->write8(address, new_value);
    }


    update_zero_sign(cpu, new_value);
}

// RTI Return from Interrupt
void emulator::rti(CPU* cpu)
{
    auto flags = cpu->pop();
    cpu->add_flags(flags);
    auto new_pc = cpu->pop();
    new_pc |= cpu->pop() << 8;
    cpu->set_program_counter(new_pc);
}

// RTS Return from Subroutine
void emulator::rts(CPU* cpu)
{
    uint16_t new_pc = cpu->pop();
    new_pc += (cpu->pop() << 8) + 1;
    cpu->set_program_counter(new_pc);
}

// SBC Subtract Memory from Accumulator with Borrow
void emulator::sbc(CPU* cpu)
{
    auto a             = cpu->accumulator();
    auto mem           = cpu->read8(cpu->address_to_arguemnts());
    auto carry         = cpu->carry();
    uint16_t new_value = a - mem - carry;

    update_zero_sign(cpu, new_value);
    update_overflow(cpu, a, mem, new_value);
    if (cpu->decimal())
    {
        // TODO BCD
    }

    cpu->update_flags([new_value] { return new_value > 0x100; }, emulator::carry);
    cpu->set_accumulator(new_value & 0xFF);
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
    auto address = cpu->address_to_arguemnts();
    cpu->write8(address, cpu->accumulator());
}

// STX Store Index X in Memory
void emulator::stx(CPU* cpu)
{
    auto address = cpu->address_to_arguemnts();
    cpu->write8(address, cpu->x_register());
}

// STY Store Index Y in Memory
void emulator::sty(CPU* cpu)
{
    auto address = cpu->address_to_arguemnts();
    cpu->write8(address, cpu->y_register());
}

// TAX Transfer Accumulator to Index X
void emulator::tax(CPU* cpu)
{
    auto a = cpu->accumulator();
    cpu->set_x_register(a);
    update_zero_sign(cpu, a);
}

// TAY Transfer Accumulator to Index Y
void emulator::tay(CPU* cpu)
{
    auto a = cpu->accumulator();
    cpu->set_y_register(a);
    update_zero_sign(cpu, a);
}

// TSX Transfer Stack Pointer to Index X
void emulator::tsx(CPU* cpu)
{
    auto s = cpu->stack();
    cpu->set_x_register(s);
    update_zero_sign(cpu, s);
}

// TXA Transfer Index X to Accumulator
void emulator::txa(CPU* cpu)
{
    auto x = cpu->x_register();
    cpu->set_accumulator(x);
    update_zero_sign(cpu, x);
}

// TXS Transfer Index X to Stack Pointer
void emulator::txs(CPU* cpu)
{
    auto x = cpu->x_register();
    cpu->set_stack(x);
    update_zero_sign(cpu, x);
}

// TYA Transfer Index Y to Accumulator
void emulator::tya(CPU* cpu)
{
    auto y = cpu->y_register();
    cpu->set_accumulator(y);
    update_zero_sign(cpu, y);
}

// Unoffical opcodes
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
