#ifndef POWERLINEC_H
#define POWERLINEC_H

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


#endif