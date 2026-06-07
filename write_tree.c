// Open the working directory and iterate over every entry (dirent).
// opendir, readdir, closedir
// Skip .git / .mygit and any other entries you don't want.
// For each entry:
// If it's a file: compute the blob hash for it (exactly what hash-object does)
// use stat/lstat and store it in the object store. If it's a directory:
// recursively call write-tree on it, which returns a tree SHA-1. Collect all
// entries — each one has a mode, a name, and a 20-byte binary SHA. Assemble the
// raw content by concatenating all entries in the format above. Wrap it with
// the tree <size>\0 header. Hash, compress, and store — same as hash-object.
// Print (or return) the 40-char hex hash.

// for storing the entries, a dynamic allocator is best (why not arena allocator
// ! see git's mem_pool)
#include "write_tree.h"

bool contains(char *arr[], char *entry, int size)
{
  for (int i = 0; i < size; i++)
  {
    if (strcmp(arr[i], entry) == 0)
      return true;
  }
  return false;
}

char *write_tree(char *path_to_dir)
{
  char *dir_ignore[] = {".", "..", ".mygit", ".git"};

  struct stat sb;
  int size = sizeof(dir_ignore) / sizeof(dir_ignore[0]);
  DIR *dir = opendir(path_to_dir);

  char **entries = malloc(sizeof(char) * BUF_SIZE);
  int position = 0;

  struct dirent *dir_entry;
  char full_path[BUF_SIZE];

  if (dir != NULL)
  {
    while ((dir_entry = readdir(dir)) != NULL)
    {
      if (!contains(dir_ignore, dir_entry->d_name, size))
      {
        snprintf(full_path, sizeof(full_path), "%s/%s", path_to_dir,
                 dir_entry->d_name);

        if (stat(full_path, &sb) != 0)
          perror("stat failed");

        if (S_ISREG(sb.st_mode))
        {
          hash_object(full_path, entries[position]);
          position++;
        }
        else if (S_ISDIR(sb.st_mode))
        {
          write_tree(full_path);
        }

        // TODO: check if we are at the end of entries buf and call arena alloc
      }
    }
    closedir(dir);
  }
  else
  {
    perror("Failed to open file");
  }

  return "/";
}
