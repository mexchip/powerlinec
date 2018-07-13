BIN=powerlinec
OBJS=main.o segment_dir.o segment_git.o segment_virtualenv.o

CC=gcc
CFLAGS=
LIBS=-lgit2_s -lpthread -lcrypto -lssl -lssh2 -lcurl -lhttp_parser

${BIN}: ${OBJS}
	${CC} -o $@ $^ ${LIBS}

clean:
	rm -f ${BIN} ${OBJS}
