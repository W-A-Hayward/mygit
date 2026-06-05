// hash-object  →  write-tree  →  commit-tree  →  log  →  checkout

#include "git_write.h"
#include "hash_object.h"

int hash_object(int argc, char *argv[])
{
  if (argc != 2)
    return ERROR_CODE;

  struct stat st;
  stat(argv[1], &st);
  size_t size = st.st_size;

  char *buf[size];

  FILE *file = fopen(argv[1], "r");
  if (file == NULL)
  {
    perror("Error opening the file");
    return ERROR_CODE;
  }
  fread(buf, 1, size, file);
  fclose(file);

  // first part of header
  char header[64];
  int header_len = snprintf(header, sizeof(header), "blob %zu", size);
  header_len += 1; // to include the \0 we want

  // adding the rest
  size_t store_size = header_len + size;
  char *store = malloc(store_size);

  memcpy(store, header, header_len);
  memcpy(store + header_len, buf, size);

  // hashing
  unsigned char hash[20];
  SHA1((unsigned char *)store, store_size, hash);

  char hex_hash[41];
  for (int i = 0; i < 20; i++)
  {
    sprintf(&hex_hash[i * 2], "%02x", hash[i]);
  }

  // storing to dir
  char dir_name[3];
  char file_name[39];

  memcpy(dir_name, hex_hash, 2);
  dir_name[2] = '\0';

  memcpy(file_name, hex_hash + 2, 38);
  file_name[38] = '\0';

  const char *base_path = ".mygit/objects";
  char dir_path[256];
  char full_path[512];

  snprintf(dir_path, sizeof(dir_path), "%s/%s", base_path, dir_name);

  snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, file_name);

  mkdir(dir_path, 0755);

  if (errno == ENOENT)
    create_git_path(dir_path);

  create_git_file(full_path, file_name, (const char *)hash, 20);

  return 0;
}
