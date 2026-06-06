// Open the working directory and iterate over every entry (dirent).
// opendir, readdir, closedir
// Skip .git / .mygit and any other entries you don't want.
// For each entry:
// If it's a file: compute the blob hash for it (exactly what hash-object does)
// use stat/lstat and store it in the object store. If it's a directory:
// recursively call write-tree on it, which returns a tree SHA-1. Collect all
// entries — each one has a mode, a name, and a 20-byte binary SHA. Assemble the
// raw content by concatenating all entries in the format above. Wrap it with
// the tree <size>\0 header. Hash, compress, and store — same as hash-object.
// Print (or return) the 40-char hex hash.

// for storing the entries, a dynamic allocator is best (why not arena allocator
// ! see git's mem_pool)

char *write_tree(char *path_to_dir) {}
