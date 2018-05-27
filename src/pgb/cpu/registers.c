#include "pgb/cpu/registers.h"

void registers_init(struct registers *registers)
{
	registers->a = 0x0;
	registers->b = 0x0;
	registers->c = 0x0;
	registers->d = 0x0;
	registers->e = 0x0;
	registers->h = 0x0;
	registers->l = 0x0;

	registers->pc = 0x0;
	registers->sp = 0x0;

	registers->flags.rsvd = 0;
	registers->flags.c = 0;
	registers->flags.h = 0;
	registers->flags.n = 0;
	registers->flags.z = 0;
}