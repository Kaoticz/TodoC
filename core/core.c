#include "./core.h"

/* Private Variables */

/// @brief The amount of characters a frame must have.
static const int __frame_char_amount = 25;

/* Function Prototypes */

/// @brief Prints the main menu of the program.
/// @param optional_message An optional message to be displayed at the top of the menu.
static void __print_menu(char* optional_message);

/// @brief Gets an integer input from the user within the specified range.
/// @param min The minimum integer expected.
/// @param max The maximum integer expected.
/// @return The integer input by the user. If the user attempted to input
/// @return an integer outside the specified range, it returns "min - 1" instead.
static int __get_user_int_input(int min, int max);

/// @brief Gets a valid integer input from the user within the specified range.
/// @param min The minimum integer expected.
/// @param max The maximum integer expected.
/// @param message The message to be displayed to the user.
/// @return The integer input by the user.
static int __get_valid_user_int_input(int min, int max, const char* message);

/// @brief Invokes the appropriate action according to the input provided by the user.
/// @param input The user's input.
/// @param message The message returned by the operation. May be NULL.
/// @return Zero if the operation executed successfuly or failed in a non-critical way,
/// @return non-zero if a critical error occurred and the program must shutdown.
static int __dispatcher(const int input, char* message);

/// @brief Creates a new task.
/// @param task The content of the task.
/// @param message The message returned by the operation. May be NULL.
/// @return True if the task was created, False otherwise.
static bool __create_task(const char* task, char* message);

static bool __edit_task(const int task_id, const char* task, char* message);

/// @brief Deletes the specified task from the database.
/// @param task_id The Id of the task.
/// @param message The message returned by the operation. May be NULL.
/// @return True if the task was deleted, False otherwise.
static bool __delete_task(const int task_id, char* message);

/// @brief Writes the task of the specified ID to stdout.
/// @param task_id The ID of the task.
/// @param message The message returned by the operation. May be NULL.
/// @return True if the task was printed, False otherwise.
static bool __print_task(const int task_id, char* message);

/// @brief Writes all tasks to stdout.
/// @param message The message returned by the operation. May be NULL.
/// @return True if at least one task was printed out, False if no tasks were found.
static bool __print_all_tasks(char* message);

/// @brief Prompts the user to press Enter.
/// @param message The message to be shown to the user.
static void __prompt_and_wait(char* message);

/// @brief Writes the specified character to stdout by the specified amount.
/// @param character The character to be printed.
/// @param amount How many times the character should be printed.
static void __print_char(const char character, const int amount);

/* Public Functions */

int app_loop()
{
    int status_code = 0, input = 0;
    char message[64] = { 0 };

    do
    {
        clear_console();
        __print_menu(message);
        printf("> ");

        input = __get_user_int_input(0, 5);
        status_code = __dispatcher(input, message);

        if (status_code != EXIT_SUCCESS)
        {
            fprintf(stderr, message);
            return status_code;
        }

    } while (input != APP_EXIT);

    clear_console();

    return 0;
}

/* Private Functions */

static void __print_menu(char* optional_message)
{
    if (optional_message != NULL && optional_message[0] != '\0')
    {
        printf("%s" NEWLINE NEWLINE, optional_message);
        optional_message[0] = '\0';
    }

    printf(
        "Welcome to TodoC!" NEWLINE
        "Select one of the options below:" NEWLINE
        "%d. Create a new note." NEWLINE
        "%d. Edit a note." NEWLINE
        "%d. Delete a note." NEWLINE
        "%d. Read a specific note." NEWLINE
        "%d. Read all notes." NEWLINE
        "%d. Exit." NEWLINE,
        CREATE_TASK, EDIT_TASK, DELETE_TASK, READ_TASK, READ_ALL_TASKS, APP_EXIT
    );
}

static int __get_user_int_input(int min, int max)
{
    if (min > max)
        swap(&min, &max);

    int input = 0;
    scanf("%d", &input);
    flush(stdin);

    return (input >= min && input <= max)
        ? input
        : min - 1;
}

static int __get_valid_user_int_input(int min, int max, const char* message)
{
    int input, fail_code = min - 1;

    do
    {
        printf(message);
        input = __get_user_int_input(min, max);
    } while (input == fail_code);

    return input;
}

