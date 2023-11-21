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

/// @brief Gets a multi-line string input from the user.
/// @attention Requires the usert o press Ctrl + Z to get out of the input loop.
/// @param optional_message An optional message to be printed to the user.
/// @return The string input by the user.
static const char* get_user_text_input(const char* optional_message);

/// @brief Invokes the appropriate action according to the input provided by the user.
/// @param db The database.
/// @param input The user's input.
/// @param message The message returned by the operation. May be NULL.
/// @return Zero if the operation executed successfuly or failed in a non-critical way,
/// @return non-zero if a critical error occurred and the program must be terminated.
static int __dispatcher(const sqlite3* db, const int input, char* message);

/// @brief Creates a new task.
/// @param db The database.
/// @param task The content of the task.
/// @param message The message returned by the operation. May be NULL.
/// @return True if the task was created, False otherwise.
static bool __create_task(const sqlite3* db, const char* task, char* message);

/// @brief Updates the specified task with new content.
/// @param db The database.
/// @param task_id The ID of the task.
/// @param task The new content of the task.
/// @param message The message returned by the operation. May be NULL.
/// @return True if the task was updated, False otherwise.
static bool __edit_task(const sqlite3* db, const int task_id, const char* task, char* message);

/// @brief Deletes the specified task from the database.
/// @param db The database.
/// @param task_id The Id of the task.
/// @param message The message returned by the operation. May be NULL.
/// @return True if the task was deleted, False otherwise.
static bool __delete_task(const sqlite3* db, const int task_id, char* message);

/// @brief Writes the task of the specified ID to stdout.
/// @param db The database.
/// @param task_id The ID of the task.
/// @param message The message returned by the operation. May be NULL.
/// @return True if the task was printed, False otherwise.
static bool __print_task(const sqlite3* db, const int task_id, char* message);

/// @brief Writes all tasks to stdout.
/// @param db The database.
/// @param message The message returned by the operation. May be NULL.
/// @return True if at least one task was printed out, False if no tasks were found.
static bool __print_all_tasks(const sqlite3* db, char* message);

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
    const sqlite3* db = get_db();

    if (db == NULL)
    {
        fprintf(stderr, "The database is corrupted or could not be created due to lack of write permissions. Aborting...");
        return EPERM;
    }

    do
    {
        clear_console();
        __print_menu(message);
        printf("> ");

        input = __get_user_int_input(0, 5);
        clear_console();
        status_code = __dispatcher(db, input, message);

        if (status_code != EXIT_SUCCESS)
        {
            fprintf(stderr, message);
            sqlite3_close((sqlite3*)db);

            return status_code;
        }

    } while (input != APP_EXIT);

    clear_console();
    sqlite3_close((sqlite3*)db);

    return status_code;
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

static const char* get_user_text_input(const char* optional_message)
{
    // Print the instructions header.
    if (optional_message != NULL)
        printf(optional_message);

    printf(NEWLINE "Press \"Enter + Ctrl + Z + Enter\" to save your note." NEWLINE);
    __print_char('=', __frame_char_amount);
    printf(NEWLINE);

    // Set the variables.
    char current_char = '\0';
    int buffer_length = 16 * sizeof(char);
    int buffer_position = 0;
    char* buffer = malloc(buffer_length);

    // Start listening to SIGTSTP (Ctrl + Z).
    start_sigtstp_handler();

    do
    {
        current_char = getchar();

        if (!is_typing_allowed())
            break;

        if (buffer_position >= buffer_length - 1)
        {
            buffer_length *= 2;
            buffer = realloc(buffer, buffer_length);
        }

        buffer[buffer_position++] = current_char;
    } while (current_char != EOF);

    // Finalize the string by replacing the last
    // newline with a null terminator character.
    buffer[max(0, buffer_position - 1)] = '\0';

    return buffer;
}

