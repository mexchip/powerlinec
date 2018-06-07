#ifndef POWERLINEC_H
#define POWERLINEC_H

#include <stddef.h>

typedef enum {
	SEPARATOR = 0,
	HOME,
	UBUNTU,
	GIT,
	MAX_ICON
} ICON;

typedef struct {
	char* text;
	int foreground;
	int background;
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
	"cyan",
	"white"
};

char* icons[] = {
	"\ue0b0", // separator
	"\uf015", // home
	"\uf30c", // ubuntu
	"\ue0a0"  // git
};

#endif

#endif