#ifndef SIGHANDLERS_H // Only include this header file if it hasn't been included in the calling file already
    #define SIGHANDLERS_H

    #include <stdio.h>
    #include <signal.h>

    /// @brief Registers all signal handlers.
    void register_signal_handlers();

    /// @brief Executes an action when Ctrl + Z is pressed.
    /// @param sigtstp_num The signal number for SIGTSTP.
    extern void sigtstp_handler(int);
#endif // SIGHANDLERS_H