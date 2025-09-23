#include "utils.h"

void freeMem(void *p)
{
  void **ptr;
  ptr = (void **)p;
  if (ptr && *ptr)
  {
    FREE(*ptr);
    *ptr = NULL;
  }
}
