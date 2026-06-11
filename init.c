#include "init.h"

#if defined(_WIN32) || defined(_WIN64)
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#define DELIMITER "\\"
#else
#include <sys/types.h>
#define MKDIR(path) mkdir(path, 0777)
#define DELIMITER "/"
#endif

#define BUF_SIZE 512

int create_folder(const char *path)
{
  char tmp[BUF_SIZE];
  char *p = NULL;
  size_t len;

  snprintf(tmp, sizeof(tmp), "%s", path);
  len = strlen(tmp);

  if (tmp[len - 1] == '/' || tmp[len - 1] == '\\')
  {
    tmp[len - 1] = 0;
  }

  for (p = tmp + 1; *p; p++)
  {
    if (*p == '/' || *p == '\\')
    {
      char temp_char = *p;
      *p = 0; // Temporarily cut the string

      MKDIR(tmp);

      *p = temp_char; // Restore the character
    }
  }
  return MKDIR(tmp);
}

int create_file(char *path, char *name, char *contents)
{
  create_folder(path);

  char full_path[BUF_SIZE];
  snprintf(full_path, sizeof(full_path), "%s%s%s", path, DELIMITER, name);

  FILE *file_ptr = fopen(full_path, "w");
  if (file_ptr == NULL)
  {
    return -1;
  }

  fprintf(file_ptr, "%s\n", contents);
  fclose(file_ptr);
  return 0;
}

int write_head()
{
  return create_file("./.mygit", "HEAD", "ref: refs/heads/main\n");
}

int write_config(char *username, char *email)
{
  char contents[BUF_SIZE];
  snprintf(contents, sizeof(contents), "user.name: %s\nuser.email: %s\n",
           username, email);

  return create_file("./.mygit", "config", contents);
}

int write_empty_obj() { return create_folder("./.mygit/objects"); }

int write_empty_refs() { return create_folder("./.mygit/refs/heads"); }

int init()
{
  if (create_folder("./.mygit") < 0)
    return -1;
  if (write_head() < 0)
    return -1;
  if (write_config("W-A-Hayward", "whaywardca@gmail.com") < 0)
    return -1;
  if (write_empty_obj() < 0)
    return -1;
  if (write_empty_refs() < 0)
    return -1;
  return 0;
}
