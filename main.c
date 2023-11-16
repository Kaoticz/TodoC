#include <stdio.h>
#include <sqlite3.h>
#include "./utilities/utilities.h"
#include "./handlers/signal_handlers.h"

/// @brief The entry point of the application.
/// @return The exit code of the program.
int main()
{
    // Register the signal handlers.
    register_signal_handlers();
    getchar();
    printf("%s\n", sqlite3_libversion());

    const char* path = get_executable_directory();
    printf("Directory: %s\n", path);
    free((char *)path);

    return 0;
}