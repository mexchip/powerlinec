#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <git2.h>

#include "powerlinec.h"
#include "segments.h"

SEGMENT create_segment(char* description);
void delete_segment(SEGMENT* segment);
void draw_segment(SEGMENT* segment);
int get_color_index(char* color);
int get_segment_index(char* segment);

SEGMENT (*available_segments[sizeof(segment_names)])(void);

int main(int argc, char* argv[]) {
	available_segments[0] = segment_dir;
	available_segments[1] = segment_git;

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

SEGMENT segment_dir() {
	SEGMENT segment;
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

SEGMENT segment_git() {
	SEGMENT	 segment;

	int error = 0;
	git_libgit2_init();
	git_repository* repo = NULL;
	
	error = git_repository_open(&repo, "./");
	if (0 == error) {
		const char* branch = NULL;
		git_reference* head = NULL;
		error = git_repository_head(&head, repo);
		if (0 == error) {
			branch = git_reference_shorthand(head);
			int length = strlen(branch) + strlen(icons[GIT]) + 3;
			segment.text = (char*)malloc(length);
			memset(segment.text, 0, length);
			sprintf(segment.text, "%s %s", icons[GIT], branch);
			
			git_reference_free(head);
		}

		git_repository_free(repo);
	}

	git_libgit2_shutdown();

	return segment;
}
