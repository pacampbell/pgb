#ifndef PGBA_CPU_CPU_H
#define PGBA_CPU_CPU_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "clock.h"
#include "registers.h"

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
};

int cpu_load_rom(struct cpu *cpu, uint8_t *data, size_t size);
int cpu_load_rom_from_file(struct cpu *cpu, const char *path);
int cpu_step(struct cpu *cpu, size_t step, size_t *instructions_stepped);
int cpu_init(struct cpu *cpu);
int cpu_destroy(struct cpu *cpu);
bool cpu_is_halted(struct cpu *cpu);
void cpu_dump_register_state(struct cpu *cpu);

#endif /* PGBA_CPU_CPU_H */
