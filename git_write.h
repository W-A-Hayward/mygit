#include <stddef.h>
#include <stdio.h>
#include <sys/stat.h>
#include <zlib.h>

#define ERROR_CODE -1

int create_git_path(const char *full_path);
int create_git_file(const char *dir_path, const char *file_name,
                    const unsigned char *data, size_t size);
int write_git_object(const unsigned char *content, size_t content_size,
                     unsigned char hash_out[20]);
