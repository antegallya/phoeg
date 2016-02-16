INCLUDES += -I/usr/include/postgresql
INCLUDES += -I/usr/include/postgresql/9.4/server/

CC = gcc
ECPG = ecpg
ECPGFLAGS += -c $(INCLUDES)
CFLAGS += -g -Wall
CFLAGS += $(INCLUDES)
CPPFLAGS += -g -Wall -std=c++11
LDLIBS += -lecpg -lpq -lpgtypes
LDFLAGS += -L/usr/lib/postgresql/9.4/lib/

ALL = dbquery test testsBoost

.PHONY: all clean
.SUFFIXES: .pgc

all: $(ALL)

.pgc.c:
	$(ECPG) $(ECPGFLAGS) $?

clean:
	rm -f $(ALL) dbquery.c *.o *~
