#ifndef UTILITIES_H // Only include this header file if it hasn't been included in the calling file already
#define UTILITIES_H

#include <string.h>

#ifdef _WIN32
    #include <Windows.h>

    /// @brief The maximum length a directory path can have.
    #define MAX_PATH_LENGTH MAX_PATH

    /// @brief The character that separates directories in a file system path.
    #define DIRECTORY_SEPARATOR '\\'
#elif __unix__ // _WIN32
    #include <linux/limits.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <limits.h>

    /// @brief The maximum length a directory path can have.
    #define MAX_PATH_LENGTH PATH_MAX

    /// @brief The character that separates directories in a file system path.
    #define DIRECTORY_SEPARATOR '/'
#endif // __unix__

    /// @brief Gets the absolute path to the executable file of this program.
    /// @attention Must be manually deallocated!
    /// @return A pointer to the string with the file path.
    extern const char* get_executable_path();

    /// @brief Gets the absolute path to the directory executable file of this program.
    /// @attention Must be manually deallocated!
    /// @return A pointer to the string with the directory path.
    extern const char* get_executable_directory();
#endif // UTILITIES_H