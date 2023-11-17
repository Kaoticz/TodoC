#include "./sqlite_db.h"

/* Private Variables */

/// @brief Used to keep track of iterations of "__callback_select_tasks()".
static int __select_tasks_current_index = 0;

/* Function Prototyping */

/// @brief Initializes a SQLite database at the specified location.
/// @param db_location The absolute path to the database file.
/// @return True if the database was successfully created, False otherwise.
static bool __initialize_database(const char* db_location);

/// @brief Executes a SQL query.
/// @param db The SQLite database.
/// @param sql_query The SQL query to execute.
/// @param callback The function to execute when the database returns data.
/// @param custom_state Pointer to an object that's being passed into the callback function.
/// @return True if the query completed successfully, False otherwise.
static bool __execute_query(const sqlite3* db, const char* sql_query, int (*callback)(void*, int, char**, char**), void* custom_state);

/// @brief Executes a parameterized SQL query.
/// @param db The SQLite database.
/// @param sql_query The SQL query to execute.
/// @param callback The function to execute to add the parameters to the query.
/// @param arg_count The amount of parameters in the query.
/// @param ... The parameters to be added to the query.
/// @return True if the query completed successfully, False otherwise.
static bool __execute_parameterized_query(const sqlite3* db, const char* sql_query, int (*callback)(sqlite3_stmt*, va_list, int), int arg_count, ...);

/// @brief Adds query parameters for "add_task".
/// @param stmt The compiled SQL statement.
/// @param args The arguments to be added to the query.
/// @param arg_count The amount of arguments to be added.
/// @return Zero if the operation succeeded, non-zero otherwise.
static int __prepare_add_query(sqlite3_stmt* stmt, va_list args, int arg_count);

/// @brief Adds query parameters for "delete_task".
/// @param stmt The compiled SQL statement.
/// @param args The arguments to be added to the query.
/// @param arg_count The amount of arguments to be added.
/// @return Zero if the operation succeeded, non-zero otherwise.
static int __prepare_delete_query(sqlite3_stmt* stmt, va_list args, int arg_count);

/// @brief Callback that returns the result of a "COUNT(*)" query.
/// @param custom_state int* to write the query result to.
/// @param column_amount The amount of columns returned in the query.
/// @param column_contents An array of strings with the content of all columns in a single row.
/// @param column_names An array of strings with the name of all columns in a single row.
/// @return Zero.
static int __callback_count_tasks(void* custom_state, UNUSED int column_amount, char** column_contents, UNUSED char** column_names);

/// @brief Callback that returns the result of a "SELECT tasks" query.
/// @param custom_state db_tasks* to write the query result to.
/// @param column_amount The amount of columns returned in the query.
/// @param column_contents An array of strings with the content of all columns in a single row.
/// @param column_names An array of strings with the name of all columns in a single row.
/// @return Zero.
static int __callback_select_tasks(void* custom_state, UNUSED int column_amount, char** column_contents, UNUSED char** column_names);

/* Public Functions */

const sqlite3* create_sqlite_db(const char* db_location)
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

db_tasks get_all_tasks(const sqlite3* db)
{
    int task_amount = 0;
    __execute_query(db, "SELECT COUNT(*) FROM tasks;", __callback_count_tasks, &task_amount);

    db_tasks db_tasks = {
        .amount = task_amount,
        .task_ids = (task_amount == 0) ? NULL : calloc(task_amount, sizeof(int)),
        .tasks = (task_amount == 0) ? NULL : calloc(task_amount, sizeof(char*))
    };

    if (task_amount == 0)
        return db_tasks;

    __execute_query(db, "SELECT id, task FROM tasks;", __callback_select_tasks, &db_tasks);
    __select_tasks_current_index = 0;

    return db_tasks;
}

