#include "git_write.h"

#include <errno.h>
#include <openssl/sha.h>
#include <stdlib.h>
#include <string.h>

int create_git_path(const char *full_path)
{
  char *sub_path = strdup(full_path);
  if (!sub_path)
    return -1;

  char *p = sub_path;

  if (*p == '/')
    p++;

  while (*p)
  {
    if (*p == '/')
    {
      *p = '\0';

      if (mkdir(sub_path, 0755) != 0 && errno != EEXIST)
      {
        free(sub_path);
        return -1;
      }

      *p = '/';
    }
    p++;
  }

  if (mkdir(sub_path, 0755) != 0 && errno != EEXIST)
  {
    free(sub_path);
    return -1;
  }

  free(sub_path);
  return 1;
}

int create_git_file(const char *dir_path, const char *file_name,
                    const unsigned char *data, size_t size)
{
  uLong dest_len = compressBound((uLong)size);
  Bytef *dest = (Bytef *)malloc(dest_len);
  if (dest == NULL)
  {
    perror("Malloc error");
    return ERROR_CODE;
  }

  if (compress(dest, &dest_len, data, (uLong)size) != Z_OK)
  {
    perror("Failed to compress file");
    free(dest);
    return ERROR_CODE;
  }

  char final_path[512];
  snprintf(final_path, sizeof(final_path), "%s/%s", dir_path, file_name);

  FILE *file = fopen(final_path, "wb");
  if (file == NULL)
  {
    perror("Error while opening file");
    free(dest);
    return ERROR_CODE;
  }

  size_t written = fwrite(dest, 1, dest_len, file);
  fclose(file);
  free(dest);

  if (written != dest_len)
  {
    perror("Failed to write all the data into the file");
    return ERROR_CODE;
  }

  return 0;
}

int write_git_object(const unsigned char *content, size_t content_size,
                     unsigned char hash_out[20])
{
  SHA1(content, content_size, hash_out);

  char hex_hash[41];
  for (int i = 0; i < 20; i++)
    sprintf(&hex_hash[i * 2], "%02x", hash_out[i]);

  char dir_path[256];
  char file_name[39];
  memcpy(file_name, hex_hash + 2, 38);
  file_name[38] = '\0';

  snprintf(dir_path, sizeof(dir_path), ".mygit/objects/%.2s", hex_hash);

  if (mkdir(dir_path, 0755) != 0 && errno != EEXIST)
  {
    if (create_git_path(dir_path) != 1)
      return ERROR_CODE;
  }

  return create_git_file(dir_path, file_name, content, content_size);
}
