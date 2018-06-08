#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "segment_dir.h"

int segment_dir(SEGMENT* segment) {
	char* pwd = getenv("PWD");
	char* home = getenv("HOME");

	segment->text = (char*)malloc(strlen(pwd));
	memset(segment->text, 0, strlen(pwd));

	if (strncmp(pwd, home, strlen(home)) == 0) {
		sprintf(segment->text, "%s%s", icons[HOME], pwd + strlen(home));
	}
	else {
		sprintf(segment->text, "%s", pwd);
	}

	return 0;
}
