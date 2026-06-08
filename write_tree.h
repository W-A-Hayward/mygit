#include "arena.h"
#include "git_write.h"
#include "hash_object.h"
#include <dirent.h>
#include <stdbool.h>
#include <string.h>

#define NORMAL_FILE 100644
#define EXECUTABLE 100755
#define DIRECTORY 40000
#define SYMLINK 120000
#define UNKNOWN 000000

#define BUF_SIZE 1024

typedef struct
{
  size_t mode;
  char *name;
  unsigned char hash[20];
} Entry;

typedef struct
{
  Entry *items;
  size_t count;
  size_t cap;
} EntryList;

bool contains(char *arr[], char *entry, int size);
size_t get_git_mode(struct stat *st);
void write_tree(char *path_to_dir, unsigned char hash_out[20]);
