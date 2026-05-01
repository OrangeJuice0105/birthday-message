#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include "shared.h"

#define DEFAULT_SIZE 80

#ifdef __linux__

    #include <signal.h>
    #include <unistd.h>
    #include <sys/ioctl.h>
    #include <termios.h>
    #include <sys/select.h>

    static struct termios old_termios;

    void enable_raw_mode(void) {
        struct termios raw;
        tcgetattr(STDIN_FILENO, &old_termios);
        raw = old_termios;
        raw.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    }

    void disable_raw_mode(void) {
        tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
    }

    bool quit_pressed(void) {

        struct timeval tv = {0, 0};
        fd_set fds;

        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);

        if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0) {
            char c;
            if (read(STDIN_FILENO, &c, 1) == 1) {
                return c == 'q' || c == 'Q';
            }
        }

        return false;
    }

    void handle_signal(int signal) {
        if (signal == SIGINT) {
            running = false;
        }
    }

    void clear_screen() {
        puts("\033[H\033[J");
    }

    void set_up_sig_handler() {
        struct sigaction action;
        sigemptyset(&action.sa_mask);
        action.sa_handler = handle_signal;
        action.sa_flags = 0;
        sigaction(SIGINT, &action, NULL);
    }

    size_t get_terminal_width(void) {
        struct winsize window;
        return (ioctl(STDOUT_FILENO, TIOCGWINSZ, &window) == -1 || window.ws_col == 0) ? DEFAULT_SIZE : window.ws_col;
    }
    
#else

    #include <conio.h>

    bool quit_pressed(void) {
        if (_kbhit()) {
            int c = _getch();
            return c == 'q' || c == 'Q';
        }

        return false;
    }

    void clear_screen() {
        system("cls");
    }

    BOOL WINAPI handle_signal(DWORD signal) {
        if (signal == CTRL_C_EVENT || signal == CTRL_CLOSE_EVENT) {
            running = false;
            return TRUE;      
        }
        return FALSE;
    }

    void enable_vt_mode() {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }

    void set_up_sig_handler() {
        enable_vt_mode();
        SetConsoleCtrlHandler(handle_signal, TRUE);
    }

    size_t get_terminal_width(void) {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE) {
            return DEFAULT_SIZE;
        }
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
            return DEFAULT_SIZE;
        }
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }

#endif