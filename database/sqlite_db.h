#ifndef SQLITEDB_H // Only include this header file if it hasn't been included in the calling file already
    #define SQLITEDB_H

    #include <sqlite3.h>
    #include <stddef.h>
    #include "../utilities/utilities.h"

    /// @brief Creates and opens a SQLite database at the specified location.
    /// @param db_location The absolute path to the database file.
    /// @return The database or NULL if the file is not a valid SQLite database.
    extern const sqlite3* create_sqlite_db(const char *db_location);

    /// @brief Adds the specified task to the database.
    /// @param db The database.
    /// @param task The task to be added.
    /// @return True if the task was successfully written to the database, False otherwise.
    extern bool add_task(const sqlite3 *db, const char *task);
#endif // SQLITEDB_H