void free_db_tasks(db_tasks* db_tasks)
{
    if (db_tasks->amount == 0)
        return;

    for (int counter = 0; counter < db_tasks->amount; counter++)
        free((char*)db_tasks->tasks[counter]);

    free((int*)db_tasks->task_ids);
    free(db_tasks->tasks);

    // Reset the amount
    int* amount_ptr = (int*)&db_tasks->amount;
    *amount_ptr = 0;

    // Reset the task IDs
    db_tasks->task_ids = NULL;

    // Reset the tasks
    db_tasks->tasks = NULL;
}

bool add_task(const sqlite3* db, const char* task)
{
    const char* sql_query = "INSERT INTO tasks (task, created_at) VALUES (?, ?);";
    return __execute_parameterized_query(db, sql_query, __prepare_add_query, 2, task, get_current_time());
}

bool delete_task(const sqlite3* db, int id)
{
    const char* sql_query = "DELETE FROM tasks WHERE id = ?;";
    return __execute_parameterized_query(db, sql_query, __prepare_delete_query, 1, id);
}

/* Private Functions */

static bool __initialize_database(const char *db_location)
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
            created_at INTEGER NOT NULL     \
        );";

    bool query_success = __execute_query(db, sql_query, NULL, NULL);

    sqlite3_close(db);

    return query_success;
}

static bool __execute_query(const sqlite3* db, const char* sql_query, int (*callback)(void*, int, char**, char**), void* custom_state)
{
    char* err_msg = NULL;
    const int command_code = sqlite3_exec((sqlite3*)db, sql_query, callback, custom_state, &err_msg);

    if (command_code == SQLITE_OK)
        return true;

    fprintf(stderr, "SQLite query error: %s\n", err_msg);
    sqlite3_free(err_msg);

    return false;
}

static bool __execute_parameterized_query(const sqlite3* db, const char* sql_query, int (*callback)(sqlite3_stmt*, va_list, int), int arg_count, ...)
{
    sqlite3_stmt* stmt = NULL;
    va_list args;

    va_start(args, arg_count);

    int db_code = sqlite3_prepare_v2((sqlite3*)db, sql_query, -1, &stmt, NULL)  // Prepare the database for a parameterized query.
        || callback(stmt, args, arg_count);                                     // Add the arguments.

    va_end(args);

    if (db_code != SQLITE_OK)
    {
        fprintf(stderr, "Query parametization failed: %s\n", sqlite3_errmsg((sqlite3*)db));
        return false;
    }

    // Execute the query.
    db_code = sqlite3_step(stmt);

    // Clean up
    sqlite3_finalize(stmt);

    if (db_code != SQLITE_DONE)
    {
        fprintf(stderr, "SQLite query error: %s\n", sqlite3_errmsg((sqlite3*)db));
        return false;
    }

    return true;
}

static int __prepare_add_query(sqlite3_stmt* stmt, va_list args, int arg_count)
{
    UNUSED_VAR(arg_count);
    return sqlite3_bind_text(stmt, 1, va_arg(args, char*), -1, SQLITE_STATIC)   // Add 'task'.
        || sqlite3_bind_int64(stmt, 2, va_arg(args, time_t));                   // Add 'created_at'.
}

static int __prepare_delete_query(sqlite3_stmt* stmt, va_list args, int arg_count)
{
    UNUSED_VAR(arg_count);
    return sqlite3_bind_int(stmt, 1, va_arg(args, int));    // Add 'id'.
}

/* Private Functions - Callbacks */

static int __callback_count_tasks(void* custom_state, UNUSED int column_amount, char** column_contents, UNUSED char** column_names)
{
    *((int*)custom_state) = atoi(column_contents[0]);
    return 0;
}

static int __callback_select_tasks(void* custom_state, UNUSED int column_amount, char** column_contents, UNUSED char** column_names)
{
    db_tasks* db_tasks = custom_state;

    // Set the array of ids.
    ((int*)db_tasks->task_ids)[__select_tasks_current_index] = atoi(column_contents[0]);

    // Set the array of strings.
    int task_length = strlen(column_contents[1]) + 1;
    char* content_copy = malloc(task_length);
    db_tasks->tasks[__select_tasks_current_index++] = content_copy;

    strcpy(content_copy, column_contents[1]);

    return 0;
}