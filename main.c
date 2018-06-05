#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* color_names[] = {
	"black",
	"red",
	"green",
	"yellow",
	"blue",
	"magenta",
	"cyan",
	"cyan",
	"white"
};

char* segment_names[] = {
	"dir"
};

typedef enum {
	SEPARATOR = 0,
	HOME,
	UBUNTU,
	MAX_ICON
} ICON;

typedef struct {
	char* text;
	int foreground;
	int background;
} SEGMENT;

char* icons[] = {
	"\ue0b0", // separator
	"\uf015", // home
	"\uf30c"  // ubuntu
};

SEGMENT create_segment(char* description);
void delete_segment(SEGMENT* segment);
void draw_segment(SEGMENT* segment);
int get_color_index(char* color);
int get_segment_index(char* segment);

SEGMENT segment_dir();

SEGMENT (*available_segments[1])(void);

int main(int argc, char* argv[]) {
	available_segments[0] = segment_dir;

	for (int i = 0; i < MAX_ICON; i++) {
		printf("%s\n", icons[i]);
	}

	SEGMENT segment = create_segment("dir:blue:black");
	draw_segment(&segment);
	delete_segment(&segment);

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
	free(segment->text);
	segment->foreground = 0;
	segment->background = 0;
}

void draw_segment(SEGMENT* segment) {
	printf("\x1b[48;%dm", segment->background);
	printf("\x1b[38;%dm", segment->foreground);

	printf("%s", segment->text);

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

SEGMENT segment_dir() {
	SEGMENT segment;
	char* pwd = getenv("PWD");
	char* home = getenv("HOME");

	segment.text = (char*)malloc(strlen(pwd));
	memset(segment.text, 0, strlen(pwd));

	if (strncmp(pwd, home, strlen(home)) == 0) {
		sprintf(segment.text, " %s%s ", icons[HOME], pwd + strlen(home));
	}
	else {
		sprintf(segment.text, " %s ", pwd);
	}

	return segment;
}