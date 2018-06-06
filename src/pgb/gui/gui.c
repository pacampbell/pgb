#include <gtk/gtk.h>
#include <pgb/debug.h>
#include <pgb/gui/gui.h>

int start_emulating(const char *rom_path, const char *decoder_type);

static
void callback_start_emulation(GtkWidget *widget, gpointer user_data)
{
	int ret;
	struct pgb_app_config *config = user_data;

	ret = start_emulating(config->rom_path, config->decoder_type);
	OK_OR_WARN(ret == 0);
}

static
void gui_activate(GtkApplication *app, gpointer user_data)
{
	GtkWidget *window, *button, *button_box;

	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "PGB");
	gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

	button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_container_add(GTK_CONTAINER(window), button_box);

	button = gtk_button_new_with_label("Start");
	g_signal_connect(button, "clicked", G_CALLBACK(callback_start_emulation), user_data);
	gtk_container_add(GTK_CONTAINER(button_box), button);

	gtk_widget_show_all(window);
}

int gui_create_window(struct pgb_app_config *config)
{
	GtkApplication *app;

	app = gtk_application_new("org.pgb.gui", G_APPLICATION_FLAGS_NONE);

	g_signal_connect(app, "activate", G_CALLBACK(gui_activate), config);
	g_application_run(G_APPLICATION(app), 0, NULL);
	g_object_unref(app);

	return 0;
}