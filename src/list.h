#ifndef _LIST_H
#define _LIST_H

#include <stdint.h>

#define LIST_DECL(type)        \
typedef struct list_##type##_s \
{                              \
    type *data;                \
    uint64_t len;              \
    uint64_t cap;              \
} list_##type##_t;

#define LIST_DEF(type)                                             \
void list_##type##_resize(list_ ## type ## _t* list, uint64_t len) \
{                                                                  \
    /* blah */                                                     \
}

#endif