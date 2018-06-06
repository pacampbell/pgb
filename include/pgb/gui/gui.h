#ifndef PGB_GUI_GUI_H
#define PGB_GUI_GUI_H

struct pgb_app_config {
	const char *rom_path;
	const char *decoder_type;
};

int gui_create_window(struct pgb_app_config *config);

#endif /* PGB_GUI_GUI_H */