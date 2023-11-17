#ifndef SQLITEDB_H // Only include this header file if it hasn't been included in the calling file already
    #define SQLITEDB_H

    #include <sqlite3.h>
    #include <stddef.h>
    #include "../utilities/utilities.h"

    /// @brief Object that contains all tasks from the database.
    /// @attention Must be manually deallocated!
    typedef struct db_tasks
    {
        /// @brief The amount of tasks stored in this object.
        const int amount;

        /// @brief An array of strings that contains all tasks or NULL if there aren't any.
        const char** tasks;
    } db_tasks;

    /// @brief Creates and opens a SQLite database at the specified location.
    /// @param db_location The absolute path to the database file.
    /// @return The database or NULL if the file is not a valid SQLite database.
    extern const sqlite3* create_sqlite_db(const char* db_location);

    /// @brief Adds the specified task to the database.
    /// @param db The database.
    /// @param task The task to be added.
    /// @return True if the task was successfully written to the database, False otherwise.
    extern bool add_task(const sqlite3* db, const char* task);

    /// @brief Gets all tasks in the database.
    /// @attention Must be manually deallocated!
    /// @param db The database.
    /// @return An object that contains all tasks from the database.
    extern db_tasks get_all_tasks(const sqlite3* db);
#endif // SQLITEDB_H