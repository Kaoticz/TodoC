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

    char *lastSlash = strrchr(full_path, DIRECTORY_SEPARATOR_CHAR);

    if (lastSlash != NULL)
        *lastSlash = '\0';

    return full_path;
}

const char* str_append(const char* destination, const char* source)
{
    int dest_length = strlen(destination) * sizeof(char);
    int source_length = strlen(source) * sizeof(char);
    char *result = malloc(dest_length + source_length);
    
    mempcpy(result, destination, dest_length);
    mempcpy(result + dest_length, source, source_length);

    return result;
}

bool file_exists(const char *file_path)
{
    FILE *file = fopen(file_path, "r");

    if (file == NULL)
        return false;

    fclose(file);
    return true;
}

bool create_empty_file(const char *file_path)
{
    if (file_exists(file_path))
        return false;

    FILE *file = fopen(file_path, "w");

    if (file == NULL)
        return false;

    fclose(file);
    return true;
}