static int __dispatcher(const int input, char* message)
{
    int status_code = 0;
    clear_console();

    switch (input)
    {
        case CREATE_TASK:
            __create_task("Hello new task!", message);
            break;
        case EDIT_TASK:
        {
            int task_id = __get_valid_user_int_input(1, INT_MAX, "Type the ID of the note: ");
            clear_console();

            __edit_task(task_id, "New task!", message);
            break;
        }
        case DELETE_TASK:
        {
            int task_id = __get_valid_user_int_input(1, INT_MAX, "Type the ID of the note: ");
            clear_console();

            if (!__print_task(task_id, message))
                break;

            printf("Are you sure you want to delete this note? Type %d to confirm: ", task_id);
            int input = __get_user_int_input(task_id, task_id);

            if (input == task_id)
                __delete_task(task_id, message);
            break;
        }
        case READ_TASK:
        {
            int task_id = __get_valid_user_int_input(1, INT_MAX, "Type the ID of the note: ");
            clear_console();

            if (__print_task(task_id, message))
                __prompt_and_wait("Press Enter to continue.");
            break;
        }
        case READ_ALL_TASKS:
            if (__print_all_tasks(message))
                __prompt_and_wait("Press Enter to continue.");
            break;
        default:
            strcpy(message, "Please, enter a valid option.");
            break;
    };

    return status_code;
}

static bool __create_task(const char* task, char* message)
{
    const sqlite3* db = get_db();
    bool inserted = insert_task(db, task);

    // Cleanup
    sqlite3_close((sqlite3*)db);

    const char* returning_message = (inserted)
        ? "Note created successfully."
        : "An error occurred when attempting to create a note.";

    strcpy(message, returning_message);

    return inserted;
}

static bool __edit_task(const int task_id, const char* task, char* message)
{
    const sqlite3* db = get_db();
    bool updated = update_task(db, task_id, task);

    // Cleanup
    sqlite3_close((sqlite3*)db);

    const char* returning_message = (updated)
        ? "Note updated successfully."
        : "An error occurred when attempting to update a note.";

    strcpy(message, returning_message);

    return updated;
}

static bool __delete_task(const int task_id, char* message)
{
    const sqlite3* db = get_db();
    bool deleted = delete_task(db, task_id);

    // Cleanup
    sqlite3_close((sqlite3*)db);

    const char* returning_message = (deleted)
        ? "Note of ID %d has been successfully deleted."
        : "An error occurred when attempting to delete note of ID %d. Entry most likely was not found.";

    sprintf(message, returning_message, task_id);

    return deleted;
}

static bool __print_task(const int task_id, char* message)
{
    const sqlite3* db = get_db();
    db_task db_task = get_task(db, task_id);

    if (db_task.task == NULL)
    {
        sprintf(message, "Note of ID %d was not found.", task_id);
        sqlite3_close((sqlite3*)db);

        return false;
    }

    __print_char('=', __frame_char_amount);
    printf(NEWLINE);

    printf("%s" NEWLINE, db_task.task);

    __print_char('=', __frame_char_amount);
    printf(NEWLINE);

    // Cleanup
    sqlite3_close((sqlite3*)db);
    free_db_task(&db_task);

    return true;
}

static bool __print_all_tasks(char* message)
{
    const sqlite3* db = get_db();
    db_tasks db_tasks = get_all_tasks(db);

    if (db_tasks.amount <= 0)
    {
        strcpy(message, "No notes were found.");
        sqlite3_close((sqlite3*)db);

        return false;
    }

    __print_char('=', __frame_char_amount);
    printf(NEWLINE);

    for (int index = 0; index < db_tasks.amount; index++)
        printf("--- Note ID: %d ---" NEWLINE "%s" NEWLINE, db_tasks.task_ids[index], db_tasks.tasks[index]);

    __print_char('=', __frame_char_amount);
    printf(NEWLINE);

    // Cleanup
    sqlite3_close((sqlite3*)db);
    free_db_tasks(&db_tasks);

    return true;
}

static void __prompt_and_wait(char* message)
{
    printf("%s" NEWLINE, message);
    flush(stdin);
}

static void __print_char(const char character, const int amount)
{
    for (int count = 0; count < amount; count++)
        printf("%c", character);
}