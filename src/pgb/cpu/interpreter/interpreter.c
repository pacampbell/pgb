#include <assert.h>
#include <errno.h>
#include <stddef.h>

#include <pgb/cpu/isa.h>
#include <pgb/cpu/private/isa.h>
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
	int ret = 0;
	uint16_t src, dst;
	uint8_t src8, dst8;
	struct cpu *cpu;

	cpu = &device->cpu;

	switch (instruction->a.type) {
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER8:
		ret = cpu_register_read8(cpu, instruction->a.reg, &dst8);
		dst = dst8;
		break;
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER16:
		ret = cpu_register_read16(cpu, instruction->a.reg, &dst);
		break;
	default:
		assert(false && "Unsupported add operand type");
		break;
	}
	OK_OR_RETURN(ret == 0, ret);

	switch (instruction->b.type) {
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER8:
		ret = cpu_register_read8(cpu, instruction->b.reg, &src8);
		src = src8;
		break;
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER16:
		ret = cpu_register_read16(cpu, instruction->b.reg, &src);
		break;
	default:
		assert(false && "Unsupported add operand type");
		break;
	}
	OK_OR_RETURN(ret == 0, ret);

	dst += src;

	switch (instruction->a.type) {
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER8:
		dst8 = (uint8_t)dst; // TODO: Check for overlfows
		ret = cpu_register_write8(cpu, instruction->a.reg, dst8);
		break;
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER16:
		ret = cpu_register_write16(cpu, instruction->a.reg, dst);
		break;
	default:
		assert(false && "Unsupported add operand type");
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
	int ret = 0;
	uint16_t src;
	uint8_t src8;
	struct cpu *cpu;

	cpu = &device->cpu;

	switch (instruction->a.type) {
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER8:
		ret = cpu_register_read8(cpu, instruction->a.reg, &src8);
		break;
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER16:
		ret = cpu_register_read16(cpu, instruction->a.reg, &src);
		break;
	default:
		return 0;
	}
	OK_OR_RETURN(ret == 0, ret);

	switch (instruction->a.type) {
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER8:
		src8 -= 1;
		break;
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER16:
		src -= 1;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_RETURN(ret == 0, ret);

	switch (instruction->a.type) {
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER8:
		ret = cpu_register_write8(cpu, instruction->a.reg, src8);
		break;
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER16:
		ret = cpu_register_write16(cpu, instruction->a.reg, src);
		break;
	default:
		return 0;
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
	int ret = 0;
	uint16_t src;
	uint8_t src8;
	struct cpu *cpu;

	cpu = &device->cpu;

	switch (instruction->a.type) {
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER8:
		ret = cpu_register_read8(cpu, instruction->a.reg, &src8);
		break;
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER16:
		ret = cpu_register_read16(cpu, instruction->a.reg, &src);
		break;
	default:
		return 0;
	}
	OK_OR_RETURN(ret == 0, ret);

	switch (instruction->a.type) {
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER8:
		src8 += 1;
		break;
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER16:
		src += 1;
		break;
	default:
		break;
	}

	switch (instruction->a.type) {
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER8:
		ret = cpu_register_write8(cpu, instruction->a.reg, src8);
		break;
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER16:
		ret = cpu_register_write16(cpu, instruction->a.reg, src);
		break;
	default:
		return 0;
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
	int ret = 0;
	uint16_t src;
	uint8_t src8;
	struct cpu *cpu;

	cpu = &device->cpu;

	switch (instruction->b.type) {
	case DECODED_INSTRUCTION_OPERAND_TYPE_UINT8:
		src8 = (uint8_t)instruction->b.value;
		break;
	case DECODED_INSTRUCTION_OPERAND_TYPE_UINT16:
		src = instruction->b.value;
		break;
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER8:
		ret = cpu_register_read8(cpu, instruction->b.reg, &src8);
		break;
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER16:
		ret = cpu_register_read16(cpu, instruction->b.reg, &src);
		break;
	default:
		return 0;
	}
	OK_OR_RETURN(ret == 0, ret);

	switch (instruction->a.type) {
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER8:
		ret = cpu_register_write8(cpu, instruction->a.reg, src8);
		break;
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER16:
		ret = cpu_register_write16(cpu, instruction->a.reg, src);
		break;
	default:
		return 0;
	}
	OK_OR_RETURN(ret == 0, ret);

	return ret;
}

static
int interpreter_execute_instruction_ldd(struct device *device, struct decoded_instruction *instruction)
{
	int ret;
	struct cpu *cpu;
	struct mmu *mmu;
	uint8_t dst8, src8;
	uint16_t dst, src;

	cpu = &device->cpu;
	mmu = &device->mmu;

	switch (instruction->a.type) {
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER8:
		ret = cpu_register_read8(cpu, instruction->a.reg, &dst8);
		break;
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER16:
		ret = cpu_register_read16(cpu, instruction->a.reg, &dst);
		break;
	default:
		assert(false && "Unsupported 'dst' operand type");
		break;
	}
	OK_OR_RETURN(ret == 0, ret);

	switch (instruction->b.type) {
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER8:
		ret = cpu_register_read8(cpu, instruction->b.reg, &src8);
		break;
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER16:
		ret = cpu_register_read16(cpu, instruction->b.reg, &src);
		break;
	default:
		assert(false && "Unsupported 'src' operand type");
		break;
	}
	OK_OR_RETURN(ret == 0, ret);

	if (instruction->a.modifier == DECODED_MODIFIER_W16) {
		ret = mmu_write_byte(mmu, dst, src8);
		OK_OR_RETURN(ret == 0, ret);

		ret = cpu_register_write16(cpu, instruction->a.reg, dst - 1);
	} else {
		ret = mmu_read_byte(mmu, src, &dst8);
		OK_OR_RETURN(ret == 0, ret);

		ret = cpu_register_write8(cpu, instruction->a.reg, dst8);
		OK_OR_RETURN(ret == 0, ret);

		ret = cpu_register_write16(cpu, instruction->b.reg, dst - 1);
	}
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
	int ret = 0;
	uint16_t dst, src;
	uint8_t dst8, src8;
	struct cpu *cpu;

	cpu = &device->cpu;

	switch (instruction->a.type) {
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER8:
		ret = cpu_register_read8(cpu, instruction->a.reg, &dst8);
		break;
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER16:
		ret = cpu_register_read16(cpu, instruction->a.reg, &dst);
		break;
	default:
		assert(false && "XOR operand 'dst' not supported");
		ret = -EINVAL;
		break;
	}
	OK_OR_RETURN(ret == 0, ret);

	switch (instruction->b.type) {
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER8:
		ret = cpu_register_read8(cpu, instruction->b.reg, &src8);
		break;
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER16:
		ret = cpu_register_read16(cpu, instruction->b.reg, &src);
		break;
	default:
		assert(false && "XOR operand 'src' not supported");
		ret = -EINVAL;
		break;
	}
	OK_OR_RETURN(ret == 0, ret);

	switch (instruction->a.type) {
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER8:
		dst8 ^= src8;
		break;
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER16:
		dst ^= src;
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_RETURN(ret == 0, ret);

	switch (instruction->a.type) {
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER8:
		ret = cpu_register_write8(cpu, instruction->a.reg, dst8);
		break;
	case DECODED_INSTRUCTION_OPERAND_TYPE_REGISTER16:
		ret = cpu_register_write16(cpu, instruction->a.reg, dst);
		break;
	default:
		assert(false && "XOR operand 'dst' not supported");
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

	switch (instruction->info->isa_operation) {
	case ISA_OPERATION_ADC:
		ret = interpreter_execute_instruction_adc(device, instruction);
		break;
	case ISA_OPERATION_ADD:
		ret = interpreter_execute_instruction_add(device, instruction);
		break;
	case ISA_OPERATION_ADDC:
		ret = interpreter_execute_instruction_addc(device, instruction);
		break;
	case ISA_OPERATION_AND:
		ret = interpreter_execute_instruction_and(device, instruction);
		break;
	case ISA_OPERATION_CALL:
		ret = interpreter_execute_instruction_call(device, instruction);
		break;
	case ISA_OPERATION_CCF:
		ret = interpreter_execute_instruction_ccf(device, instruction);
		break;
	case ISA_OPERATION_CP:
		ret = interpreter_execute_instruction_cp(device, instruction);
		break;
	case ISA_OPERATION_CPL:
		ret = interpreter_execute_instruction_cpl(device, instruction);
		break;
	case ISA_OPERATION_DAA:
		ret = interpreter_execute_instruction_daa(device, instruction);
		break;
	case ISA_OPERATION_DEC:
		ret = interpreter_execute_instruction_dec(device, instruction);
		break;
	case ISA_OPERATION_DI:
		ret = interpreter_execute_instruction_di(device, instruction);
		break;
	case ISA_OPERATION_EI:
		ret = interpreter_execute_instruction_ei(device, instruction);
		break;
	case ISA_OPERATION_HALT:
		ret = interpreter_execute_instruction_halt(device, instruction);
		break;
	case ISA_OPERATION_INC:
		ret = interpreter_execute_instruction_inc(device, instruction);
		break;
	case ISA_OPERATION_JP:
		ret = interpreter_execute_instruction_jp(device, instruction);
		break;
	case ISA_OPERATION_JR:
		ret = interpreter_execute_instruction_jr(device, instruction);
		break;
	case ISA_OPERATION_LD:
		ret = interpreter_execute_instruction_ld(device, instruction);
		break;
	case ISA_OPERATION_LDD:
		ret = interpreter_execute_instruction_ldd(device, instruction);
		break;
	case ISA_OPERATION_LDH:
		ret = interpreter_execute_instruction_ldh(device, instruction);
		break;
	case ISA_OPERATION_LDHL:
		ret = interpreter_execute_instruction_ldhl(device, instruction);
		break;
	case ISA_OPERATION_LDI:
		ret = interpreter_execute_instruction_ldi(device, instruction);
		break;
	case ISA_OPERATION_NOP:
		ret = interpreter_execute_instruction_nop(device, instruction);
		break;
	case ISA_OPERATION_OR:
		ret = interpreter_execute_instruction_or(device, instruction);
		break;
	case ISA_OPERATION_POP:
		ret = interpreter_execute_instruction_pop(device, instruction);
		break;
	case ISA_OPERATION_PREFIX:
		ret = interpreter_execute_instruction_prefix(device, instruction);
		break;
	case ISA_OPERATION_PUSH:
		ret = interpreter_execute_instruction_push(device, instruction);
		break;
	case ISA_OPERATION_RET:
		ret = interpreter_execute_instruction_ret(device, instruction);
		break;
	case ISA_OPERATION_RETI:
		ret = interpreter_execute_instruction_reti(device, instruction);
		break;
	case ISA_OPERATION_RST:
		ret = interpreter_execute_instruction_rst(device, instruction);
		break;
	case ISA_OPERATION_SBC:
		ret = interpreter_execute_instruction_sbc(device, instruction);
		break;
	case ISA_OPERATION_SCF:
		ret = interpreter_execute_instruction_scf(device, instruction);
		break;
	case ISA_OPERATION_STOP:
		ret = interpreter_execute_instruction_stop(device, instruction);
		break;
	case ISA_OPERATION_SUB:
		ret = interpreter_execute_instruction_sub(device, instruction);
		break;
	case ISA_OPERATION_XOR:
		ret = interpreter_execute_instruction_xor(device, instruction);
		break;
	/* Start of prefix instructions */
	case ISA_OPERATION_BIT:
		ret = interpreter_execute_instruction_bit(device, instruction);
		break;
	case ISA_OPERATION_RES:
		ret = interpreter_execute_instruction_res(device, instruction);
		break;
	case ISA_OPERATION_RL:
		ret = interpreter_execute_instruction_rl(device, instruction);
		break;
	case ISA_OPERATION_RLA:
		ret = interpreter_execute_instruction_rla(device, instruction);
		break;
	case ISA_OPERATION_RLC:
		ret = interpreter_execute_instruction_rlc(device, instruction);
		break;
	case ISA_OPERATION_RLCA:
		ret = interpreter_execute_instruction_rlca(device, instruction);
		break;
	case ISA_OPERATION_RR:
		ret = interpreter_execute_instruction_rr(device, instruction);
		break;
	case ISA_OPERATION_RRA:
		ret = interpreter_execute_instruction_rra(device, instruction);
		break;
	case ISA_OPERATION_RRC:
		ret = interpreter_execute_instruction_rrc(device, instruction);
		break;
	case ISA_OPERATION_RRCA:
		ret = interpreter_execute_instruction_rrca(device, instruction);
		break;
	case ISA_OPERATION_SET:
		ret = interpreter_execute_instruction_set(device, instruction);
		break;
	case ISA_OPERATION_SLA:
		ret = interpreter_execute_instruction_sla(device, instruction);
		break;
	case ISA_OPERATION_SRA:
		ret = interpreter_execute_instruction_sra(device, instruction);
		break;
	case ISA_OPERATION_SRL:
		ret = interpreter_execute_instruction_srl(device, instruction);
		break;
	case ISA_OPERATION_SWAP:
		ret = interpreter_execute_instruction_swap(device, instruction);
		break;
	default:
		ret = -EINVAL;
		fprintf(stderr, "Encountered unexpected operation type '%02x'. (Emulator error?)\n", instruction->info->isa_operation);
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}
