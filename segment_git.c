#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <git2.h>

#include "segment_git.h"

int segment_git(SEGMENT* segment) {
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
			segment->text = (char*)malloc(length);
			memset(segment->text, 0, length);
			sprintf(segment->text, "%s %s", icons[GIT], branch);
			
			git_reference_free(head);
		}

		git_repository_free(repo);
	}

	git_libgit2_shutdown();

	return 0;
}
