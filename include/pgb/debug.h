#ifndef PGD_DEBUG_H
#define PGD_DEBUG_H

#include <stdio.h>

#ifdef DEBUG
#define IS_DEBUG() 1
#else
#define IS_DEBUG() 0
#endif

#define OK_OR_WARN(cond) do {									\
	if (!(cond)) {										\
		fprintf(stderr, "%s:%s:%u !(%s)\n" , __FILE__, __FUNCTION__, __LINE__, #cond);	\
	}											\
} while (0)

#define OK_OR_RETURN(cond, ret) do {	\
	if (!(cond)) {			\
		OK_OR_WARN(cond);	\
		return ret;		\
	}				\
} while (0)

#define OK_OR_RETURN_VOID(cond) do {	\
	if (!(cond)) {			\
		OK_OR_WARN(cond);	\
		return;			\
	}				\
} while (0)

#define OK_OR_GOTO(cond, label) do {	\
	if (!(cond)) {			\
		OK_OR_WARN(cond);	\
		goto label;		\
	}				\
} while (0)

#define OK_OR_BREAK(cond)		\
	if (!(cond)) {			\
		OK_OR_WARN(cond);	\
		break;			\
	}

#define OK_OR_CONTINUE(cond)		\
	if (!(cond)) {			\
		OK_OR_WARN(cond);	\
		continue;		\
	}

#endif /* PGD_DEBUG_H */
