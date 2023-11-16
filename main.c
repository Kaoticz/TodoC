#include <stdio.h>
#include <signal.h>
#include <sqlite3.h>
#include "./utilities/utilities.h"

void sigtstp_handler(int);

/// @brief The entry point of the application.
/// @return The exit code of the program.
int main()
{
    // Set the handler for SIGTSTP (Ctrl-Z).
    signal(SIGTSTP, sigtstp_handler);
    getchar();
    printf("%s\n", sqlite3_libversion());

    const char* path = get_executable_directory();
    printf("Directory: %s\n", path);
    free((char*)path);

    return 0;
}

/// @brief Executes an action when Ctrl + Z is pressed.
/// @param sigtstp_num The signal number for SIGTSTP.
void sigtstp_handler(int sigtstp_num)
{
    printf("Cannot execute Ctrl+Z - %d/%d\n", sigtstp_num, SIGTSTP);

    // Reset handler to catch SIGTSTP next time
    signal(SIGTSTP, sigtstp_handler);
}