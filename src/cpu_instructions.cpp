#include "cpu_instructions.h"

// ADC Add Memory to Accumulator with Carry
void emulator::adc(CPU* cpu)
{
}

// AND "AND" Memory with Accumulator
void emulator::nd(CPU* cpu)
{
}

// ASL Shift Left One Bit (Memory or Accumulator)
void emulator::asl(CPU* cpu)
{
}

// BCC Branch on Carry Clear
void emulator::bcc(CPU* cpu)
{
}

// BCS Branch on Carry Set
void emulator::bcs(CPU* cpu)
{
}

// BEQ Branch on Result Zero
void emulator::beq(CPU* cpu)
{
}

// BIT Test Bits in Memory with Accumulator
void emulator::bit(CPU* cpu)
{
}

// BMI Branch on Result Minus
void emulator::bmi(CPU* cpu)
{
}

// BNE Branch on Result not Zero
void emulator::bne(CPU* cpu)
{
}

// BPL Branch on Result Plus
void emulator::bpl(CPU* cpu)
{
}

// BRK Force Break
void emulator::brk(CPU* cpu)
{
}

// BVC Branch on Overflow Clear
void emulator::bvc(CPU* cpu)
{
}

// BVS Branch on Overflow Set
void emulator::bvs(CPU* cpu)
{
}

// CLC Clear Carry Flag
void emulator::clc(CPU* cpu)
{
}

// CLD Clear Decimal Mode
void emulator::cld(CPU* cpu)
{
}

// CLI Clear interrupt Disable Bit
void emulator::cli(CPU* cpu)
{
}

// CLV Clear Overflow Flag
void emulator::clv(CPU* cpu)
{
}

// CMP Compare Memory and Accumulator
void emulator::cmp(CPU* cpu)
{
}

// CPX Compare Memory and Index X
void emulator::cpx(CPU* cpu)
{
}

// CPY Compare Memory and Index Y
void emulator::cpy(CPU* cpu)
{
}

// DEC Decrement Memory by One
void emulator::dec(CPU* cpu)
{
}

// DEX Decrement Index X by One
void emulator::dex(CPU* cpu)
{
}

// DEY Decrement Index Y by One
void emulator::dey(CPU* cpu)
{
}

// EOR "Exclusive-Or" Memory with Accumulator
void emulator::eor(CPU* cpu)
{
}

// INC Increment Memory by One
void emulator::inc(CPU* cpu)
{
}

// INX Increment Index X by One
void emulator::inx(CPU* cpu)
{
}

// INY Increment Index Y by One
void emulator::iny(CPU* cpu)
{
}

// JMP Jump to New Location
void emulator::jmp(CPU* cpu)
{
}

// JSR Jump to New Location Saving Return Address
void emulator::jsr(CPU* cpu)
{
}

// LDA Load Accumulator with Memory
void emulator::lda(CPU* cpu)
{
}

// LDX Load Index X with Memory
void emulator::ldx(CPU* cpu)
{
}

// LDY Load Index Y with Memory
void emulator::ldy(CPU* cpu)
{
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
}

// SED Set Decimal Mode
void emulator::sed(CPU* cpu)
{
}

// SEI Set Interrupt Disable Status
void emulator::sei(CPU* cpu)
{
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
