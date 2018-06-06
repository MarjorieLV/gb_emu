#ifndef _INSTRUCTIONS_
#define _INSTRUCTIONS_

#include "iinstructions.hpp"

//OpCode 0x00
class NOP : public IInstructions
{
public :
    NOP(int cycles)
        :IInstructions(cycles){};

    void doInstruction(IMemory& memory) override {
        uint16_t cursor = memory.get16BitRegister(IMemory::REG16BIT::PC);
        memory.set16BitRegister(IMemory::REG16BIT::PC, cursor + 1);
    }
};

//OpCode 0x01 0x11 0x21 0x31
class Load16NextBitToRegister : public IInstructions
{
public:
    Load16NextBitToRegister(int cycles, IMemory::REG16BIT reg)
        :IInstructions(cycles),
         _register(reg){};

    void doInstruction(IMemory& memory) override {
        uint16_t cursor = memory.get16BitRegister(IMemory::REG16BIT::PC);
        IMemory::RomData const & rom = memory.getReadOnlyMemory();
        uint16_t value = ((uint16_t)rom[cursor + 2] << 8) | rom[cursor + 1];
        memory.set16BitRegister(_register, value);
        memory.set16BitRegister(IMemory::REG16BIT::PC, cursor + 3);
    }

    IMemory::REG16BIT _register;
};

//OpCode 0x04 0x0C 0x14 0x1C 0x24 0x2C 0x3C
class Increment8BitRegister : public IInstructions
{
public:
    Increment8BitRegister(int cycles, IMemory::REG8BIT reg8Bit)
        :IInstructions(cycles),
         _reg8Bit(reg8Bit){};

    void doInstruction(IMemory& memory) override {
        uint8_t regValue = memory.get8BitRegister(_reg8Bit);
        memory.set8BitRegister(_reg8Bit, regValue + 1);
        uint16_t cursor = memory.get16BitRegister(IMemory::REG16BIT::PC);
        memory.set16BitRegister(IMemory::REG16BIT::PC, cursor + 1);
    }

    IMemory::REG8BIT _reg8Bit;
};

//OpCode 0x03 0x13 0x23C 0x33
class Increment16NitRegister : public IInstructions
{
public:
    Increment16NitRegister(int cycles, IMemory::REG16BIT reg16Bit)
        :IInstructions(cycles),
         _reg16Bit(reg16Bit){};

    void doInstruction(IMemory& memory) override {
        uint16_t regValue = memory.get16BitRegister(_reg16Bit);
        memory.set16BitRegister(_reg16Bit, regValue + 1);
        uint16_t cursor = memory.get16BitRegister(IMemory::REG16BIT::PC);
        memory.set16BitRegister(IMemory::REG16BIT::PC, cursor + 1);
    }

    IMemory::REG16BIT _reg16Bit;
};

//OpCode 0x02 0x12 0x22 0x32 0x70 0x71 0x72 0x73 0x74 0x75 0x77
class Load8BitRegValueToAdressInReg : public IInstructions
{
public:
    Load8BitRegValueToAdressInReg(int cycles, IMemory::REG16BIT reg16Bit, IMemory::REG8BIT reg8Bit, int addTo16BitReg)
        :IInstructions(cycles),
         _16BitReg(reg16Bit),
         _8BitReg(reg8Bit),
         _addTo16BitReg(addTo16BitReg){};

    void doInstruction(IMemory& memory) override {
        uint8_t reg8BitValue = memory.get8BitRegister(_8BitReg);
        uint16_t reg16BitValue = memory.get16BitRegister(_16BitReg);
        memory.writeInROM(reg8BitValue, reg16BitValue);
        uint16_t cursor = memory.get16BitRegister(IMemory::REG16BIT::PC);
        memory.set16BitRegister(IMemory::REG16BIT::PC, cursor + 1);
        if (_addTo16BitReg != 0) {
            reg16BitValue += _addTo16BitReg;
            memory.set16BitRegister(_16BitReg, reg16BitValue);
        }
    }

    IMemory::REG16BIT _16BitReg;
    IMemory::REG8BIT _8BitReg;
    int _addTo16BitReg;
};

//OpCode 0x40 to 0x45 0x47 to 0x4D 0x4F 0x50 to 0x55 0x57 to 0x5D 0x5F 0x60 to 0x65 0x67 to 0x6D 0x6F 0x78 to 0x7D 0x7F
class Load8BitRegValueTo8BitRegister : public IInstructions
{
public:
    Load8BitRegValueTo8BitRegister(int cycles, IMemory::REG8BIT toCopyTo, IMemory::REG8BIT toCopyFrom)
        :IInstructions(cycles),
         _toCopyTo(toCopyTo),
         _toCopyFrom(toCopyFrom){};

    void doInstruction(IMemory& memory) override {
        uint8_t valueToCopy = memory.get8BitRegister(_toCopyFrom);
        memory.set8BitRegister(_toCopyTo, valueToCopy);
        uint8_t cursor = memory.get16BitRegister(IMemory::REG16BIT::PC);
        memory.set16BitRegister(IMemory::REG16BIT::PC, cursor + 1);
    }

    IMemory::REG8BIT _toCopyTo;
    IMemory::REG8BIT _toCopyFrom;
};
#endif /*INSTRUCTIONS*/
