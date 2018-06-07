BIN=powerlinec
OBJS=main.o segment_dir.o

CC=gcc
CFLAGS=
LIBS=-lgit2

${BIN}: ${OBJS}
	${CC} -o $@ $^ ${LIBS}

clean:
	rm -f {$BIN} ${OBJS}
