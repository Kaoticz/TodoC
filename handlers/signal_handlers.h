#ifndef SIGHANDLERS_H // Only include this header file if it hasn't been included in the calling file already
    #define SIGHANDLERS_H

    #include <signal.h>
    #include "../utilities/utilities.h"

    /// @brief Starts the handler for the SIGTSTP signal, indicating
    /// @brief that the user doesn't want to type anymore.
    /// @attention Activated through Ctrl + Z.
    extern void start_sigtstp_handler();

    /// @brief Gets the flag that indicates typing should stop.
    /// @return True if typing is allowed, False otherwise.
    extern bool is_typing_allowed();
#endif // SIGHANDLERS_H