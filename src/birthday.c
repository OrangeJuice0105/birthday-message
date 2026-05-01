#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <time.h>
#include <string.h>

#include "shared.h"
#include "colors.h"
#include "banner.h"

#define BIRTHDAY_MESSAGE "Happy birthday to you!"
#define SHUTDOWN_MESSAGE_NAME "Happy birthday %s!"
#define SHUTDOWN_MESSAGE_NORMAL "Happy birthday!"

#define REFRESH_RATE 300
#define MAX_SIZE 40

volatile bool running = true;

size_t get_max_line_length(const char **lines, size_t count) {
    size_t max = 0;
    for (size_t i = 0; i < count; i++) {
        size_t length = strlen(lines[i]);
        if (length > max) {
            max = length;
        }
    }
    return max;
}

void print_rainbow_line(const char *line, int padding, int shift) {
    printf("%*s", padding, "");
    for (int i = 0; line[i] != '\0'; i++) {
        int color_index = (i + shift) % COLOURS_COUNT;
        printf("%s%c%s", ANSI_RAINBOW[color_index], line[i], RESET);
    }
    putchar('\n');
}

void print_message(const char *text, size_t term_width) {
    size_t length = strlen(text);
    int spaces = 0;
    if (term_width > length) {
        spaces = (term_width - length) / 2;
    }
    for (int i = 0; i < spaces; ++i) {
        putchar(' ');
    }
    puts(text);
    putchar('\n');
}

void print_centered_banner(const char **lines, size_t count, size_t term_width) {

    size_t max_len = get_max_line_length(lines, count);
    
    for (size_t i = 0; i < count; i++) {
        int padding = (term_width - max_len) / 2;
        if (padding < 0) {
            padding = 0;
        }
        print_rainbow_line(lines[i], padding, i);
    }
}

void print_last_message(char *message) {
    puts(RESET);
    putchar('\n');
    size_t length = strlen(message);
    for (size_t i = 0; i < length; i++) {
        putchar(message[i]);
        fflush(stdout);
        SLEEP(REFRESH_RATE);
    }
    putchar('\n');
    exit(EXIT_SUCCESS);
}

int main(int argc, const char *argv[]) {

    char message[MAX_SIZE];
    memset(message, 0, sizeof(message));

    if (argc == 1) {
        sprintf(message, SHUTDOWN_MESSAGE_NORMAL);
    } else if (strcmp(argv[1], "self")) {
        sprintf(message, SHUTDOWN_MESSAGE_NAME, "to me");
    } else {
        sprintf(message, SHUTDOWN_MESSAGE_NAME, argv[1]);
    }

    int index = 0;
    set_up_sig_handler();

    #ifdef __linux__
        enable_raw_mode();
        atexit(disable_raw_mode);
    #endif
    
    while (running) {
        clear_screen();
        size_t terminal_width = get_terminal_width();
        printf("%s", ANSI_RAINBOW[index]);
        putchar('\n');
        print_message(BIRTHDAY_MESSAGE, terminal_width);
        putchar('\n');
        print_centered_banner(BANNER, BANNER_SIZE, terminal_width);
        if (index++ >= COLOURS_COUNT) {
            index = 0;
        }
        SLEEP(REFRESH_RATE);
        if (quit_pressed()) {
            running = false;
        }
    }
    print_last_message(message);
    return EXIT_SUCCESS;
}