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

size_t get_git_mode(struct stat *st)
{
  if (S_ISLNK(st->st_mode))
  {
    return SYMLINK;
  }
  else if (S_ISDIR(st->st_mode))
  {
    return DIRECTORY;
  }
  else if (S_ISREG(st->st_mode))
  {
    // Check if executable
    if (st->st_mode & S_IXUSR)
    {
      return EXECUTABLE;
    }
    else
    {
      return NORMAL_FILE;
    }
  }
  return UNKNOWN; // Unsupported or unknown type
}

int compare_entries(const void *a, const void *b)
{
  return strcmp(((Entry *)a)->name, ((Entry *)b)->name);
}

void write_tree(char *path_to_dir, unsigned char hash_out[20])
{
  char *dir_ignore[] = {".", "..", ".mygit", ".git"};
  int ignore_size = sizeof(dir_ignore) / sizeof(dir_ignore[0]);

  DIR *dir = opendir(path_to_dir);
  if (dir == NULL)
  {
    perror("Failed to open directory");
    return;
  }

  // Initialize the arena for this directory level
  Arena arena = arena_init(64 * BUF_SIZE);

  // Keep track of an array of entries in our arena
  int entry_count = 0;
  int entry_capacity = 16;
  Entry *entries = arena_alloc(&arena, entry_capacity * sizeof(Entry), 1);

  struct dirent *dir_entry;
  struct stat sb;
  char full_path[BUF_SIZE];

  while ((dir_entry = readdir(dir)) != NULL)
  {
    if (contains(dir_ignore, dir_entry->d_name, ignore_size))
      continue;

    snprintf(full_path, sizeof(full_path), "%s/%s", path_to_dir,
             dir_entry->d_name);

    if (stat(full_path, &sb) != 0)
    {
      perror("stat failed");
      continue;
    }

    if (entry_count >= entry_capacity)
    {
      int old_capacity = entry_capacity;
      entry_capacity *= 2;
      Entry *new_entries =
          arena_alloc(&arena, entry_capacity * sizeof(Entry), 1);
      memcpy(new_entries, entries, old_capacity * sizeof(Entry));
      entries = new_entries;
    }

    Entry *current_entry = &entries[entry_count];

    size_t name_len = strlen(dir_entry->d_name);
    current_entry->name = arena_alloc(&arena, name_len + 1, 1);
    strcpy(current_entry->name, dir_entry->d_name);
    current_entry->mode = get_git_mode(&sb);

    if (S_ISREG(sb.st_mode))
    {
      // Process Leaf
      hash_object(full_path, current_entry->hash);
      entry_count++;
    }
    else if (S_ISDIR(sb.st_mode))
    {
      // DFS Post-Order Recurse
      write_tree(full_path, current_entry->hash);
      entry_count++;
    }
  }
  closedir(dir);

  // Process the collected elements of this directory
  if (entry_count > 0)
  {
    // Git requires entries to be sorted alphabetically
    qsort(entries, entry_count, sizeof(Entry), compare_entries);

    size_t total_serialized_bytes = 0;
    for (int i = 0; i < entry_count; i++)
    {
      // Format: "[mode] [name]\0[20 bytes of binary hash]"
      char mode_buf[32];
      snprintf(mode_buf, sizeof(mode_buf), "%zu ", entries[i].mode);
      total_serialized_bytes +=
          strlen(mode_buf) + strlen(entries[i].name) + 1 + 20;
    }

    // Allocate buffer to hold the raw tree contents
    unsigned char *tree_buffer = arena_alloc(&arena, total_serialized_bytes, 1);
    unsigned char *ptr = tree_buffer;

    for (int i = 0; i < entry_count; i++)
    {
      int written =
          sprintf((char *)ptr, "%zu %s", entries[i].mode, entries[i].name);
      ptr += written + 1;

      memcpy(ptr, entries[i].hash, 20);
      ptr += 20;
    }

    hash_buffer(tree_buffer, total_serialized_bytes, hash_out);

    // Optional: Save this tree object to disk (like hash_object does for files)
    // write_tree_object_to_disk(hash_out, tree_buffer, total_serialized_bytes);
  }
  else
  {
    // Handle empty directory case (Git generally ignores empty directories)
    memset(hash_out, 0, 20);
  }

  // Free all memory used by this directory level
  arena_free(&arena);
}
