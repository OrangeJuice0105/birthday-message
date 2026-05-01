SOURCES := src/birthday.c src/shared.c
HEADERS := src/*.h

CXX = gcc

ifeq ($(OS),Windows_NT)
OUTPUT := bin/birthday.exe
RM := del /q /f
else
OUTPUT := bin/birthday
RM := rm -f

CFLAGS := -Wall -Wextra -Werror -Wpedantic -g

all: $(SOURCES) $(HEADERS)
	$(CXX) $(CFLAGS) $(SOURCES) -o $(OUTPUT)

run:
	./$(OUTPUT)

.PHONY: clean
clean: 
	$(RM) $(OUTPUT)