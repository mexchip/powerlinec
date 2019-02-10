#ifndef POWERLINEC_H
#define POWERLINEC_H

#include <stddef.h>

typedef enum {
	SEPARATOR = 0,
	HOME,
	UBUNTU,
	GIT,
	MODIFIED,
	STAGED,
	AHEAD,
	BEHIND,
	STASH,
	GITHUB,
	GITLAB,
	BITBUCKET,
	MAX_ICON
} ICON;

typedef struct {
	int foreground;
	int background;
	char* text;
} SEGMENT;

extern char* color_names[];
extern char* icons[];

#ifdef DEFINE_GLOBALS
char* color_names[] = {
	"black",
	"red",
	"green",
	"yellow",
	"blue",
	"magenta",
	"cyan",
	"white"
};

char* icons[] = {
	"\ue0b0", // separator
	"\uf015", // home
	"\uf30c", // ubuntu
	"\ue0a0", // git
	"\uf111", // modified
	"\uf067", // staged
	"\uf148", // ahead
	"\uf149", // behind
	"\uf01c", // stash
	"\uf09b", // github
	"\uf296", // gitlab
	"\uf171", // bitbucket
};

#endif

#endif
