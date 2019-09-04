#ifndef PGB_CPU_CPU_H
#define PGB_CPU_CPU_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <pgb/cpu/clock.h>
#include <pgb/cpu/decoder.h>
#include <pgb/cpu/registers.h>

struct device;
struct decoded_instruction;
struct mmu;
enum instruction_operand;

struct cpu_status {
	bool halted;
};

struct cpu {
	struct registers registers;
	struct clock clock;
	struct cpu_status status;
	struct decoder decoder;
};

int cpu_step(struct device *device, size_t step, size_t *instructions_stepped);
int cpu_init(struct cpu *cpu, const char *decoder_str);
int cpu_destroy(struct cpu *cpu);
bool cpu_is_halted(struct cpu *cpu);
void cpu_dump_register_state(struct cpu *cpu);

int fetch(struct device *device, uint8_t *opcode, bool *found_prefix);
int decode(struct device *device, uint8_t opcode, bool is_prefix, struct decoded_instruction *decoded_instruction);

int cpu_register_read8(struct cpu *cpu, enum instruction_operand reg, uint8_t *value);
int cpu_register_write8(struct cpu *cpu, enum instruction_operand reg, uint8_t value);
int cpu_register_read16(struct cpu *cpu, enum instruction_operand reg, uint16_t *value);
int cpu_register_write16(struct cpu *cpu, enum instruction_operand reg, uint16_t value);

#endif /* PGB_CPU_CPU_H */
