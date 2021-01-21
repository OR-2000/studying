#include <stdio.h>
#include <stdlib.h>
#include "bptree.h"

void bptree_search(unsigned long int key)
{
  struct NODE *leaf = find_leaf(Root, key);

  for (int i = 0; i < leaf->nkey; i++)
  {
    if (leaf->key[i] == key)
    {
      return;
    }
  }

  printf("Key not found: %lu", key);
  exit(1);
}
