#ifndef PGBA_CPU_INTERPRETER_H
#define PGBA_CPU_INTERPRETER_H

struct device;
struct decoded_instruction;

int interpreter_execute_instruction(struct device *device, struct decoded_instruction *instruction);

#endif /* PGBA_CPU_INTERPRETER_H */
