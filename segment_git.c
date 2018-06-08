#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <git2.h>

#include "segment_git.h"

typedef struct {
	char* branch_name;
	uint8_t dirty;
	uint8_t modified;
	uint8_t staged;
} GIT_STATUS;

static int get_git_status(GIT_STATUS* git_status);

int segment_git(SEGMENT* segment) {
	int error = 0;

	GIT_STATUS status = {
		.branch_name = NULL,
		.dirty = 0,
		.modified = 0,
		.staged = 0
	};

	int res = get_git_status(&status);

	if (0 == res) {
		int length = strlen(status.branch_name) + strlen(icons[GIT]) + 3;
		segment->text = (char*)malloc(length);
		memset(segment->text, 0, length);
		sprintf(segment->text, "%s %s", icons[GIT], status.branch_name);

		if (status.dirty > 0) {
			segment->background++;
		}

		return 0;
	}

	return -1;
}

static int get_git_status(GIT_STATUS* git_status) {
	git_libgit2_init();
	git_repository* repo = NULL;
	
	int error = 0;
	error = git_repository_open(&repo, "./");
	if (0 == error) {
		git_reference* head = NULL;
		error = git_repository_head(&head, repo);
		if (0 == error) {
			git_status->branch_name = strdup(git_reference_shorthand(head));
			git_status_options options = GIT_STATUS_OPTIONS_INIT;
			options.show = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
			options.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED;
			git_status_list* status;
			error = git_status_list_new(&status, repo, &options);
			if (0 == error) {
				size_t changes = git_status_list_entrycount(status);
				if (changes > 0) {
					git_status->dirty = 1;
				}
			}
		
			git_reference_free(head);
		}

		git_repository_free(repo);

		return 0;
	}

	git_libgit2_shutdown();

	return -1;
}
