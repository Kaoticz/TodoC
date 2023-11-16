#include "./utilities.h"

/* Public Functions */

const char* get_executable_path()
{
    char buffer[MAX_PATH_LENGTH];

#ifdef _WIN32
    DWORD length = GetModuleFileName(NULL, buffer, MAX_PATH);
#else
    ssize_t length = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
#endif

    if (length <= 0)
        return NULL;

    buffer[length] = '\0';

    // Create the result
    char* result = malloc(length);
    memcpy(result, buffer, length);

    return result;
}

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