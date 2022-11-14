BIN=powerlinec
OBJS=main.o segment_dir.o segment_git.o segment_virtualenv.o

CFLAGS="-I$(HOME)/libgit2/include/"
LIBS="$(HOME)/libgit2/build/libgit2.a"

${BIN}: ${OBJS}
	${CC} -o $@ $^ ${LIBS}

clean:
	rm -f ${BIN} ${OBJS}
