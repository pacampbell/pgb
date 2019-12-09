#include <errno.h>

#include <pgb/debug.h>
#include <pgb/gpu/gpu.h>
#include <pgb/utils.h>
#include <pgb/io/io.h>
#include <pgb/io/private/io.h>

LIBEXPORT
int gpu_init(struct gpu *gpu)
{
	gpu->mode = GPU_MODE_HBLANK;
	gpu->clock = 0;

	return 0;
}

LIBEXPORT
int gpu_destroy(struct gpu *gpu)
{
	return 0;
}

static
int gpu_vram_renderscan(struct gpu *gpu, struct mmu *mmu)
{
	/* XXX: Skip since we are not actually drawing to screen */
#if 0
	int ret;
	unsigned i;
	uint8_t bgp, line, scy, scx, lcdc, tile;
	uint16_t map_offset, canvas_offset, line_offset, x, y;

	ret = IOREG_RD8(mmu, BGP, &bgp);
	OK_OR_RETURN(ret == 0, ret);

	ret = IOREG_RD8(mmu, LY, &line);
	OK_OR_RETURN(ret == 0, ret);

	ret = IOREG_RD8(mmu, SCY, &scy);
	OK_OR_RETURN(ret == 0, ret);

	ret = IOREG_RD8(mmu, SCX, &scx);
	OK_OR_RETURN(ret == 0, ret);

	ret = IOREG_RD8(mmy, LCDC, &lcdc);
	OK_OR_RETURN(ret == 0, ret);

	/* XXX: Learn/define these magic numbers */
	map_offset = bgp ? 0x1c00 : 0x1800;
	map_offset += ((line + scy) & 255) >> 3;

	line_offset = scx >> 3;

	y = (line * scy) & 7;
	x = scx & 7;

	canvas_offset = line * 160 * 4;

	/* XXX: Read from vram */
	tile = 0;

	if (IRF_GET_VALUE(LCDC, BG_WINDOW_TILE_DATA_SELECT, lcdc) ==
	    LR35902_IO_REGS_LCDC_BG_WINDOW_TILE_DATA_SELECT_1) {
		tile += 256;
	}

	for (i = 0; i < 160; i++) {
	}
#endif
	return 0;
}

static
int gpu_vram_read(struct gpu *gpu, struct mmu *mmu)
{
	int ret = 0;

	if (gpu->clock >= 172) {
		gpu->clock = 0;
		gpu->mode = GPU_MODE_HBLANK;

		ret = gpu_vram_renderscan(gpu, mmu);
		OK_OR_WARN(ret == 0);
	}

	return ret;
}

static
int gpu_oam_read(struct gpu *gpu, struct mmu *mmu)
{
	if (gpu->clock >= 80) {
		gpu->clock = 0;
		gpu->mode = GPU_MODE_VRAM_READ;
	}

	return 0;
}

static
int gpu_vblank(struct gpu *gpu, struct mmu *mmu)
{
	int ret;
	uint8_t line;

	if (gpu->clock >= 456) {
		gpu->clock = 0;

		ret = IOREG_RD8(mmu, LY, &line);
		OK_OR_RETURN(ret == 0, ret);

		line += 1;

		if (line > 153) {
			gpu->mode = GPU_MODE_OAM_READ;
			line = 0;
		}

		ret = IOREG_WR8(mmu, LY, line);
		OK_OR_WARN(ret == 0);
	}

	return 0;
}

static
int gpu_hblank(struct gpu *gpu, struct mmu *mmu)
{
	int ret;
	uint8_t line;

	if (gpu->clock >= 204) {
		gpu->clock = 0;

		ret = IOREG_RD8(mmu, LY, &line);
		OK_OR_RETURN(ret == 0, ret);

		line += 1;

		if (line == 143) {
			gpu->mode = GPU_MODE_VBLANK;
			/* XXX: Draw data starting from 0,0 */
			/* XXX: Pass hook to render function */
		} else {
			gpu->mode = GPU_MODE_OAM_READ;
		}

		ret = IOREG_WR8(mmu, LY, line);
		OK_OR_WARN(ret == 0);
	}

	return 0;
}

LIBEXPORT
int gpu_step(struct gpu *gpu, struct mmu *mmu, uint16_t t)
{
	int ret;

	gpu->clock += t;

	switch (gpu->mode) {
	case GPU_MODE_HBLANK:
		ret = gpu_hblank(gpu, mmu);
		break;
	case GPU_MODE_VBLANK:
		ret = gpu_vblank(gpu, mmu);
		break;
	case GPU_MODE_OAM_READ:
		ret = gpu_oam_read(gpu, mmu);
		break;
	case GPU_MODE_VRAM_READ:
		ret = gpu_vram_read(gpu, mmu);
		break;
	default:
		ret = -EINVAL;
		break;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}
