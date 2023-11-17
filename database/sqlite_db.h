#ifndef SQLITEDB_H // Only include this header file if it hasn't been included in the calling file already
    #define SQLITEDB_H

    #include <sqlite3.h>
    #include <stddef.h>
    #include "../utilities/utilities.h"

    /// @brief Object that contains all tasks from the database.
    /// @attention Must be manually deallocated with "free_db_tasks()"!
    typedef struct db_tasks
    {
        /// @brief The amount of tasks stored in this object.
        const int amount;

        /// @brief An array of integers that contains all task IDs or NULL if there aren't any.
        const int* task_ids;

        /// @brief An array of strings that contains all tasks or NULL if there aren't any.
        const char** tasks;
    } db_tasks;

    /// @brief Creates and opens a SQLite database at the specified location.
    /// @param db_location The absolute path to the database file.
    /// @return The database or NULL if the file is not a valid SQLite database.
    extern const sqlite3* create_sqlite_db(const char* db_location);

    /// @brief Gets all tasks in the database.
    /// @attention Must be manually deallocated!
    /// @param db The database.
    /// @return An object that contains all tasks from the database.
    extern db_tasks get_all_tasks(const sqlite3* db);

    /// @brief Deallocates the memory used by the specified db_tasks.
    /// @param database_tasks The db_tasks to deallocate memory from.
    extern void free_db_tasks(db_tasks* database_tasks);

    /// @brief Adds the specified task to the database.
    /// @param db The database.
    /// @param task The task to be added.
    /// @return True if the task was successfully written to the database, False otherwise.
    extern bool insert_task(const sqlite3* db, const char* task);

    /// @brief Removes the task with the specified ID from the database.
    /// @param db The database.
    /// @param id The ID of the task to be removed.
    /// @return True if the task was successfully removed from the database, False otherwise.
    extern bool delete_task(const sqlite3* db, int id);

    /// @brief Updates the task with the specified ID in the database.
    /// @param db The database.
    /// @param id The ID of the task.
    /// @param new_task The new content of the task.
    /// @return True if the task was successfully updated, False otherwise.
    extern bool update_task(const sqlite3* db, int id, const char* new_task);
#endif // SQLITEDB_H