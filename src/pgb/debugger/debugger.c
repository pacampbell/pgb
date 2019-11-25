#include <errno.h>
#include <string.h>

#include <pgb/cpu/cpu.h>
#include <pgb/cpu/decoder.h>
#include <pgb/cpu/instruction_info.h>
#include <pgb/debug.h>
#include <pgb/debugger/debugger.h>
#include <pgb/device/device.h>
#include <pgb/io/io.h>
#include <pgb/io/private/io.h>
#include <pgb/utils.h>

enum operand_option {
	OPERAND_OPTION_A,
	OPERAND_OPTION_B
};

#define CHOOSE_VALUE(operand, decoded_instruction, TYPE) ((operand) == OPERAND_OPTION_A ? (decoded_instruction).a.TYPE : (decoded_instruction).b.TYPE)

static
int parse_u16(uint16_t value, char **__str)
{
	int size;
	char *str;

	size = snprintf(NULL, 0, "; 0x%04x", value);
	size += 1;

	str = calloc(size, sizeof(char));
	OK_OR_RETURN(str != NULL, -ENOMEM);

	sprintf(str, "; 0x%04x", value);

	*__str = str;

	return 0;
}

static
int parse_i16(int16_t value, char **__str)
{
	int size;
	char *str;

	size = snprintf(NULL, 0, "; %d", value);
	size += 1;

	str = calloc(size, sizeof(char));
	OK_OR_RETURN(str != NULL, -ENOMEM);

	sprintf(str, "; %d", value);

	*__str = str;

	return 0;
}

