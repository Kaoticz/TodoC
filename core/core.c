#include "./core.h"

/* Function Prototypes */

/// @brief Prints the main menu of the program.
/// @param optional_message An optional message to be displayed at the top of the menu.
static void __print_menu(const char* optional_message);

/// @brief Gets an integer input from the user within the specified range.
/// @param min The minimum integer expected.
/// @param max The maximum integer expected.
/// @return The integer input by the user. If the user attempted to input
/// @return an integer outside the specified range, it returns zero instead.
/// @return If zero is within the range, it returns "min - 1".
static int __get_user_int_input(int min, int max);

/* Public Functions */

int app_loop()
{
    int status_code = 0, input = 0;
    char error_message[64] = { 0 };

    do
    {
        clear_console();
        __print_menu((const char*)error_message);

        input = __get_user_int_input(0, 5);
        status_code = __dispatcher(input, error_message);

        if (status_code != 0)
        {
            fprintf(stderr, error_message);
            return status_code;
        }

    } while (input != 0);

    clear_console();

    return 0;
}

/* Private Functions */

static void __print_menu(const char* optional_message)
{
    if (optional_message != NULL && optional_message[0] != '\0')
        printf("%s" NEWLINE NEWLINE, optional_message);

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

    return (input >= min && input <= max)
        ? input
        : (min <= 0)
            ? min - 1
            : 0;
}

static int __dispatcher(int input, char* error_message)
{
    switch (input)
    {
        case 5:

            break;
    };

    return 0;
}