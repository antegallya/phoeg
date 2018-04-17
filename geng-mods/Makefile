# Configure this
includes=-I /usr/include/nauty
CC=gcc

CFLAGS=$(includes) -O3 -mpopcnt
LDFLAGS=-lnautyW1
W1=-DMAXN=WORDSIZE -DWORDSIZE=32

PRUNERS=clawfree

PROGPREFFIX=nauty-geng-

PROGS=$(addprefix ${PROGPREFFIX}, ${PRUNERS})

all: ${PROGS}

${PROGPREFFIX}%: pruners.c geng.c
	${CC} -o $@ ${CFLAGS} ${W1} -DPRUNE=$* $^ ${LDFLAGS}

clean:
	rm -f ${PROGS}
