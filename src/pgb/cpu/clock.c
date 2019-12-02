#include <pgb/cpu/clock.h>
#include <pgb/utils.h>

LIBEXPORT
int clock_init(struct clock *clock)
{
	clock->t = 0;
	clock->m = 0;

	return 0;
}
