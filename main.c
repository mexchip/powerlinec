#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERSION "0.3"

#define DEFINE_GLOBALS
#include "powerlinec.h"
#include "segments.h"

const size_t color_names_size = sizeof(color_names);

SEGMENT create_segment(char* description);
void delete_segment(SEGMENT* segment);
void draw_segment(SEGMENT* segment);
int get_color_index(char* color);
int get_segment_index(char* segment);
void draw_separator(int background, int foreground);

int main(int argc, char* argv[]) {
	if (1 >= argc) {
		printf("ERROR: no arguments provided.\n");
		exit(1);
	}

	if (0 == strcmp("--version", argv[1])) {
		printf("powerlinec %s\n", VERSION);
		exit(0);
	}

	SEGMENT old;
	int drawn = 0;
	int flag = 0;
	for (int i = 1; i < argc; i++) {
		SEGMENT current = create_segment(argv[i]);
		if (NULL != current.text) {
			if (flag >= 1) {
				draw_separator(current.background, old.background - 10);
			}
			draw_segment(&current);
			drawn++;
			old.foreground = current.foreground;
			old.background = current.background;
			flag = 1;
		}
		else {
			flag = 0;
		}
		delete_segment(&current);
	}

	if (0 < drawn) {
		draw_separator(0, old.background - 10);
		printf("\\[\x1b[0m\\] ");
	}

	return 0;
}

SEGMENT create_segment(char* description) {
	char* copy = strdup(description);

	int segment_index = -1, background, foreground;

	char* token;

	token = strtok(copy, ":");
	if (NULL != token)
		segment_index = get_segment_index(token);

	token = strtok(NULL, ":");
	if (NULL != token)
		background = get_color_index(token) + 40;

	token = strtok(NULL, ":");
	if (NULL != token)
		foreground = get_color_index(token) + 30;

	SEGMENT segment = {.text = NULL, .background = background, .foreground = foreground};
	if (segment_index >= 0) {
		int res = (*available_segments[segment_index])(&segment);
	}

	return segment;
}

void delete_segment(SEGMENT* segment) {
	if (NULL != segment->text)
		free(segment->text);

	segment->foreground = 0;
	segment->background = 0;
}

void draw_segment(SEGMENT* segment) {
	printf("\\[\x1b[%dm\\]", segment->background);
	printf("\\[\x1b[%dm\\]", segment->foreground);

	printf(" %s ", segment->text);
}

int get_color_index(char* color) {
	size_t size = sizeof(color_names) / sizeof(*color_names);

	for (int i = 0; i < size; i++) {
		if (strcmp(color, color_names[i]) == 0) {
			return i;
		}
	}

	return 0;
}

int get_segment_index(char* segment) {
	size_t size = sizeof(segment_names) / sizeof(*segment_names);

	for (int i = 0; i < size; i++) {
		if (strcmp(segment, segment_names[i]) == 0) {
			return i;
		}
	}

	return -1;
}

void draw_separator(int background, int foreground) {
	printf("\\[\x1b[%dm\\]", background);
	printf("\\[\x1b[%dm\\]", foreground);

	printf("%s", icons[SEPARATOR]);
}
