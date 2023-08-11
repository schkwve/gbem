#include <instr.h>
#include <cpu.h>

instruction instr_set[0x100] = {
	[0x00] = { IN_NOP, AM_IMP },	   [0x05] = { IN_DEC, AM_R, RT_B },
	[0x0E] = { IN_LD, AM_R_D8, RT_C }, [0xAF] = { IN_XOR, AM_R, RT_A },
	[0xC3] = { IN_JP, AM_D16 },		   [0xF3] = { IN_DI }
};

instruction *inst_get_by_opcode(uint8_t opcode)
{
	return &instr_set[opcode];
}
