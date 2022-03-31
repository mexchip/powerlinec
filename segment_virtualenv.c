#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "segment_virtualenv.h"

int segment_virtualenv(SEGMENT* segment) {
    char* virtualenv = getenv("VIRTUAL_ENV");

    if (NULL != virtualenv) {
        // use the lowest level directory for text segment
        size_t index = strlen(virtualenv) - 1;
        for (; index >= 0 && virtualenv[index] != '/'; index--);
        size_t length = strlen(virtualenv) - index + 1;
        segment->text = (char*)malloc(length);
        memset(segment->text, 0, length);

        strcpy(segment->text, virtualenv + index + 1);
    }

	return 0;
}