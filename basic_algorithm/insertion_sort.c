#include <stdio.h>
#include "algorithm.h"

void insertion_sort(unsigned long int *Arr)
{
  unsigned long int key, down;

  for (unsigned long int i = 1; i < MAX_OBJ; i++)
  {
    temp = Arr[i];
    down = i - 1;

    while (temp < Arr[down] && 0 <= down)
    {
      Arr[down + 1] = Arr[down];
      down--;
    }

    Arr[down + 1] = temp;
  }
}
