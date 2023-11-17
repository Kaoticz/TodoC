#include <stdio.h>
#include "./utilities/utilities.h"
#include "./handlers/signal_handlers.h"
#include "./database/sqlite_db.h"

/// @brief The entry point of the application.
/// @return The exit code of the program.
int main()
{
    // Register the signal handlers.
    //register_signal_handlers();

    const char* dir_path = get_executable_directory();
    const char* db_path = str_append(dir_path, DIRECTORY_SEPARATOR "test.db");
    const sqlite3* db = create_sqlite_db(db_path);
    //bool result = add_task(db, "Sample task.");
    db_tasks db_tasks = get_all_tasks(db);

    free((char*)dir_path);
    free((char*)db_path);
    sqlite3_close((sqlite3*)db);

    for (int counter = 0; counter < db_tasks.amount; counter++)
    {
        printf("%d. %s\n", counter, db_tasks.tasks[counter]);
    }

    free_db_tasks(&db_tasks);

    return 0;
}