#include "./signal_handlers.h"

/* Private Variables */

static bool __typing_stop_flag = false;

/* Function Prototyping */

/// @brief Executes an action when Ctrl + Z is pressed.
/// @param sigtstp_num The signal number for SIGTSTP.
static void __sigtstp_handler(int);

/* Public Functions */

void start_sigtstp_handler()
{
    signal(SIGTSTP, __sigtstp_handler);
    __typing_stop_flag = true;
}

bool is_typing_allowed()
{
    return __typing_stop_flag;
}

/* Private Functions */

static void __sigtstp_handler(int sigtstp_num)
{
    UNUSED(sigtstp_num);
    __typing_stop_flag = false;
}