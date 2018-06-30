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
	uint8_t ahead;
	uint8_t behind;
} GIT_STATUS;

static int get_git_status(GIT_STATUS* git_status);
static git_commit* branch_to_commit(git_repository* repo, git_reference* branch);

int segment_git(SEGMENT* segment) {
	int error = 0;

	GIT_STATUS status = {
		.branch_name = NULL,
		.dirty = 0,
		.modified = 0,
		.staged = 0
	};

	int res = get_git_status(&status);

	if (0 == res && NULL != status.branch_name) {
		int length = strlen(status.branch_name) + strlen(icons[GIT]) + 3;
		if (status.modified)
			length += strlen(icons[MODIFIED]);
		if (status.staged)
			length += strlen(icons[STAGED]);

		length += status.ahead * strlen(icons[AHEAD]);
		length += status.behind * strlen(icons[BEHIND]);
		
		segment->text = (char*)malloc(length);
		memset(segment->text, 0, length);

		strcpy(segment->text, icons[GIT]);
		if (0 < status.ahead) {
			strcat(segment->text, " ");
		}
		for (int i = 0; i < status.ahead; i++) {
			strcat(segment->text, icons[AHEAD]);
		}
		if (0 < status.behind) {
			strcat(segment->text, " ");
		}
		for (int i = 0; i < status.behind; i++) {
			strcat(segment->text, icons[BEHIND]);
		}

		strcat(segment->text, " ");
		strcat(segment->text, status.branch_name);

		if (status.modified || status.staged)
			strcat(segment->text, " ");

		if (status.modified)
			strcat(segment->text, icons[MODIFIED]);
		
		if (status.staged)
			strcat(segment->text, icons[STAGED]);

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
	error = git_repository_open_ext(&repo, "./", 0, NULL);
	if (0 == error) {
		git_reference* head = NULL;
		error = git_repository_head(&head, repo);
		if (0 == error) {
			git_remote* remote = NULL;
			error = git_remote_lookup(&remote, repo, "origin");
			if (0 == error) {
				const char* fetch = git_remote_url(remote);
				if (fetch) {
					git_reference* upstream;
					error = git_branch_upstream(&upstream, head);
					if (0 == error) {
						size_t ahead, behind;
						git_commit* branch_commit = branch_to_commit(repo, head);
						git_commit* upstream_commit = branch_to_commit(repo, upstream);;
						const git_oid* local_oid;
						const git_oid* upstream_oid;
						local_oid = git_commit_id(branch_commit);
						upstream_oid = git_commit_id(upstream_commit);
						git_graph_ahead_behind(&ahead, &behind, repo, local_oid, upstream_oid);
						git_status->ahead = ahead;
						git_status->behind = behind;
					}
				}
			}
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

				const git_status_entry* s;
				for (size_t i = 0; i < changes; i++) {
					s = git_status_byindex(status, i);

					if (s->status == GIT_STATUS_CURRENT)
						continue;

					if (
						s->status & GIT_STATUS_WT_NEW
						|| s->status & GIT_STATUS_WT_MODIFIED
						|| s->status & GIT_STATUS_WT_DELETED
						|| s->status & GIT_STATUS_WT_RENAMED
						|| s->status & GIT_STATUS_WT_TYPECHANGE
					) {
						git_status->modified = 1;
					}

					if (
						s->status & GIT_STATUS_INDEX_NEW
						|| s->status & GIT_STATUS_INDEX_MODIFIED
						|| s->status & GIT_STATUS_INDEX_DELETED
						|| s->status & GIT_STATUS_INDEX_RENAMED
						|| s->status & GIT_STATUS_INDEX_TYPECHANGE
					) {
						git_status->staged = 1;
					}
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

git_commit* branch_to_commit(git_repository* repo, git_reference* branch) {
	int error = 0;

	git_object* commit_obj;
	error = git_reference_peel(&commit_obj, branch, GIT_OBJ_COMMIT);
	if (0 != error) {
		// printf("error peel\n");
		return NULL;
	}

	const git_oid* oid = git_object_id(commit_obj);

	git_commit* commit;
	error = git_commit_lookup(&commit, repo, oid);
	git_object_free(commit_obj);

	if (0 == error) {
		// printf("returning commit\n");
		return commit;
	}

	// printf("returning NULL\n");
	return NULL;
}