#include <assert.h>
#include <errno.h>
#include <stddef.h>

#include <pgb/cpu/instruction_info.h>
#include <pgb/cpu/private/lr35902.h>
#include <pgb/cpu/registers.h>
#include <pgb/debug.h>
#include <pgb/utils.h>

#define DETECT_UNSIGNED_ARITHMETIC_OVERFLOW(a, b) (((a) + (b)) < (a))
#define DETECT_UNSIGNED_ARITHMETIC_UNDERFLOW(a, b) (((a) - (b)) > (a))

static
int interpreter_execute_instruction_adc(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_add(struct device *device, struct decoded_instruction *instruction)
{
	int ret;
	uint8_t src8, dst8;
	uint16_t src16, dst16;
	struct cpu *cpu;
	struct instruction_info *info;

	cpu = &device->cpu;
	info = instruction->info;

	switch (info->operands.a.type) {
	case INSTRUCTION_OPERAND_TYPE_REGISTER8:
		ret = cpu_register_read8(cpu, info->operands.a.operand, &dst8);
		break;
	case INSTRUCTION_OPERAND_TYPE_REGISTER16:
		ret = cpu_register_read16(cpu, info->operands.a.operand, &dst16);
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_RETURN(ret == 0, ret);

	switch (info->operands.b.type) {
	case INSTRUCTION_OPERAND_TYPE_REGISTER8:
		ret = cpu_register_read8(cpu, info->operands.b.operand, &src8);
		break;
	case INSTRUCTION_OPERAND_TYPE_REGISTER16:
		ret = cpu_register_read16(cpu, info->operands.b.operand, &src16);
		break;
	case INSTRUCTION_OPERAND_TYPE_NONE:
		ret = cpu_register_read8(cpu, INSTRUCTION_OPERAND_A, &src8);
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_RETURN(ret == 0, ret);

	switch (info->operands.a.type) {
	case INSTRUCTION_OPERAND_TYPE_REGISTER8:
		dst8 += src8;
		break;
	case INSTRUCTION_OPERAND_TYPE_REGISTER16:
		dst16 += src16;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_RETURN(ret == 0, ret);

	switch (info->operands.a.type) {
	case INSTRUCTION_OPERAND_TYPE_REGISTER8:
		ret = cpu_register_write8(cpu, info->operands.a.operand, dst8);
		break;
	case INSTRUCTION_OPERAND_TYPE_REGISTER16:
		ret = cpu_register_write16(cpu, info->operands.a.operand, dst16);
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

static
int interpreter_execute_instruction_addc(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_and(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_call(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_ccf(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_cp(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_cpl(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_daa(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_dec(struct device *device, struct decoded_instruction *instruction)
{
	int ret;
	uint8_t src8;
	uint16_t src16;
	struct cpu *cpu;
	struct instruction_info *info;

	cpu = &device->cpu;
	info = instruction->info;

	switch (info->operands.a.type) {
	case INSTRUCTION_OPERAND_TYPE_REGISTER8:
		ret = cpu_register_read8(cpu, info->operands.a.operand, &src8);
		break;
	case INSTRUCTION_OPERAND_TYPE_REGISTER16:
		ret = cpu_register_read16(cpu, info->operands.a.operand, &src16);
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_RETURN(ret == 0, ret);

	switch (info->operands.a.type) {
		case INSTRUCTION_OPERAND_TYPE_REGISTER8:
		src8 -= 1;
		break;
	case INSTRUCTION_OPERAND_TYPE_REGISTER16:
		src16 -= 1;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_RETURN(ret == 0, ret);

	switch (info->operands.a.type) {
	case INSTRUCTION_OPERAND_TYPE_REGISTER8:
		ret = cpu_register_write8(cpu, info->operands.a.operand, src8);
		break;
	case INSTRUCTION_OPERAND_TYPE_REGISTER16:
		ret = cpu_register_write16(cpu, info->operands.a.operand, src16);
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

static
int interpreter_execute_instruction_di(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_ei(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_halt(struct device *device, struct decoded_instruction *instruction)
{
	device->cpu.status.halted = true;

	return 0;
}

static
int interpreter_execute_instruction_inc(struct device *device, struct decoded_instruction *instruction)
{
	int ret;
	uint8_t src8;
	uint16_t src16;
	struct cpu *cpu;
	struct instruction_info *info;

	cpu = &device->cpu;
	info = instruction->info;

	switch (info->operands.a.type) {
	case INSTRUCTION_OPERAND_TYPE_REGISTER8:
		ret = cpu_register_read8(cpu, info->operands.a.operand, &src8);
		break;
	case INSTRUCTION_OPERAND_TYPE_REGISTER16:
		ret = cpu_register_read16(cpu, info->operands.a.operand, &src16);
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_RETURN(ret == 0, ret);

	switch (info->operands.a.type) {
		case INSTRUCTION_OPERAND_TYPE_REGISTER8:
		src8 += 1;
		break;
	case INSTRUCTION_OPERAND_TYPE_REGISTER16:
		src16 += 1;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_RETURN(ret == 0, ret);

	switch (info->operands.a.type) {
	case INSTRUCTION_OPERAND_TYPE_REGISTER8:
		ret = cpu_register_write8(cpu, info->operands.a.operand, src8);
		break;
	case INSTRUCTION_OPERAND_TYPE_REGISTER16:
		ret = cpu_register_write16(cpu, info->operands.a.operand, src16);
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

static
int interpreter_execute_instruction_jp(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_jr(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_ld(struct device *device, struct decoded_instruction *instruction)
{
	int ret;
	uint8_t src8, addr8;
	uint16_t src16, addr16;
	struct cpu *cpu;
	struct mmu *mmu;
	struct instruction_info *info;

	cpu = &device->cpu;
	mmu = &device->mmu;
	info = instruction->info;

	switch (info->operands.b.modifier) {
	case INSTRUCTION_OPERAND_MODIFIER_MEM_READ_8:
		ret = 0;
		break;
	case INSTRUCTION_OPERAND_MODIFIER_MEM_READ_16:
		ret = 0;
		break;
	case INSTRUCTION_OPERAND_MODIFIER_NONE:
		switch (info->operands.b.type) {
		case INSTRUCTION_OPERAND_TYPE_U8:
			src8 = instruction->b.u8;
			ret = 0;
			break;
		case INSTRUCTION_OPERAND_TYPE_U16:
			src16 = instruction->b.u16;
			ret = 0;
			break;
		case INSTRUCTION_OPERAND_TYPE_REGISTER8:
			ret = cpu_register_read8(cpu, info->operands.b.operand, &src8);
			break;
		case INSTRUCTION_OPERAND_TYPE_REGISTER16:
			ret = cpu_register_read16(cpu, info->operands.b.operand, &src16);
			break;
		default:
			ret = -EINVAL;
			break;
		}
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_RETURN(ret == 0, ret);

	switch (info->operands.a.modifier) {
	case INSTRUCTION_OPERAND_MODIFIER_MEM_WRITE_8:
		{
			switch (info->operands.a.type) {
			case INSTRUCTION_OPERAND_TYPE_REGISTER8:
				ret = cpu_register_read8(cpu, info->operands.a.operand, &addr8);
				break;
			default:
				ret = -EINVAL;
				break;
			}
			OK_OR_RETURN(ret == 0, ret);

			ret = mmu_write_byte(mmu, addr8, src8);
		}
		break;
	case INSTRUCTION_OPERAND_MODIFIER_MEM_WRITE_16:
		{
			switch (info->operands.a.type) {
			case INSTRUCTION_OPERAND_TYPE_REGISTER16:
				ret = cpu_register_read16(cpu, info->operands.a.operand, &addr16);
				break;
			case INSTRUCTION_OPERAND_TYPE_U16:
				addr16 = instruction->a.u16;
				break;
			default:
				ret = -EINVAL;
				break;
			}
			OK_OR_RETURN(ret == 0, ret);

			ret = mmu_write_word(mmu, addr16, src16);
		}
		break;
	case INSTRUCTION_OPERAND_MODIFIER_NONE:
		switch (info->operands.a.type) {
		case INSTRUCTION_OPERAND_TYPE_REGISTER8:
			ret = cpu_register_write8(cpu, info->operands.a.operand, src8);
			break;
		case INSTRUCTION_OPERAND_TYPE_REGISTER16:
			ret = cpu_register_write16(cpu, info->operands.a.operand, src16);
			break;
		default:
			ret = -EINVAL;
			break;
		}
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

static
int interpreter_execute_instruction_ldd(struct device *device, struct decoded_instruction *instruction)
{
	int ret;
	uint8_t src8;
	uint16_t addr16;
	struct cpu *cpu;
	struct mmu *mmu;
	struct instruction_info *info;

	cpu = &device->cpu;
	mmu = &device->mmu;
	info = instruction->info;

	switch (info->operands.b.modifier) {
	case INSTRUCTION_OPERAND_MODIFIER_MEM_READ_16:
		{
			ret = cpu_register_read16(cpu, info->operands.b.operand, &addr16);
			OK_OR_RETURN(ret == 0, ret);

			ret = mmu_read_byte(mmu, addr16, &src8);
		}
		break;
	case INSTRUCTION_OPERAND_MODIFIER_NONE:
		switch (info->operands.b.type) {
		case INSTRUCTION_OPERAND_TYPE_REGISTER8:
			ret = cpu_register_read8(cpu, info->operands.b.operand, &src8);
			break;
		default:
			ret = -EINVAL;
			break;
		}
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_RETURN(ret == 0, ret);

	switch (info->operands.a.modifier) {
	case INSTRUCTION_OPERAND_MODIFIER_MEM_WRITE_16:
		{
			ret = cpu_register_read16(cpu, info->operands.a.operand, &addr16);
			OK_OR_RETURN(ret == 0, ret);

			ret = mmu_write_byte(mmu, addr16, src8);
		}
		break;
	case INSTRUCTION_OPERAND_MODIFIER_NONE:
		switch (info->operands.b.type) {
		case INSTRUCTION_OPERAND_TYPE_REGISTER8:
			ret = cpu_register_write8(cpu, info->operands.a.operand, src8);
			break;
		default:
			ret = -EINVAL;
			break;
		}
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_RETURN(ret == 0, ret);

	ret = cpu_register_write16(cpu, INSTRUCTION_OPERAND_HL, addr16 - 1);
	OK_OR_WARN(ret == 0);

	return ret;
}

static
int interpreter_execute_instruction_ldh(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_ldhl(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_ldi(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_nop(struct device *device, struct decoded_instruction *instruction)
{
	/* NOP */
	return 0;
}

static
int interpreter_execute_instruction_or(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_pop(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_prefix(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_push(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_ret(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_reti(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_rst(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_sbc(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_scf(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_stop(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_sub(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_xor(struct device *device, struct decoded_instruction *instruction)
{
	int ret;
	uint8_t src8, dst8;
	uint16_t src16, dst16;
	struct cpu *cpu;
	struct instruction_info *info;

	cpu = &device->cpu;
	info = instruction->info;

	switch (info->operands.a.type) {
	case INSTRUCTION_OPERAND_TYPE_REGISTER8:
		ret = cpu_register_read8(cpu, info->operands.a.operand, &dst8);
		break;
	case INSTRUCTION_OPERAND_TYPE_REGISTER16:
		ret = cpu_register_read16(cpu, info->operands.a.operand, &dst16);
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_RETURN(ret == 0, ret);

	switch (info->operands.b.type) {
	case INSTRUCTION_OPERAND_TYPE_REGISTER8:
		ret = cpu_register_read8(cpu, info->operands.b.operand, &src8);
		break;
	case INSTRUCTION_OPERAND_TYPE_REGISTER16:
		ret = cpu_register_read16(cpu, info->operands.b.operand, &src16);
		break;
	case INSTRUCTION_OPERAND_TYPE_NONE:
		ret = cpu_register_read8(cpu, INSTRUCTION_OPERAND_A, &src8);
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_RETURN(ret == 0, ret);

	switch (info->operands.a.type) {
	case INSTRUCTION_OPERAND_TYPE_REGISTER8:
		dst8 ^= src8;
		break;
	case INSTRUCTION_OPERAND_TYPE_REGISTER16:
		dst16 ^= src16;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_RETURN(ret == 0, ret);

	switch (info->operands.a.type) {
	case INSTRUCTION_OPERAND_TYPE_REGISTER8:
		ret = cpu_register_write8(cpu, info->operands.a.operand, dst8);
		break;
	case INSTRUCTION_OPERAND_TYPE_REGISTER16:
		ret = cpu_register_write16(cpu, info->operands.a.operand, dst16);
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

static
int interpreter_execute_instruction_bit(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_res(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_rl(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_rla(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_rlc(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_rlca(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_rr(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_rra(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_rrc(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_rrca(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_set(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_sla(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_sra(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_srl(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

static
int interpreter_execute_instruction_swap(struct device *device, struct decoded_instruction *instruction)
{
	return 0;
}

int interpreter_execute_instruction(struct device *device, struct decoded_instruction *instruction)
{
	int ret = 0;

	assert(instruction->info != NULL && "Instruction information should not be NULL");

	switch (instruction->info->instruction_class) {
	case INSTRUCTION_CLASS_ADC:
		ret = interpreter_execute_instruction_adc(device, instruction);
		break;
	case INSTRUCTION_CLASS_ADD:
		ret = interpreter_execute_instruction_add(device, instruction);
		break;
	case INSTRUCTION_CLASS_ADDC:
		ret = interpreter_execute_instruction_addc(device, instruction);
		break;
	case INSTRUCTION_CLASS_AND:
		ret = interpreter_execute_instruction_and(device, instruction);
		break;
	case INSTRUCTION_CLASS_CALL:
		ret = interpreter_execute_instruction_call(device, instruction);
		break;
	case INSTRUCTION_CLASS_CCF:
		ret = interpreter_execute_instruction_ccf(device, instruction);
		break;
	case INSTRUCTION_CLASS_CP:
		ret = interpreter_execute_instruction_cp(device, instruction);
		break;
	case INSTRUCTION_CLASS_CPL:
		ret = interpreter_execute_instruction_cpl(device, instruction);
		break;
	case INSTRUCTION_CLASS_DAA:
		ret = interpreter_execute_instruction_daa(device, instruction);
		break;
	case INSTRUCTION_CLASS_DEC:
		ret = interpreter_execute_instruction_dec(device, instruction);
		break;
	case INSTRUCTION_CLASS_DI:
		ret = interpreter_execute_instruction_di(device, instruction);
		break;
	case INSTRUCTION_CLASS_EI:
		ret = interpreter_execute_instruction_ei(device, instruction);
		break;
	case INSTRUCTION_CLASS_HALT:
		ret = interpreter_execute_instruction_halt(device, instruction);
		break;
	case INSTRUCTION_CLASS_INC:
		ret = interpreter_execute_instruction_inc(device, instruction);
		break;
	case INSTRUCTION_CLASS_JP:
		ret = interpreter_execute_instruction_jp(device, instruction);
		break;
	case INSTRUCTION_CLASS_JR:
		ret = interpreter_execute_instruction_jr(device, instruction);
		break;
	case INSTRUCTION_CLASS_LD:
		ret = interpreter_execute_instruction_ld(device, instruction);
		break;
	case INSTRUCTION_CLASS_LDD:
		ret = interpreter_execute_instruction_ldd(device, instruction);
		break;
	case INSTRUCTION_CLASS_LDH:
		ret = interpreter_execute_instruction_ldh(device, instruction);
		break;
	case INSTRUCTION_CLASS_LDHL:
		ret = interpreter_execute_instruction_ldhl(device, instruction);
		break;
	case INSTRUCTION_CLASS_LDI:
		ret = interpreter_execute_instruction_ldi(device, instruction);
		break;
	case INSTRUCTION_CLASS_NOP:
		ret = interpreter_execute_instruction_nop(device, instruction);
		break;
	case INSTRUCTION_CLASS_OR:
		ret = interpreter_execute_instruction_or(device, instruction);
		break;
	case INSTRUCTION_CLASS_POP:
		ret = interpreter_execute_instruction_pop(device, instruction);
		break;
	case INSTRUCTION_CLASS_PREFIX:
		ret = interpreter_execute_instruction_prefix(device, instruction);
		break;
	case INSTRUCTION_CLASS_PUSH:
		ret = interpreter_execute_instruction_push(device, instruction);
		break;
	case INSTRUCTION_CLASS_RET:
		ret = interpreter_execute_instruction_ret(device, instruction);
		break;
	case INSTRUCTION_CLASS_RETI:
		ret = interpreter_execute_instruction_reti(device, instruction);
		break;
	case INSTRUCTION_CLASS_RST:
		ret = interpreter_execute_instruction_rst(device, instruction);
		break;
	case INSTRUCTION_CLASS_SBC:
		ret = interpreter_execute_instruction_sbc(device, instruction);
		break;
	case INSTRUCTION_CLASS_SCF:
		ret = interpreter_execute_instruction_scf(device, instruction);
		break;
	case INSTRUCTION_CLASS_STOP:
		ret = interpreter_execute_instruction_stop(device, instruction);
		break;
	case INSTRUCTION_CLASS_SUB:
		ret = interpreter_execute_instruction_sub(device, instruction);
		break;
	case INSTRUCTION_CLASS_XOR:
		ret = interpreter_execute_instruction_xor(device, instruction);
		break;
	/* Start of prefix instructions */
	case INSTRUCTION_CLASS_BIT:
		ret = interpreter_execute_instruction_bit(device, instruction);
		break;
	case INSTRUCTION_CLASS_RES:
		ret = interpreter_execute_instruction_res(device, instruction);
		break;
	case INSTRUCTION_CLASS_RL:
		ret = interpreter_execute_instruction_rl(device, instruction);
		break;
	case INSTRUCTION_CLASS_RLA:
		ret = interpreter_execute_instruction_rla(device, instruction);
		break;
	case INSTRUCTION_CLASS_RLC:
		ret = interpreter_execute_instruction_rlc(device, instruction);
		break;
	case INSTRUCTION_CLASS_RLCA:
		ret = interpreter_execute_instruction_rlca(device, instruction);
		break;
	case INSTRUCTION_CLASS_RR:
		ret = interpreter_execute_instruction_rr(device, instruction);
		break;
	case INSTRUCTION_CLASS_RRA:
		ret = interpreter_execute_instruction_rra(device, instruction);
		break;
	case INSTRUCTION_CLASS_RRC:
		ret = interpreter_execute_instruction_rrc(device, instruction);
		break;
	case INSTRUCTION_CLASS_RRCA:
		ret = interpreter_execute_instruction_rrca(device, instruction);
		break;
	case INSTRUCTION_CLASS_SET:
		ret = interpreter_execute_instruction_set(device, instruction);
		break;
	case INSTRUCTION_CLASS_SLA:
		ret = interpreter_execute_instruction_sla(device, instruction);
		break;
	case INSTRUCTION_CLASS_SRA:
		ret = interpreter_execute_instruction_sra(device, instruction);
		break;
	case INSTRUCTION_CLASS_SRL:
		ret = interpreter_execute_instruction_srl(device, instruction);
		break;
	case INSTRUCTION_CLASS_SWAP:
		ret = interpreter_execute_instruction_swap(device, instruction);
		break;
	default:
		ret = -EINVAL;
		fprintf(stderr, "Encountered unexpected operation type '%02x'. (Emulator error?)\n", instruction->info->instruction_class);
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}
