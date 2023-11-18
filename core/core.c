#include "./core.h"

/* Function Prototypes */

/// @brief Prints the main menu of the program.
/// @param optional_message An optional message to be displayed at the top of the menu.
static void __print_menu(char* optional_message);

/// @brief Gets an integer input from the user within the specified range.
/// @param min The minimum integer expected.
/// @param max The maximum integer expected.
/// @return The integer input by the user. If the user attempted to input
/// @return an integer outside the specified range, it returns zero instead.
/// @return If zero is within the range, it returns "min - 1".
static int __get_user_int_input(int min, int max);

/// @brief Invokes the appropriate action according to the input provided by the user.
/// @param input The user's input.
/// @param message The message returned by the operation. May be NULL.
/// @return Zero if the operation executed successfuly or failed in a non-critical way,
/// @return non-zero if a critical error occurred and the program must shutdown.
static int __dispatcher(int input, char* message);

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
static void __print_char(char character, int amount);

/* Public Functions */

int app_loop()
{
    int status_code = 0, input = 0;
    char message[64] = { 0 };

    do
    {
        clear_console();
        __print_menu(message);

        input = __get_user_int_input(0, 5);
        status_code = __dispatcher(input, message);

        if (status_code != 0)
        {
            fprintf(stderr, message);
            return status_code;
        }

    } while (input != 0);

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
        "1. Create a new note." NEWLINE
        "2. Edit a note." NEWLINE
        "3. Delete a note." NEWLINE
        "4. Read a specific note." NEWLINE
        "5. Read all notes." NEWLINE
        "0. Exit." NEWLINE
    );
}

static int __get_user_int_input(int min, int max)
{
    if (min > max)
        swap(&min, &max);

    int input = 0;
    printf("> ");
    scanf("%d", &input);
    flush(stdin);

    return (input >= min && input <= max)
        ? input
        : (min <= 0)
            ? min - 1
            : 0;
}

static int __dispatcher(int input, char* message)
{
    int status_code = 0;
    clear_console();

    switch (input)
    {
        case 5:
            if (__print_all_tasks(message))
                __prompt_and_wait("Press Enter to continue.");
            break;
        default:
            strcpy(message, "Please, enter a valid option.");
            break;
    };

    return status_code;
}

static bool __print_all_tasks(char* message)
{
    const sqlite3* db = get_db();
    db_tasks db_tasks = get_all_tasks(db);

    if (db_tasks.amount <= 0)
    {
        strcpy(message, "No notes were found.");
        return false;
    }

    __print_char('=', 20);
    printf(NEWLINE);

    for (int index = 0; index < db_tasks.amount; index++)
        printf("--- Note %d ---" NEWLINE "%s" NEWLINE, db_tasks.task_ids[index], db_tasks.tasks[index]);

    __print_char('=', 20);
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

static void __print_char(char character, int amount)
{
    for (int count = 0; count < amount; count++)
        printf("%c", character);
}