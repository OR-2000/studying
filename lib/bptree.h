#ifndef BPTREE_H
#define BPTREE_H

#define N 3
#define MAX_OBJ (1000ul * 10000ul)

struct DATA
{
  unsigned long int key;
  char value;
  struct DATA *next;
};

struct NODE
{
  struct NODE *parent;
  struct NODE *chi[N];
  unsigned long int key[N - 1];
  int nkey;
  int isLeaf;
};

struct TEMP
{
  struct NODE *chi[N + 1];
  unsigned long int key[N];
  int nkey;
  int isLeaf;
};

struct NODE *Root;

struct NODE *find_leaf(struct NODE *node, unsigned long int key)
{
  if (node->isLeaf)
  {
    return node;
  }

  int chiIdx = node->nkey;

  for (int i = 0; i < node->nkey; i++)
  {
    if (key < node->key[i])
    {
      chiIdx = i;
      break;
    }
  }

  return find_leaf(node->chi[chiIdx], key);
}

unsigned long int get_min_key(struct NODE *node)
{
  if (node->isLeaf)
  {
    return node->key[0];
  }

  return get_min_key(node->chi[1]);
}

#endif
