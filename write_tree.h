#include "git_write.h"
#include "hash_object.h"
#include <dirent.h>
#include <stdbool.h>
#include <string.h>

#define ASCII 100644
#define EXECUTABLE 100755
#define DIRECTORY 40000
#define BUF_SIZE 1024

bool contains(char *arr[], char *entry, int size);
char *write_tree(char *path_to_dir);
