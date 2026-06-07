// hash-object  →  write-tree  →  commit-tree  →  log  →  checkout
#include "hash_object.h"
#include "write_tree.h"

int main(int argc, char *argv[])
{
  write_tree(".");

  return 1;
}
