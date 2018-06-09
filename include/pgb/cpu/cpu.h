#ifndef PGBA_CPU_CPU_H
#define PGBA_CPU_CPU_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <pgb/cpu/clock.h>
#include <pgb/cpu/decoder.h>
#include <pgb/cpu/registers.h>

struct device;
enum instruction_operand;

struct cpu_status {
	bool halted;
};

struct rom_image {
	uint8_t *data;
	size_t size;
};

struct cpu {
	struct registers registers;
	struct clock clock;
	struct cpu_status status;
	struct rom_image rom_image;
	struct decoder decoder;
};

int cpu_load_rom(struct cpu *cpu, uint8_t *data, size_t size);
int cpu_load_rom_from_file(struct cpu *cpu, const char *path);
int cpu_step(struct device *device, size_t step, size_t *instructions_stepped);
int cpu_init(struct cpu *cpu, const char *decoder_str);
int cpu_destroy(struct cpu *cpu);
bool cpu_is_halted(struct cpu *cpu);
void cpu_dump_register_state(struct cpu *cpu);

int cpu_register_read8(struct cpu *cpu, enum instruction_operand reg, uint8_t *value);
int cpu_register_write8(struct cpu *cpu, enum instruction_operand reg, uint8_t value);
int cpu_register_read16(struct cpu *cpu, enum instruction_operand reg, uint16_t *value);
int cpu_register_write16(struct cpu *cpu, enum instruction_operand reg, uint16_t value);

#endif /* PGBA_CPU_CPU_H */
