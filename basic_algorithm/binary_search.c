#include <stdio.h>
#include <stdlib.h>
#include "algorithm.h"

void binary_search(unsigned long int *Arr, unsigned long int key, unsigned long int length)
{
  unsigned long int head, tail, mid;

  head = 0;
  tail = length - 1;

  while (head <= tail)
  {
    mid = (head + tail) / 2;

    if (key == Arr[mid])
    {
      return;
    }
    if (key < Arr[mid])
    {
      tail = mid - 1;
    }
    else
    {
      head = mid + 1;
    }
  }

  printf("Key not found: %lu\n", key);
  exit(1);
}
