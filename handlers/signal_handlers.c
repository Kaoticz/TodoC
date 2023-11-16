#include "./signal_handlers.h"

/* Public Functions */

void register_signal_handlers()
{
    signal(SIGTSTP, sigtstp_handler);
}

void sigtstp_handler(int sigtstp_num)
{
    printf("Cannot execute Ctrl+Z - %d/%d\n", sigtstp_num, SIGTSTP);

    // Reset handler to catch SIGTSTP next time
    signal(SIGTSTP, sigtstp_handler);
}