#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

unsigned long int MAX_OBJ;

//ハッシュ Open Addressing

unsigned long int
collision_check(unsigned long int *Hash, unsigned long int key, unsigned long int idx)
{
  //衝突がなくなるまで繰り返す
  for (unsigned long int i = 0; i < MAX_OBJ; i++)
  {
    // uniqeを保証する
    if (Hash[idx] == key)
    {
      printf("Couldn't create the unique key: %lu", key);
      exit(1);
    }

    if (Hash[idx] == 0)
    {
      return idx;
    }

    if (MOD <= idx + DELTA)
    {
      idx = (idx + DELTA) - MOD;
    }
    else
    {
      idx = idx + DELTA;
    }
  }

  printf("Couldn't insert the key in hash: %lu", key);
  exit(1);
  return -1;
}

void insert_in_hash(unsigned long int *Hash, unsigned long int key)
{
  unsigned long int idx;

  idx = key % MOD;

  idx = collision_check(Hash, key, idx);

  Hash[idx] = key;
}

unsigned long int get_from_hash(unsigned long int *Hash, unsigned long int key)
{
  unsigned long int idx;

  idx = key % MOD;

  for (unsigned long int i = 0; i < MAX_OBJ; i++) // 探している数にマッチするまでまで繰り返す
  {
    if (Hash[idx] == key)
    {
      return Hash[idx];
    }

    if (MOD <= idx + DELTA)
    {
      idx = (idx + DELTA) - MOD;
    }
    else
    {
      idx = idx + DELTA;
    }
  }

  printf("Key not found: %lu", key);
  exit(1);
  return -1;
}

void gen_and_check_hash(unsigned long int *Temp, unsigned long int *Hash, unsigned long int MaxObj)
{
  MAX_OBJ = MaxObj;

  random_array(Temp, MAX_OBJ);

  // for (unsigned long int i = 0; i < MAX_OBJ; i++)
  // {
  //   insert_in_hash(Hash, Temp[i]);
  // }

  // // TODO: sortしてuniqe keyを保証する

  // for (unsigned long int i = 0; i < MAX_OBJ; i++)
  // {
  //   get_from_hash(Hash, Temp[i]);
  // }
}
