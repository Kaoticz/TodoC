#ifndef CORE_H // Only include this header file if it hasn't been included in the calling file already
    #define CORE_H

    #include <stdio.h>
    #include "../database/sqlite_db.h"
    #include "../handlers/signal_handlers.h"
    #include "../utilities/utilities.h"

    /// @brief The main loop of the program.
    /// @return Exit code.
    extern int app_loop();
#endif // CORE_H