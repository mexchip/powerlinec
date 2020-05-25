#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <git2.h>

#include "segment_git.h"

#define MAX_AHEAD 5
#define MAX_BEHIND 5
#define MAX_STASH 5

typedef struct {
	char* branch_name;
	uint16_t dirty;
	uint16_t modified;
	uint8_t staged;
	uint16_t stash;
	uint16_t ahead;
	uint16_t behind;
	char* git_server_icon;
} GIT_STATUS;

static int get_git_status(GIT_STATUS* git_status);
static git_commit* branch_to_commit(git_repository* repo, git_reference* branch);
static void set_server_icon(GIT_STATUS* git_status, const char* origin_url);
static int stash_counter(size_t index, const char* message, const git_oid* stash_id, void* payload);

static char* GIT_SERVERS_URLS[] = {
	"github.com",
	"gitlab.com",
	"bitbucket.com"
};

typedef enum {
	GIT_SERVER_GITHUB,
	GIT_SERVER_GITLAB,
	GIT_SERVER_BITBUCKET,
	MAX_GIT_SERVER
} GIT_SERVERS;

static size_t GIT_SERVERS_ICONS_INDICES[] = {9, 10, 11};

int segment_git(SEGMENT* segment) {
	int error = 0;

	GIT_STATUS status = {
		.branch_name = NULL,
		.dirty = 0,
		.modified = 0,
		.staged = 0,
		.stash = 0,
		.ahead = 0,
		.behind = 0,
		.git_server_icon = NULL,
	};

	int res = get_git_status(&status);

	if (0 == res && NULL != status.branch_name) {
		int length = strlen(status.branch_name) + 2;
		if (NULL != status.git_server_icon) {
			length += strlen(status.git_server_icon) + 1;
		}

		if (status.modified)
			length += strlen(icons[MODIFIED]);
		if (status.staged)
			length += strlen(icons[STAGED]);

		if (status.stash > MAX_STASH) {
			int digit_count = 0;
			uint8_t temp = status.stash;
			while (temp != 0) {
				temp /= 10;
				++digit_count;
			}
			length += digit_count + strlen(icons[STASH]); // stash digits + icon
		} else {
			length += status.stash * strlen(icons[STASH]);
		}

		if (status.ahead > MAX_AHEAD) {
			int digit_count = 0;
			uint8_t temp = status.ahead;
			while (temp != 0) {
				temp /= 10;
				++digit_count;
			}
			length += digit_count + strlen(icons[AHEAD]); // ahead digits + icon
		} else {
			length += status.ahead * strlen(icons[AHEAD]);
		}

		if (status.behind > MAX_BEHIND) {
			int digit_count = 0;
			uint8_t temp = status.behind;
			while (temp != 0) {
				temp /= 10;
				++digit_count;
			}
			length += digit_count + strlen(icons[BEHIND]); // behind digits + icon
		} else {
			length += status.behind * strlen(icons[BEHIND]);
		}

		segment->text = (char*)malloc(length);
		memset(segment->text, 0, length);

		if (NULL != status.git_server_icon) {
			strcpy(segment->text, status.git_server_icon);
		}

		if (0 < status.stash) {
			strcat(segment->text, " ");
		}

		if (status.stash > MAX_STASH) {
			char buffer[10];
			sprintf(buffer, "%s%d", icons[STASH], status.stash);
			strcat(segment->text, buffer);
		} else {
			for (int i = 0; i < status.stash; i++) {
				strcat(segment->text, icons[STASH]);
			}
		}

		if (0 < status.ahead) {
			strcat(segment->text, " ");
		}

		if (status.ahead > MAX_AHEAD) {
			char buffer[10];
			sprintf(buffer, "%s%d", icons[AHEAD], status.ahead);
			strcat(segment->text, buffer);
		} else {
			for (int i = 0; i < status.ahead; i++) {
				strcat(segment->text, icons[AHEAD]);
			}
		}

		if (0 < status.behind) {
			strcat(segment->text, " ");
		}

		if (status.behind > MAX_BEHIND) {
			char buffer[10];
			sprintf(buffer, "%s%d", icons[BEHIND], status.behind);
			strcat(segment->text, buffer);
		} else {
			for (int i = 0; i < status.behind; i++) {
				strcat(segment->text, icons[BEHIND]);
			}
		}

		if (NULL != status.git_server_icon) {
			strcat(segment->text, " ");
		}

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

					set_server_icon(git_status, fetch);
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

		git_stash_foreach(repo, stash_counter, &git_status->stash);
		git_repository_free(repo);
		git_libgit2_shutdown();

		return 0;
	}

	git_libgit2_shutdown();

	return -1;
}

static git_commit* branch_to_commit(git_repository* repo, git_reference* branch) {
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

static void set_server_icon(GIT_STATUS* git_status, const char* origin_url) {
	for (int i = 0; i < MAX_GIT_SERVER; i++) {
		if (NULL != strstr(origin_url, GIT_SERVERS_URLS[i])) {
			git_status->git_server_icon = (char*)malloc(strlen(icons[GIT_SERVERS_ICONS_INDICES[i]]));
			memset(git_status->git_server_icon, 0, strlen(icons[GIT_SERVERS_ICONS_INDICES[i]]));
			sprintf(git_status->git_server_icon, "%s", icons[GIT_SERVERS_ICONS_INDICES[i]]);
			return;
		}
	}

	// no server found
	git_status->git_server_icon = (char*)malloc(strlen(icons[GIT]));
	memset(git_status->git_server_icon, 0, strlen(icons[GIT]));
	sprintf(git_status->git_server_icon, "%s", icons[GIT]);
}

static int stash_counter(size_t index, const char* message, const git_oid* stash_id, void* payload) {
    (*(uint16_t*)payload)++;

    return 0;
}
