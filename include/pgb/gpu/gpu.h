#ifndef PGB_GPU_H
#define PGB_GPU_H

#include <stdint.h>

struct device;
struct mmu;

enum gpu_mode {
	GPU_MODE_HBLANK    = 0,
	GPU_MODE_VBLANK    = 1,
	GPU_MODE_OAM_READ  = 2,
	GPU_MODE_VRAM_READ = 3
};

struct gpu {
	enum gpu_mode mode;
	uint16_t clock;
};

int gpu_init(struct gpu *gpu);
int gpu_step(struct gpu *gpu, struct mmu *mmu, uint16_t t);
int gpu_destroy(struct gpu *gpu);

#endif /* PGB_GPU_H */
