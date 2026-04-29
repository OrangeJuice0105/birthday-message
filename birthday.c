#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <time.h>
#include <string.h>

#include "shared.h"
#include "colors.h"
#include "banner.h"

#define BIRTHDAY_MESSAGE "Happy birthday to you!"

volatile bool running = true;

int get_max_line_length(const char **lines, int count) {
    int max = 0;
    for (int i = 0; i < count; i++) {
        int len = strlen(lines[i]);
        if (len > max) {
            max = len;
        }
    }
    return max;
}

void print_message(const char *text, size_t term_width) {
    int len = strlen(text);
    int spaces = 0;
    if (term_width > len) {
        spaces = (term_width - len) / 2;
    }
    for (int i = 0; i < spaces; ++i) {
        putchar(' ');
    }
    puts(text);
    putchar('\n');
}

void print_centered_banner(const char **lines, size_t count, size_t term_width) {

    int max_len = get_max_line_length(lines, count);
    
    for (int i = 0; i < count; i++) {
        int len = strlen(lines[i]);
        int padding = (term_width - max_len) / 2;
        if (padding < 0) {
            padding = 0;
        }
        printf("%*s%s\n", padding, "", lines[i]);
    }
}

int main(int argc, const char *argv[]) {

    set_up_sig_handler();

    while (running) {
        clear_screen();
        size_t terminal_width = get_terminal_width();
        print_message(BIRTHDAY_MESSAGE, terminal_width);
        print_centered_banner(BANNER, BANNER_SIZE, terminal_width);
        SLEEP(20000);
    }
}