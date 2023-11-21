#ifndef CORE_H // Only include this header file if it hasn't been included in the calling file already
    #define CORE_H

    #include <stdio.h>
    #include <errno.h>
    #include "../database/sqlite_db.h"
    #include "../handlers/signal_handlers.h"
    #include "../utilities/utilities.h"

    /// @brief Represents the command to close the program.
    #define APP_EXIT 0
    
    /// @brief Represents the command to create a new task.
    #define CREATE_TASK 1
    
    /// @brief Represents the command to edit a task.
    #define EDIT_TASK 2
    
    /// @brief Represents the command to delete a task.
    #define DELETE_TASK 3
    
    /// @brief Represents the command to read a task.
    #define READ_TASK 4

    /// @brief Represents the command to read all tasks.
    #define READ_ALL_TASKS 5

    /// @brief The main loop of the program.
    /// @return Exit code.
    extern int app_loop();
#endif // CORE_H