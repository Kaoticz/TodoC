#include "./utilities.h"

/* Public Functions */

/// @brief Gets the absolute path to the executable file of this program.
/// @remark Must be manually deallocated!
/// @return A pointer to the string with the file path.
const char* get_executable_path()
{
    char buffer[MAX_PATH_LENGTH];
    ssize_t length = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);

    if (length <= 0)
        return NULL;

    buffer[length] = '\0';

    // Create the result
    char* result = malloc(length);
    memcpy(result, buffer, length);

    return result;
}

/// @brief Gets the absolute path to the directory executable file of this program.
/// @return A pointer to the string with the directory path.
/// @remarks Must be manually deallocated!
const char* get_executable_directory()
{
    const char* full_path = get_executable_path();

    if (full_path == NULL)
        return NULL;

    char *lastSlash = strrchr(full_path, DIRECTORY_SEPARATOR);

    if (lastSlash != NULL)
        *lastSlash = '\0';

    return full_path;
}