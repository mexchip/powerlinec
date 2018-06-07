#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <git2.h>

#define DEFINE_GLOBALS
#include "powerlinec.h"
#include "segments.h"

const size_t color_names_size = sizeof(color_names);

SEGMENT create_segment(char* description);
void delete_segment(SEGMENT* segment);
void draw_segment(SEGMENT* segment);
int get_color_index(char* color);
int get_segment_index(char* segment);

int main(int argc, char* argv[]) {
	if (1 >= argc) {
		printf("ERROR: no arguments provided\n");
		exit(1);
	}

	for (int i = 1; i < argc; i++) {
		SEGMENT current = create_segment(argv[i]);
		draw_segment(&current);
		delete_segment(&current);
	}

	return 0;
}

SEGMENT create_segment(char* description) {
	char* copy = strdup(description);

	int segment_index;

	char* token;

	token = strtok(copy, ":");
	segment_index = get_segment_index(token);
	SEGMENT segment = (*available_segments[segment_index])();

	token = strtok(NULL, ":");
	if (NULL != token)
		segment.background = 40 + get_color_index(token);
	else
		segment.background = -1;

	token = strtok(NULL, ":");
	if (NULL != token)
		segment.foreground = 30 + get_color_index(token);
	else
		segment.foreground = -1;

	return segment;
}

void delete_segment(SEGMENT* segment) {
	if (NULL != segment->text)
		free(segment->text);

	segment->foreground = 0;
	segment->background = 0;
}

void draw_segment(SEGMENT* segment) {
	printf("\x1b[%dm", segment->background);
	printf("\x1b[%dm", segment->foreground);

	printf(" %s ", segment->text);
}

int get_color_index(char* color) {
	for (int i = 0; i < sizeof(color_names); i++) {
		if (strcmp(color, color_names[i]) == 0) {
			return i;
		}
	}

	return 0;
}

int get_segment_index(char* segment) {
	for (int i = 0; i < sizeof(segment_names); i++) {
		if (strcmp(segment, segment_names[i]) == 0) {
			return i;
		}
	}

	return -1;
}
