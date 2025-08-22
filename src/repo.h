#ifndef _REPO_H
#define _REPO_H

#include <stdbool.h>

#include "sha256.h"

// is there a repo in the cwd?
bool checkrepo(void);
// is this object stored in the repo?
bool checkhash(shahash_t hash);
// is the given path within the cwd?
bool pathinrepo(const char* path);
// outpath should be at least char[PATH_MAX]
void pathrelativetorepo(const char* path, char *outpath);

#endif