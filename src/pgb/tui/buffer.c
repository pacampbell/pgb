#include <errno.h>
#include <stdlib.h>

#include <pgb/debug.h>
#include <pgb/tui/buffer.h>
#include <pgb/utils.h>

int buffer_insert_line(struct pgb_tui_buffer *buffer, const char *line, int offset)
{
	return -ENOTSUP;
}

int buffer_append_line(struct pgb_tui_buffer *buffer, const char *line)
{
	struct pgb_tui_buffer_line *buffer_line;

	buffer_line = malloc(sizeof(buffer_line));
	OK_OR_RETURN(buffer_line != NULL, -ENOMEM);

	buffer_line->line = line;

	TAILQ_INSERT_TAIL(buffer, buffer_line, entries);

	return 0;
}

int buffer_remove_line(struct pgb_tui_buffer *buffer, unsigned offset)
{
	int i;
	struct pgb_tui_buffer_line *p;

	OK_OR_RETURN(!TAILQ_EMPTY(buffer), -EINVAL);

	i = 0;
	TAILQ_FOREACH(p, buffer, entries) {
		if (i == offset) {
			break;
		}
	}

	if (p == NULL)
		return -EINVAL;

	TAILQ_REMOVE(buffer, p, entries);
	free(p);

	return 0;
}

int buffer_init(struct pgb_tui_buffer **buffer)
{
	OK_OR_RETURN(buffer != NULL, -EINVAL);

	*buffer = malloc(sizeof(**buffer));
	OK_OR_RETURN(*buffer != NULL, -ENOMEM);

	TAILQ_INIT(*buffer);

	return 0;
}

int buffer_get_line(struct pgb_tui_buffer *buffer, int offset, const char **line)
{
	return -ENOTSUP;
}
