#include <stdio.h>
#include <stdlib.h>
#include "lib/tools.h"

int main(int argc, char *argv[])
{
  Root = NULL;

  unsigned long int *Temp =
      (unsigned long int *)calloc(MAX_OBJ, sizeof(unsigned long int *));

  unsigned long int *Hash =
      (unsigned long int *)calloc(MOD, sizeof(unsigned long int *));

  gen_and_check_hash(Temp, Hash, MAX_OBJ);

  for (unsigned long int i = 0; i < MAX_OBJ; i++)
  {
    bptree_insert(Temp[i]);
  }

  for (unsigned long int i = 0; i < MAX_OBJ; i++)
  {
    bptree_search(Temp[i]);
  }

  for (unsigned long int i = 0; i < MAX_OBJ; i++)
  {
    bptree_delete(Temp[i]);
  }

  exit(0);
}