static int __dispatcher(const sqlite3* db, const int menu_selection, char* message)
{
    switch (menu_selection)
    {
        case CREATE_TASK:
        {
            const char* input = get_user_text_input("Type your new note below.");
            const size_t input_length = strlen(input);

            if (input_length != 0 && strspn(input, NEWLINE) != input_length)
                __create_task(db, input, message);

            free((char*)input);

            break;
        }
        case EDIT_TASK:
        {
            int task_id = __get_valid_user_int_input(1, INT_MAX, "Type the ID of the note: ");
            clear_console();

            if (!__print_task(db, task_id, message))
                break;

            const char* input = get_user_text_input("Type your updated note below.");
            const size_t input_length = strlen(input);

            if (input_length != 0 && strspn(input, NEWLINE) != input_length)
                __edit_task(db, task_id, input, message);

            free((char*)input);

            break;
        }
        case DELETE_TASK:
        {
            int task_id = __get_valid_user_int_input(1, INT_MAX, "Type the ID of the note: ");
            clear_console();

            if (!__print_task(db, task_id, message))
                break;

            printf("Are you sure you want to delete this note? Type %d to confirm: ", task_id);
            int input = __get_user_int_input(task_id, task_id);

            if (input == task_id)
                __delete_task(db, task_id, message);
            break;
        }
        case READ_TASK:
        {
            int task_id = __get_valid_user_int_input(1, INT_MAX, "Type the ID of the note: ");
            clear_console();

            if (__print_task(db, task_id, message))
                __prompt_and_wait("Press Enter to continue.");
            break;
        }
        case READ_ALL_TASKS:
            if (__print_all_tasks(db, message))
                __prompt_and_wait("Press Enter to continue.");
            break;
        default:
            strcpy(message, "Please, enter a valid option.");
            break;
    };

    return EXIT_SUCCESS;
}

static bool __create_task(const sqlite3* db, const char* task, char* message)
{
    bool inserted = insert_task(db, task);
    const char* returning_message = (inserted)
        ? "Note created successfully."
        : "An error occurred when attempting to create a note.";

    strcpy(message, returning_message);

    return inserted;
}

static bool __edit_task(const sqlite3* db, const int task_id, const char* task, char* message)
{
    bool updated = update_task(db, task_id, task);
    const char* returning_message = (updated)
        ? "Note updated successfully."
        : "An error occurred when attempting to update a note.";

    strcpy(message, returning_message);

    return updated;
}

static bool __delete_task(const sqlite3* db, const int task_id, char* message)
{
    bool deleted = delete_task(db, task_id);
    const char* returning_message = (deleted)
        ? "Note of ID %d has been successfully deleted."
        : "An error occurred when attempting to delete note of ID %d. Entry most likely was not found.";

    sprintf(message, returning_message, task_id);

    return deleted;
}

static bool __print_task(const sqlite3* db, const int task_id, char* message)
{
    db_task db_task = get_task(db, task_id);

    if (db_task.task == NULL)
    {
        sprintf(message, "Note of ID %d was not found.", task_id);
        return false;
    }

    __print_char('=', __frame_char_amount);
    printf(NEWLINE);

    printf("%s" NEWLINE, db_task.task);

    __print_char('=', __frame_char_amount);
    printf(NEWLINE);

    // Cleanup
    free_db_task(&db_task);

    return true;
}

static bool __print_all_tasks(const sqlite3* db, char* message)
{
    db_tasks db_tasks = get_all_tasks(db);

    if (db_tasks.amount <= 0)
    {
        strcpy(message, "No notes were found.");
        return false;
    }

    __print_char('=', __frame_char_amount);
    printf(NEWLINE);

    for (int index = 0; index < db_tasks.amount; index++)
        printf("--- Note ID: %d ---" NEWLINE "%s" NEWLINE, db_tasks.task_ids[index], db_tasks.tasks[index]);

    __print_char('=', __frame_char_amount);
    printf(NEWLINE);

    // Cleanup
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