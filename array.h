#include <stddef.h>
#ifndef ARRAY_H_
#define ARRAY_H_ 

typedef struct Array *Array;

extern Array array_new(size_t length, int size);
extern void array_free(Array *a);

extern int array_length(Array a);
extern int array_size(Array a);

extern void *array_get(Array a, size_t i);
extern void *array_put(Array a, size_t i, void *elem);
extern void *array_put_auto(Array a, size_t i, void *elem);

extern void array_resize(Array a, size_t length);
extern void array_swap(Array a, size_t i, size_t j);
extern Array array_copy(Array source);

#endif
