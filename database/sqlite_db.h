#ifndef SQLITEDB_H // Only include this header file if it hasn't been included in the calling file already
    #define SQLITEDB_H

    #include <sqlite3.h>

    typedef struct sqlite_db
    {
        char* connection_string;
    };

#endif // SQLITEDB_H