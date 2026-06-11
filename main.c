// hash-object  →  write-tree  →  commit-tree  →  log  →  checkout
#include "hash_object.h"
#include "init.h"
#include "write_tree.h"

typedef int (*command_fn)(int argc, char *argv[]);

typedef struct
{
  const char *name;
  command_fn fn;
} Command;

static void usage(void)
{
  fprintf(stderr, "usage: mygit <command> [<args>...]\n"
                  "commands: init, hash-object, write-tree\n");
}

static int cmd_init(int argc, char *argv[])
{
  (void)argc;
  (void)argv;
  return init() == 0 ? 0 : 1;
}

static const Command commands[] = {
    {"init", cmd_init},
    // {"hash-object", cmd_hash_object},
    // {"write-tree", cmd_write_tree},
    {NULL, NULL},
};

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    usage();
    return 1;
  }

  for (const Command *c = commands; c->name; c++)
  {
    if (strcmp(argv[1], c->name) == 0)
      return c->fn(argc, argv);
  }

  fprintf(stderr, "unknown command: %s\n", argv[1]);
  usage();
  return 1;
}
