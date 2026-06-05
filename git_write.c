#include "git_write.h"

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
                    const char *data, size_t size)
{
  // zlib compression
  uLong source_len = 20;

  uLong dest_len = compressBound(source_len);
  Bytef *dest = (Bytef *)malloc(dest_len);
  if (dest == NULL)
  {
    perror("Malloc error");
    return ERROR_CODE;
  }

  int result = compress(dest, &dest_len, (const Bytef *)data, source_len);
  if (result != Z_OK)
  {
    perror("Failed to compress file");
    return ERROR_CODE;
  }

  char final_path[512];
  snprintf(final_path, sizeof(final_path), dir_path, file_name);

  printf("%s", final_path);

  FILE *file = fopen(final_path, "wb");
  if (file == NULL)
  {
    perror("Error while opening file");
    return -1;
  }

  size_t written = fwrite(data, 1, size, file);
  fclose(file);

  if (written != size)
  {
    perror("Failed to write all the data into the file");
    return -1;
  }

  return 0;
}
