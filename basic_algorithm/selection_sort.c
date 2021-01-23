#include <stdio.h>
#include "algorithm.h"

void selection_sort(unsigned long int *Arr)
{
  unsigned long int temp, idx;

  idx = 0;

  for (unsigned long int j = 0; j < MAX_OBJ; j++)
  {
    temp = Arr[j];

    for (unsigned long int i = j; i < MAX_OBJ; i++)
    {
      if (Arr[i] <= temp)
      {
        temp = Arr[i];
        idx = i;
      }
    }

    Arr[idx] = Arr[j];
    Arr[j] = temp;
  }
}
