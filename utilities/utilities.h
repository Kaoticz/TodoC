#ifndef UTILITIES_H // Only include this header file if it hasn't been included in the calling file already
    #define UTILITIES_H

    #include <string.h>
    #include <stdbool.h>

    #ifdef _WIN32
        #include <windows.h>
        #include <stdio.h>

        /// @brief The maximum length a directory path can have.
        #define MAX_PATH_LENGTH MAX_PATH

        /// @brief The character that separates directories in a file system path.
        #define DIRECTORY_SEPARATOR_CHAR '\\'

        /// @brief The character that separates directories in a file system path.
        #define DIRECTORY_SEPARATOR "\\"
    #elif __unix__ // _WIN32
        #include <linux/limits.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <unistd.h>
        #include <limits.h>

        /// @brief The maximum length a directory path can have.
        #define MAX_PATH_LENGTH PATH_MAX

        /// @brief The character that separates directories in a file system path.
        #define DIRECTORY_SEPARATOR_CHAR '/'

        /// @brief The character that separates directories in a file system path.
        #define DIRECTORY_SEPARATOR "/"
    #else   // __unix__
        #error "This platform is not supported."
    #endif

        /// @brief Gets the absolute path to the executable file of this program.
        /// @attention Must be manually deallocated!
        /// @return A pointer to the string with the file path.
        extern const char* get_executable_path();

        /// @brief Gets the absolute path to the directory executable file of this program.
        /// @attention Must be manually deallocated!
        /// @return A pointer to the string with the directory path.
        extern const char* get_executable_directory();

        /// @brief Appends the *source* string into the destination string.
        /// @param destination The string to be appended to.
        /// @param source The string to append to.
        /// @attention Must be manually deallocated!
        /// @attention The strings passed in the parameters remain unchanged.
        /// @return A string that contains the content of both destination and source.
        extern const char* str_append(const char* destination, const char* source);

        /// @brief Checks if a file exists in the specified location.
        /// @param file_path The absolute path to the file.
        /// @return True if the file exists, False otherwise.
        extern bool file_exists(const char* file_path);

        /// @brief Creates an empty file in the specified location.
        /// @param file_path The absolute path to the new file.
        /// @return True if the file was successfully created, False otherwise.
        extern bool create_empty_file(const char* file_path);
#endif // UTILITIES_H