#include "array.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct Array {
  size_t length;
  int size;
  void * arr;
};

/* array_create() and array_free() are the methods for creating and cleaning 
 * up our array object. array_create() takes the number of objects that the 
 * initial array can hold, and the size of that object in bytes. */ 

Array array_create(size_t length, int size)
{
  assert(length >= 0);

  Array new_array = malloc(sizeof(*new_array));
  assert(new_array);

  new_array->size = size;
  new_array->length = length;

  if (length > 0)
  {
    new_array->arr = calloc(length, size);
    assert(new_array->arr);
  }
  else
  {
    new_array->arr = NULL;
  }

  return new_array;
}

void array_free(Array *a)
{
  assert(a && *a);
  if ((*a)->arr)
  {
    free((*a)->arr);
  }

  free(*a);
  *a = NULL;
}

int array_length(Array a)
{
  assert(a);
  return a->length;
}

int array_size(Array a)
{
  assert(a);
  return a->size;
}

/* array_get(), array_put(), and array_put_auto() access the underlying array
 * and either get the value at the given index or put the object pointed to via
 * 'elem' into the array at the given index. It is a checked runtime error to 
 * give an index to either array_get() or array_put() that is outside the 
 * bounds of the array length, but array_put_auto() will automatically resize
 * the array if given an index that is greater than the current length of the 
 * array. */ 

void* array_get(Array a, size_t i)
{
  assert(a);
  assert(i >= 0 && i < a->length);
  return a->arr + (a->size * i);
}

void* array_put(Array a, size_t i, void *elem)
{
  assert(a);
  assert(i >= 0 && i < a->length);

  assert(elem);
  memcpy(a->arr + (a->size * i), elem, a->size);
  return elem;
}

void* array_put_auto(Array a, size_t i, void * elem)
{
  assert(a);
  assert(elem);

  if (i >= a->length)
  {
    size_t newLength = (i > a->length*2) ? i : a->length*2;
    array_resize(a, (newLength));
  }

  memcpy(a->arr + (a->size * i), elem, a->size);
  return elem;
}

/* If the new length given is less than the current length of the array given,
 * then the given array is truncated to the new length; if the new length is 
 * greater than the old length, the array is expanded to the new length and 
 * the new memory is written over with 0's. */ 

void array_resize(Array a, size_t length)
{
  assert(a);

  if (length == 0)
  {
    free(a->arr);
    a->arr = NULL;
  }
  else if (a->length == 0) 
  {
    a->arr = calloc(length, a->size);
    assert(a->arr);
  }
  else 
  {
    void* tmp = realloc(a->arr, length*a->size);
    if (tmp)
    {
      a->arr = tmp;
    }
    else
    {
      printf("Couldn't allocate memory; Exiting...\n");
      exit(EXIT_FAILURE);
    }

    a->arr = realloc(a->arr, length*a->size);
    
    if (length > a->length)
    {
      int diff = length - a->length;
      memset(a->arr + a->length*a->size, 0, diff*a->size);
    }
  }
  a->length = length;
}

void array_swap(Array a, size_t i, size_t j)
{
  assert(a);
  assert(a->arr);
  assert(i < a->length);
  assert(j < a->length);

  if (i == j)
  {
    return;
  }

  void *tmp = malloc(a->size);
  assert(tmp);

  memcpy(tmp, array_get(a, i), a->size);
  memcpy(array_get(a, i), array_get(a, j), a->size);
  memcpy(array_get(a, j), tmp, a->size);
  
  free(tmp);
}

Array array_copy(Array source)
{
  assert(source);
  Array copy = array_create(source->length,source->size);
  
  if (copy->length >= source->length && source->length > 0)
  {
    memcpy(copy->arr, source->arr, source->length*source->size); 
  }
  else if (source->length > copy->length && copy->length > 0)
  {
    memcpy(copy->arr, source->arr, copy->length*source->size); 
  }

  return copy;
}
