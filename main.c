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

	// for (int i = 0; i < MAX_ICON; i++) {
	// 	printf("%s\n", icons[i]);
	// }

	SEGMENT segment = create_segment("dir:blue:black");
	draw_segment(&segment);
	delete_segment(&segment);

	// SEGMENT segment = create_segment("git:yellow:black");
	// draw_segment(&segment);
	// delete_segment(&segment);

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

	printf("\x1b[0m\n");
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
