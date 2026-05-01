#ifndef SHARED_H
#define SHARED_H

#include <stdbool.h>
#include <stdlib.h>

void clear_screen();
void set_up_sig_handler(void);
bool quit_pressed(void);
size_t get_terminal_width(void);

#ifdef __linux__

    
    #include <unistd.h>
    #include <time.h>

    #define SLEEP(ms) do { \
        struct timespec ts; \
        ts.tv_sec = (ms) / 1000; \
        ts.tv_nsec = ((ms) % 1000) * 1000000; \
        nanosleep(&ts, NULL); \
    } while (0)

    void handle_signal(int signal);
    void enable_raw_mode(void);
    void disable_raw_mode(void);
    
#else 

    #include <windows.h>
    #define SLEEP(ms) Sleep(ms)
    BOOL WINAPI handle_signal(DWORD signal);

#endif

extern volatile bool running;

#endif