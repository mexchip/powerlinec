#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "segment_dir.h"

SEGMENT segment_dir() {
	SEGMENT segment = {.text = NULL, .background = 0, .foreground = 0};
	char* pwd = getenv("PWD");
	char* home = getenv("HOME");

	segment.text = (char*)malloc(strlen(pwd));
	memset(segment.text, 0, strlen(pwd));

	if (strncmp(pwd, home, strlen(home)) == 0) {
		sprintf(segment.text, "%s%s", icons[HOME], pwd + strlen(home));
	}
	else {
		sprintf(segment.text, "%s", pwd);
	}

	return segment;
}