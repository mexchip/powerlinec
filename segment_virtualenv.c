#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "segment_virtualenv.h"

int segment_virtualenv(SEGMENT* segment) {
    char* virtualenv = getenv("VIRTUAL_ENV");

    if (NULL != virtualenv) {
        int index = strlen(virtualenv - 1);
        for (; index >= 0 && virtualenv[index] != '/'; index--);
        int length = strlen(virtualenv + index + 1);
        segment->text = (char*)malloc(length);
        memset(segment->text, 0, length);

        strcpy(segment->text, virtualenv + index + 1);
    }

	return 0;
}