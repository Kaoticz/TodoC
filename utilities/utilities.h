#ifndef UTILITIES_H // Only include this header file if it hasn't been included in the calling file already
    #define UTILITIES_H

    #include <string.h>
    #include <stdbool.h>
    #include <time.h>

    #ifdef _WIN32
        #include <windows.h>
        #include <stdio.h>

        /// @brief The maximum length a directory path can have.
        #define MAX_PATH_LENGTH MAX_PATH

        /// @brief The character that separates directories in a file system path.
        #define DIRECTORY_SEPARATOR_CHAR '\\'

        /// @brief The character that separates directories in a file system path.
        #define DIRECTORY_SEPARATOR "\\"

        /// @brief The platform-specific newline.
        #define NEWLINE "\r\n"
    #elif __unix__ // _WIN32
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

        /// @brief The platform-specific newline.
        #define NEWLINE "\n"
    #else   // __unix__
        #error "This platform is not supported."
    #endif

    /// @brief Marks multiple variables as unused.
    #define UNUSED_VAR(...) (void)__VA_ARGS__

    #ifdef __GNUC__
        /// @brief Marks a function parameter as unused.
        #define UNUSED __attribute__((__unused__))
    #else
        /// @brief Marks a function parameter as unused.
        #define UNUSED UNUSED_##x
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

    /// @brief Checks if the specified file is empty.
    /// @param file_path The absolute path to the file.
    /// @return True if the file is empty, False otherwise.
    extern bool is_file_empty(const char *file_path);

    /// @brief Creates an empty file in the specified location.
    /// @param file_path The absolute path to the new file.
    /// @return True if the file was successfully created, False otherwise.
    extern bool create_empty_file(const char *file_path);

    /// @brief Gets the current time in Unix seconds.
    /// @return The Unix time in seconds.
    extern time_t get_current_time();

    /// @brief Clears the console.
    /// @return The exit code of the clear command.
    extern int clear_console();

    /// @brief Swaps the values of two variables with each other.
    /// @param x The first variable.
    /// @param y The second variable.
    extern void swap(int* x, int* y);

    /// @brief Flushes a stream.
    /// @attention If flushing "stdin" when it's empty, the program waits for user input.
    /// @param stream The stream to be flushed.
    /// @return The number of characters removed from the stream or -1 if an error
    /// @return occurred (check feof() for "stdin" or ferror() for any other stream).
    extern int flush(FILE* stream);
#endif // UTILITIES_H