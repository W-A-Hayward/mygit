// hash-object  →  write-tree  →  commit-tree  →  log  →  checkout
#include "hash_object.h"
#include "write_tree.h"

int main(int argc, char *argv[])
{
  (void)argc;
  (void)argv;

  unsigned char hash[20];
  write_tree("./test", hash);

  return 0;
}
