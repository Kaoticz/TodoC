#include "./sqlite_db.h"

/* Function Prototyping */

/// @brief Initializes a SQLite database at the specified location.
/// @param db_location The absolute path to the database file.
/// @return True if the database was successfully created, False otherwise.
static bool __initialize_database(const char* db_location);

/// @brief Executes a SQL query.
/// @param db The SQLite database.
/// @param sql_query The SQL query to execute.
/// @param callback The function to execute when the database returns data.
/// @return True if the query
static bool __execute_query(sqlite3 *db, const char *sql_query, int (*callback)(void *, int, char **, char **));

/* Public Functions */

const sqlite3 *create_sqlite_db(const char *db_location)
{
    if (!file_exists(db_location) || is_file_empty(db_location))
    {
        create_empty_file(db_location);

        if (!__initialize_database(db_location))
            return NULL;
    }

    sqlite3* db;
    const int rc = sqlite3_open(db_location, &db);

    return (rc == SQLITE_OK) ? db : NULL;
}

bool add_task(const sqlite3* db, const char* task)
{
    const char* sql_query = "INSERT INTO tasks (task, unix_datetime) VALUES (?, ?)";

    // Prepare the query
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2((sqlite3*)db, sql_query, -1, &stmt, NULL)   // Prepare the database for a parameterized query.
        || sqlite3_bind_text(stmt, 1, task, -1, SQLITE_STATIC)              // Add 'task'.
        || sqlite3_bind_int64(stmt, 2, get_current_time());                 // Add 'unix_datetime'.

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Query parametization failed: %s\n", sqlite3_errmsg((sqlite3*)db));
        return false;
    }

    // Execute the query.
    rc = sqlite3_step(stmt);

    // Clean up
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "SQLite query error: %s\n", sqlite3_errmsg((sqlite3*)db));
        return false;
    }

    return true;
}

/* Private Functions */

static bool __initialize_database(const char* db_location)
{
    sqlite3* db;
    const int command_code = sqlite3_open(db_location, &db);

    if (command_code != SQLITE_OK)
    {
        fprintf(stderr, "Could not initialize database at \"%s\"\nError: %s\n", db_location, sqlite3_errmsg(db));
        sqlite3_close(db);

        return false;
    }

    const char* sql_query =
        "CREATE TABLE tasks (               \
            id INTEGER PRIMARY KEY,         \
            task TEXT NOT NULL,             \
            unix_datetime INTEGER NOT NULL  \
        );";

    bool query_success = __execute_query(db, sql_query, NULL);

    sqlite3_close(db);

    return query_success;
}

static bool __execute_query(sqlite3 *db, const char *sql_query, int (*callback)(void *, int, char **, char **))
{
    char *err_msg = NULL;
    const int command_code = sqlite3_exec(db, sql_query, callback, NULL, &err_msg);

    if (command_code == SQLITE_OK)
        return true;

    fprintf(stderr, "SQLite query error: %s\n", err_msg);
    sqlite3_free(err_msg);

    return false;
}