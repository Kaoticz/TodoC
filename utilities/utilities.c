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
    {
        fprintf(stderr, "Could not find the path to this executable." NEWLINE);
        return NULL;
    }

    buffer[length] = '\0';

    // Create the result
    char* result = malloc(length + 1);
    memcpy(result, buffer, length);

    result[length] = '\0';

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
    size_t dest_length = strlen(destination);
    size_t source_length = strlen(source);
    char* result = malloc(dest_length + source_length + 1);
    
    mempcpy(result, destination, dest_length);
    mempcpy(result + dest_length, source, source_length);

    result[dest_length + source_length] = '\0';

    return result;
}

bool file_exists(const char* file_path)
{
    FILE* file = fopen(file_path, "r");

    if (file == NULL)
        return false;

    fclose(file);
    return true;
}

bool is_file_empty(const char* file_path)
{
    FILE *file = fopen(file_path, "rb");

    if (file == NULL)
    {
        fprintf(stderr, "Could not open file at \"%s\"" NEWLINE, file_path);
        return false;
    }

    fseek(file, 0, SEEK_END); // Move the file pointer to the end
    long size = ftell(file);  // Get the current position, which is the file size
    fclose(file);

    return (size > 0) ? false : true;
}

bool create_empty_file(const char* file_path)
{
    if (file_exists(file_path))
        return false;

    FILE* file = fopen(file_path, "w");

    if (file == NULL)
    {
        fprintf(stderr, "Could not create file at \"%s\"" NEWLINE, file_path);
        return false;
    }

    fclose(file);
    return true;
}

time_t get_current_time()
{
    time_t current_time = time(NULL);

    if (current_time != (time_t) - 1)
        return current_time;

    fprintf(stderr, "Could not get the current time" NEWLINE);
    return 0;
}

int clear_console()
{
    #ifdef _WIN32
        return system("cls");
    #else
        return system("clear");
    #endif
}

void swap(int* x, int* y)
{
    int temp = *y;
    *y = *x;
    *x = temp;
}

int flush(FILE* stream)
{
    int counter = 0;
    char character;
    const char stop_char = (stream == stdin) ? '\n' : EOF;

    while ((character = fgetc(stream)) != stop_char && character != EOF)
        counter++;

    if (character == '\n')
        counter++;
    else if (character == EOF)
        counter = -1;

    return counter;
}

void dev_null(int count, ...)
{
    (void)(count);
}

int max(const int x, const int y)
{
    return (x > y) ? x : y;
}

int min(const int x, const int y)
{
    return (x < y) ? x : y;
}