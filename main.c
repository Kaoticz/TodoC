#include <stdio.h>
#include <signal.h>

void sigtstp_handler(int);

/// @brief The entry point of the application.
/// @return The exit code of the program.
int main()
{
    // Set the handler for SIGTSTP (Ctrl-Z).
    signal(SIGTSTP, sigtstp_handler);
    getchar();

    return 0;
}

/// @brief Executes an action when Ctrl + Z is pressed.
/// @param sigtstp_num The signal number for SIGTSTP.
void sigtstp_handler(int sigtstp_num)
{
    printf("Cannot execute Ctrl+Z\n");

    // Reset handler to catch SIGTSTP next time
    signal(SIGTSTP, sigtstp_handler);
}