#include <stdio.h>
#include <stdlib.h>
#include <pgb/banner.h>

/* XXX: Learn this path dynamically? */
const char elf_interpreter[] __attribute__((section(".interp"))) = "/lib64/ld-linux-x86-64.so.2";

static
const char *banner_text =
	LIB_NAME " - " LIB_AUTHOR " (" LIB_BUILD_DATE ")" "\n"
	"CC = " CC_VERSION "\n"
	"GIT = " GIT_BRANCH_NAME ", " GIT_HEAD_HASH "\n"
	"BUILD_OPTIONS = " LIB_BUILD_FLAGS "\n"
	"LIBS = " LIB_BUILD_DEPS "\n"
	"\n"
	"All bugs, feature requests, pull requests, etc. should be directed to " GIT_REPO_LOCATION ".\n"
	"";

void lib_main()
{
	printf("%s", banner_text);

	exit(0);
}
