#ifndef PGB_TUI_BUFFER_H
#define PGB_TUI_BUFFER_H

#include <sys/queue.h>

struct pgb_tui_buffer_line {
	TAILQ_ENTRY(pgb_tui_buffer_line) entries;
	const char *line;
};

TAILQ_HEAD(pgb_tui_buffer, pgb_tui_buffer_line);

int buffer_init(struct pgb_tui_buffer **buffer);
int buffer_insert_line(struct pgb_tui_buffer *buffer, const char *line, int offset);
int buffer_append_line(struct pgb_tui_buffer *buffer, const char *line);
int buffer_remove_line(struct pgb_tui_buffer *buffer, unsigned offset);
int buffer_get_line(struct pgb_tui_buffer *buffer, int offset, const char **line);

#endif /* PGB_TUI_BUFFER_H */
