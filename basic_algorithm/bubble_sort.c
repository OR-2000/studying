#include <stdio.h>
#include "algorithm.h"

void bubble_sort(unsigned long int *Arr)
{
  unsigned long int i, j, v, t;

  for (j = 0; j < MAX_OBJ; j++)
  {
    for (i = 0; i < MAX_OBJ; i++)
    {
      if (Arr[i + 1] < Arr[i])
      {
        t = Arr[i + 1];
        Arr[i + 1] = Arr[i];
        Arr[i] = t;
      }
    }
  }
}
