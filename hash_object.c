#include "hash_object.h"
#include "git_write.h"

void hash_buffer(const unsigned char *buffer, size_t size,
                 unsigned char hash_out[20])
{
  char header[64];
  int header_len = snprintf(header, sizeof(header), "tree %zu", size);
  header_len += 1;

  size_t store_size = (size_t)header_len + size;
  unsigned char *store = (unsigned char *)malloc(store_size);
  if (store == NULL)
  {
    perror("Malloc error");
    return;
  }

  memcpy(store, header, (size_t)header_len);
  memcpy(store + header_len, buffer, size);

  write_git_object(store, store_size, hash_out);
  free(store);
}

void hash_object(char *path, unsigned char hash_out[20])
{
  struct stat st;
  stat(path, &st);
  size_t size = st.st_size;

  char *buf[size];

  FILE *file = fopen(path, "r");
  if (file == NULL)
  {
    perror("Error opening the file");
    exit(EXIT_FAILURE);
  }
  fread(buf, 1, size, file);
  fclose(file);

  char header[64];
  int header_len = snprintf(header, sizeof(header), "blob %zu", size);
  header_len += 1;

  size_t store_size = header_len + size;
  char *store = malloc(store_size);

  memcpy(store, header, header_len);
  memcpy(store + header_len, buf, size);

  write_git_object((unsigned char *)store, store_size, hash_out);
  free(store);
}
