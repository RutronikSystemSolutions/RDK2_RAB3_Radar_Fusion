#ifndef TYPEUTILS_H
#define TYPEUTILS_H 1

#include <stddef.h>

#define ARRAY_SIZE(a)         (sizeof(a) / sizeof(a[0]))
#define ARRAY_SIZE_CHECKED(a) (((a == NULL) ? 0 : sizeof(a)) / sizeof(a[0]))


#define LIST_SIZE(t, a) (sizeof((const t[]) {a}) / sizeof(const t))


#define ALIGNED_BUFFER(type, name, alignment_type, length)                                                \
    static alignment_type name##_aligned[(length + sizeof(alignment_type) - 1) / sizeof(alignment_type)]; \
    static type *name = (type *)name##_aligned;


#endif /* TYPEUTILS_H */
