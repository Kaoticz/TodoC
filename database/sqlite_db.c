#include "./sqlite_db.h"

/* Function Prototyping */

/// @brief Initializes a SQLite database at the specified location.
/// @param db_location The absolute path to the database file.
/// @return True if the database was successfully created, False otherwise.
static bool __initialize_database(const char* db_location);

/// @brief Executes a SQL query.
/// @param db The SQLite database.
/// @param sql_query The SQL query to execute.
/// @return True if the query
static bool __execute_query(sqlite3* db, const char *sql_query);

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
    int rc = sqlite3_open(db_location, &db);

    return (rc == SQLITE_OK) ? db : NULL;
}

/* Private Functions */

static bool __initialize_database(const char* db_location)
{
    sqlite3* db;
    int command_code = sqlite3_open(db_location, &db);

    if (command_code != SQLITE_OK)
    {
        fprintf(stderr, "Could not initialize database at \"%s\"\nError: %s\n", db_location, sqlite3_errmsg(db));
        sqlite3_close(db);

        return false;
    }

    /*
    CREATE TABLE tasks (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        task TEXT,
        unix_datetime INTEGER,
        CONSTRAINT tasks_PK PRIMARY KEY (id)
    );
    */

    bool query_success = __execute_query(
        db,
        "CREATE TABLE tasks (                       \
           id INTEGER PRIMARY KEY AUTOINCREMENT,    \
           task TEXT,                               \
           unix_datetime INTEGER,                   \
           CONSTRAINT tasks_PK PRIMARY KEY(id)      \
        );"
    );

    sqlite3_close(db);

    return (query_success == true) ? true : false;
}

static bool __execute_query(sqlite3* db, const char* sql_query)
{
    char *err_msg = NULL;
    int command_code = sqlite3_exec(db, sql_query, 0, 0, &err_msg);

    if (command_code == SQLITE_OK)
        return true;

    fprintf(stderr, "SQLite query error: %s\n", err_msg);
    sqlite3_free(err_msg);

    return false;
}