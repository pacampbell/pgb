#include <errno.h>
#include <string.h>

#include <pgb/cpu/decoder.h>
#include <pgb/cpu/private/logical_decoder.h>
#include <pgb/cpu/private/table_decoder.h>
#include <pgb/debug.h>

int string_to_decoder_type(const char *str, enum decoder_type *type)
{
	int ret = 0;

	if (strcmp(str, "table") == 0) {
		*type  = DECODER_TYPE_TABLE;
	} else if (strcmp(str, "logical") == 0) {
		*type  = DECODER_TYPE_LOGICAL;
	} else {
		ret = -EINVAL;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}

int cpu_decoder_configure_decoder(enum decoder_type type, struct decoder *decoder)
{
	int ret = 0;

	switch (type) {
	case DECODER_TYPE_LOGICAL:
		decoder->type = DECODER_TYPE_LOGICAL;
		decoder->decode = cpu_logical_decoder_decode;
		break;
	case DECODER_TYPE_TABLE:
		decoder->type = DECODER_TYPE_TABLE;
		decoder->decode = cpu_table_decoder_decode;
		break;
	default:
		ret = -EINVAL;
	}
	OK_OR_WARN(ret == 0);

	return ret;
}