static
int literal_to_string(enum operand_option operand, struct decoded_instruction *decoded_instruction, enum instruction_operand_type operand_type, char **comment)
{
	int ret = 0;

	switch (operand_type) {
	case INSTRUCTION_OPERAND_TYPE_I8:
	case INSTRUCTION_OPERAND_TYPE_I16:
		ret = parse_i16(CHOOSE_VALUE(operand, *decoded_instruction, i8), comment);
		break;
	case INSTRUCTION_OPERAND_TYPE_U3:
	case INSTRUCTION_OPERAND_TYPE_U8:
	case INSTRUCTION_OPERAND_TYPE_U16:
		ret = parse_u16(CHOOSE_VALUE(operand, *decoded_instruction, u16), comment);
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

LIBEXPORT
int debugger_fetch_instructions_from_address(struct device *device, struct debugger_info *info, size_t n, size_t *num_decoded, uint16_t address)
{
	int ret;
	uint16_t pc_saved;

	pc_saved = device->cpu.registers.pc;
	device->cpu.registers.pc = address;

	ret = debugger_fetch_instructions(device, info, n, num_decoded);
	OK_OR_WARN(ret == 0);

	device->cpu.registers.pc = pc_saved;

	return ret;
}

LIBEXPORT
int debugger_fetch_instructions(struct device *device, struct debugger_info *info, size_t n, size_t *num_decoded)
{
	int ret;
	size_t i;
	bool is_prefix;
	uint8_t opcode;
	uint16_t pc_saved, address;
	struct decoded_instruction decoded_instruction;
	char *comment_a, *comment_b;

	pc_saved = device->cpu.registers.pc;

	for (i = 0; i < n; i++) {
		memset(&decoded_instruction, 0, sizeof(decoded_instruction));
		comment_a = NULL;
		comment_b = NULL;

		address = device->cpu.registers.pc;

		ret = fetch(device, &opcode, &is_prefix);
		OK_OR_BREAK(ret == 0);

		ret = decode(device, opcode, is_prefix, &decoded_instruction);
		OK_OR_BREAK(ret == 0);

		info[i].assembly = decoded_instruction.info->assembly;
		info[i].address = address;
		info[i].num_bytes = decoded_instruction.info->num_bytes;
		memcpy(info[i].raw_data, decoded_instruction.raw_data, info[i].num_bytes);

		switch (decoded_instruction.info->operands.a.type) {
		case INSTRUCTION_OPERAND_TYPE_I8:
		case INSTRUCTION_OPERAND_TYPE_I16:
		case INSTRUCTION_OPERAND_TYPE_U3:
		case INSTRUCTION_OPERAND_TYPE_U8:
		case INSTRUCTION_OPERAND_TYPE_U16:
			ret = literal_to_string(OPERAND_OPTION_A, &decoded_instruction, decoded_instruction.info->operands.a.type, &comment_a);
			break;
		default:
			ret = 0;
			break;
		}
		OK_OR_BREAK(ret == 0);

		switch (decoded_instruction.info->operands.b.type) {
		case INSTRUCTION_OPERAND_TYPE_I8:
		case INSTRUCTION_OPERAND_TYPE_I16:
		case INSTRUCTION_OPERAND_TYPE_U3:
		case INSTRUCTION_OPERAND_TYPE_U8:
		case INSTRUCTION_OPERAND_TYPE_U16:
			ret = literal_to_string(OPERAND_OPTION_B, &decoded_instruction, decoded_instruction.info->operands.b.type, &comment_b);
			break;
		default:
			ret = 0;
			break;
		}
		OK_OR_BREAK(ret == 0);

		info[i].comments.a = comment_a;
		info[i].comments.b = comment_b;
	}

	*num_decoded = i;

	device->cpu.registers.pc = pc_saved;

	return 0;
}

LIBEXPORT
void free_debugger_info(struct debugger_info *info, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++) {
		free(info[i].comments.a);
		free(info[i].comments.b);
	}
}

#define IO_REGISTER(NAME) \
	{#NAME, LR35902_IO_REGS_##NAME}

struct io_register_map {
	const char *name;
	uint16_t address;
} io_register_mappings[] = {
	IO_REGISTER(P1),
	IO_REGISTER(SB),
	IO_REGISTER(SC),
	IO_REGISTER(DIV),
	IO_REGISTER(TIMA),
	IO_REGISTER(TMA),
	IO_REGISTER(TAC),
	IO_REGISTER(IF),
	IO_REGISTER(NR10),
	IO_REGISTER(NR11),
	IO_REGISTER(NR12),
	IO_REGISTER(NR13),
	IO_REGISTER(NR14),
	IO_REGISTER(NR21),
	IO_REGISTER(NR22),
	IO_REGISTER(NR23),
	IO_REGISTER(NR24),
	IO_REGISTER(NR30),
	IO_REGISTER(NR31),
	IO_REGISTER(NR32),
	IO_REGISTER(NR33),
	IO_REGISTER(NR34),
	IO_REGISTER(NR41),
	IO_REGISTER(NR42),
	IO_REGISTER(NR43),
	IO_REGISTER(NR44),
	IO_REGISTER(NR50),
	IO_REGISTER(NR51),
	IO_REGISTER(NR52),
	IO_REGISTER(LCDC),
	IO_REGISTER(STAT),
	IO_REGISTER(SCY),
	IO_REGISTER(SCX),
	IO_REGISTER(LY),
	IO_REGISTER(LYC),
	IO_REGISTER(DMA),
	IO_REGISTER(BGP),
	IO_REGISTER(OBP0),
	IO_REGISTER(OBP1),
	IO_REGISTER(WY),
	IO_REGISTER(WX),
	IO_REGISTER(IE)
};

LIBEXPORT
int debugger_io_register_read(struct device *device, uint16_t address, uint8_t *__value)
{
	int ret;
	unsigned i;
	uint8_t value;
	bool found = false;
	struct mmu *mmu = &device->mmu;
	struct io_register_map *map;

	for (i = 0; i < ARRAY_SIZE(io_register_mappings); i++) {
		map = &io_register_mappings[i];
		if (map->address == address) {
			found = true;
			break;
		}
	}

	OK_OR_RETURN(found, -EINVAL);

	ret = mmu_read8(mmu, address, &value);
	OK_OR_RETURN(ret == 0, ret);

	*__value = value;

	return 0;
}

LIBEXPORT
int debugger_io_register_address_to_name(uint16_t address, const char **__name)
{
	unsigned i;
	const char *name = NULL;
	struct io_register_map *map;

	for (i = 0; i < ARRAY_SIZE(io_register_mappings); i++) {
		map = &io_register_mappings[i];
		if (map->address == address) {
			name = map->name;
			break;
		}
	}

	OK_OR_RETURN(name != NULL, -EINVAL);

	*__name = name;

	return 0;
}

LIBEXPORT
int debugger_io_register_listing(struct device *device,
				 struct debugger_io_register_info **_io_register_listing,
				 size_t *n)
{
	int ret = 0;
	unsigned i;
	uint8_t value;
	struct mmu *mmu = &device->mmu;
	struct debugger_io_register_info *io_register_listing;

	io_register_listing = malloc(ARRAY_SIZE(io_register_mappings) * sizeof(*io_register_listing));
	OK_OR_RETURN(io_register_listing != NULL, -ENOMEM);

	for (i = 0; i < ARRAY_SIZE(io_register_mappings); i++) {
		ret = mmu_read8(mmu, io_register_mappings[i].address, &value);
		OK_OR_GOTO(ret == 0, free_and_exit);

		io_register_listing[i].address = io_register_mappings[i].address;
		io_register_listing[i].name = io_register_mappings[i].name;
		io_register_listing[i].value = value;
	}

	*n = ARRAY_SIZE(io_register_mappings);
	*_io_register_listing = io_register_listing;

free_and_exit:
	if (ret != 0)
		free(io_register_listing);

	return ret;
}

LIBEXPORT
void debugger_io_register_free(struct debugger_io_register_info *io_register_listing)
{
	free(io_register_listing);
}
