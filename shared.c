#define _POSIX_C_SOURCE 200809L

#include "shared.h"

#define DEFAULT_SIZE 80

#ifdef __linux__

    #include <signal.h>
    #include <sys/ioctl.h>

    void handle_signal(int signal) {
        if (signal == SIGINT) {
            running = false;
        }
    }

    void clear_screen() {
        system("clear");
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