
#include <errno.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <zlib.h>
// read file → buffer + size
// build "blob <size>\0" header
// concatenate header + buffer
// SHA-1 the concatenation → 40 hex chars
// zlib compress the concatenation
// mkdir .git/objects/xx if needed
// write compressed bytes to .git/objects/xx/yyyy...
// print the 40 hex chars

#define ERROR_CODE -1

int hash_object(int argc, char *argv[]);
