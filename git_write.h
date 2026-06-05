#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <zlib.h>

#define ERROR_CODE -1

int create_git_path(const char *full_path);
int create_git_file(const char *dir_path, const char *file_name,
                    const char *data, size_t size);
