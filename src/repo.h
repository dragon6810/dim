#ifndef _REPO_H
#define _REPO_H

#include <stdbool.h>

#include "sha256.h"

// is there a repo in the cwd?
bool checkrepo(void);
// is this object stored in the repo?
bool checkhash(shahash_t hash);

#endif