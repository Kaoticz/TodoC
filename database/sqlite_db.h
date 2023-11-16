#ifndef SQLITEDB_H // Only include this header file if it hasn't been included in the calling file already
    #define SQLITEDB_H

    #include <sqlite3.h>
    #include <stddef.h>
    #include "../utilities/utilities.h"

    /// @brief Creates and opens a SQLite database at the specified location.
    /// @param db_location The absolute path to the database file.
    /// @return The database or NULL if the file is not a valid SQLite database.
    const sqlite3* create_sqlite_db(const char *db_location);

#endif // SQLITEDB_H