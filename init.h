#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int init();

// --- General helpers
int create_folder(const char *path);
int create_file(char *path, char *name, char *contents);

// --- Init specific helpers
int write_head();
int write_config(char *username, char *email);
int write_empty_obj();
int write_empty_refs();
