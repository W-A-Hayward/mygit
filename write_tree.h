#include "git_write.h"
#include "hash_object.h"

#define ASCII 100644
#define EXECUTABLE 100755
#define DIRECTORY 40000

char *write_tree(char *path_to_dir);
