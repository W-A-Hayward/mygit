#include <errno.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <zlib.h>
// read file → buffer + size
// build "blob <size>\0" header
// concatenate header + buffer
// SHA-1 the concatenation → 40 hex chars
// zlib compress the concatenation
// mkdir .git/objects/xx if needed
// write compressed bytes to .git/objects/xx/yyyy...
// print the 40 hex chars

int create_git_path(const char *full_path);
int create_git_file(const char *dir_path, const char *file_name,
                    const char *data, size_t size);
int hash_object(int argc, char *argv[]);
