#include <stdio.h>
#include "./utilities/utilities.h"
#include "./handlers/signal_handlers.h"

/// @brief The entry point of the application.
/// @return The exit code of the program.
int main()
{
    // Register the signal handlers.
    //register_signal_handlers();

    const char* dir_path = get_executable_directory();
    const char *db_path = str_append(dir_path, DIRECTORY_SEPARATOR "test.db");
    bool exists = file_exists(db_path);
    bool io_code = create_empty_file(db_path);

    printf("Directory (%d-%d): %s\n", io_code, exists, dir_path);
    free((char *)dir_path);
    free((char *)db_path);

    return 0;
}