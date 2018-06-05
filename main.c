#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	SEPARATOR = 0,
	HOME,
	UBUNTU,
	MAX_ICONS
} ICONS;

typedef enum {
	BLACK  = 0,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE
} COLORS;

typedef struct {
	char text[128];
	int foreground;
	int background;
} SEGMENT;

char* icons[] = {
	"\ue0b0", // separator
	"\uf015", // home
	"\uf30c"  // ubuntu
};

int main(int argc, char* argv[]) {
	for (int i = 0; i < MAX_ICONS; i++) {
		printf("%s\n", icons[i]);
	}

	char* pwd = getenv("PWD");
	char* home = getenv("HOME");

	printf("pwd: %s\n", pwd);
	printf("home: %s\n", home);

//	printf("\x1b[%d;%d;%dm", 5, 30 + BLACK, 40 + BLUE);

	if (strncmp(pwd, home, strlen(home)) == 0) {
		printf("Home is included...\n");

		printf("\x1b[48;%dm", 40 + BLUE);
		printf("\x1b[38;%dm",  30 + BLACK);

		printf(" %s%s ", icons[HOME], pwd + strlen(home));
	}
	else {
		printf(" %s ", pwd);
	}

	printf("\x1b[48;%dm", 0);
	printf("\x1b[38;%dm", 30 + BLUE);
	printf("%s", icons[SEPARATOR]);

	printf("\x1b[0m\n");

	return 0;
}

