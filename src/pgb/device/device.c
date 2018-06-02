#include <pgb/debug.h>
#include <pgb/utils.h>
#include <pgb/device/device.h>


int device_init(struct device *device, const char *decoder_type)
{
	int ret;

	ret = cpu_init(&device->cpu, decoder_type);
	OK_OR_RETURN(ret == 0, ret);

	ret = mmu_init(&device->mmu);
	OK_OR_RETURN(ret == 0, ret);

	return 0;
}

int device_destroy(struct device *device)
{
	cpu_destroy(&device->cpu);
	mmu_destroy(&device->mmu);

	return 0;
}

int device_emulate(struct device *device, const char *rom_path)
{
	int ret;
	size_t stepped_instructions, num_instructions = 0;

	if (IS_DEBUG()) {
		cpu_dump_register_state(&device->cpu);
	}

	ret = cpu_load_rom_from_file(&device->cpu, rom_path);
	OK_OR_RETURN(ret == 0, ret);

	do {
		ret = cpu_step(device, 1, &stepped_instructions);
		OK_OR_BREAK(ret == 0);

		num_instructions += stepped_instructions;
	} while (!cpu_is_halted(&device->cpu));

	return ret;
}
