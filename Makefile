SOURCES = birthday.c shared.c
HEADERS = *.h

CFLAGS = -Wall -Wextra -Wpedantic -g

all: $(SOURCES) $(HEADERS)
	gcc ${CFLAGS} ${SOURCES} -o birthday

clean: 
	rm -f birthday