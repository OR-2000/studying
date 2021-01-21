#include <stdio.h>
#include <stdlib.h>
#include "bptree.h"

struct NODE *alloc_parent(struct NODE *left, unsigned long int key, struct NODE *right)
{
  struct NODE *parent = (struct NODE *)calloc(1, sizeof(struct NODE));

  parent->isLeaf = 0;
  parent->key[0] = key;
  parent->chi[0] = left;
  parent->chi[1] = right;
  parent->nkey = 1;

  left->parent = parent;
  right->parent = parent;

  return parent;
}

struct NODE *alloc_node(struct NODE *parent)
{
  struct NODE *node = (struct NODE *)calloc(1, sizeof(struct NODE));
  node->isLeaf = 1;
  node->parent = parent;
  node->nkey = 0;
  return node;
}

struct DATA *alloc_data(unsigned long int key, unsigned long int value)
{
  struct DATA *data = (struct DATA *)calloc(1, sizeof(struct DATA));
  data->key = key;
  data->value = value;
  return data;
}

void copy_from_temp(struct NODE *left, struct TEMP *tmp, struct NODE *right)
{
  for (int i = 0; i < N / 2; i++)
  {
    left->key[i] = tmp->key[i];
    left->nkey++;
  }

  for (int i = N / 2; i < N; i++)
  {
    right->key[i - N / 2] = tmp->key[i];
    right->nkey++;
  }

  if (left->isLeaf)
  {
    for (int i = 0; i < N / 2; i++)
    {
      left->chi[i] = tmp->chi[i];
    }

    for (int i = N / 2; i < N; i++)
    {
      right->chi[i - N / 2] = tmp->chi[i];
    }
  }
  else
  {
    for (int i = 0; i < (N / 2 + 1); i++)
    {
      left->chi[i] = tmp->chi[i];
      left->chi[i]->parent = left;
    }

    for (int i = (N / 2 + 1); i < N + 1; i++)
    {
      right->chi[i - (N / 2 + 1)] = tmp->chi[i];
      right->chi[i - (N / 2 + 1)]->parent = right;
    }
  }
}

void insert_in_temp(struct TEMP *tmp, unsigned long int key, struct NODE *child)
{
  if (tmp->isLeaf)
  {
    int flag = 1;

    for (int i = tmp->nkey; 0 < i; i--)
    {
      if (tmp->key[i - 1] < key)
      {
        tmp->key[i] = key;
        tmp->chi[i] = child;
        flag = 0;
        break;
      }
      tmp->key[i] = tmp->key[i - 1];
      tmp->chi[i] = tmp->chi[i - 1];
    }

    if (flag)
    {
      tmp->key[0] = key;
      tmp->chi[0] = child;
    }
  }
  else
  {
    int flag = 1;

    for (int i = tmp->nkey; 0 < i; i--)
    {
      if (tmp->key[i - 1] < key)
      {
        tmp->key[i] = key;
        tmp->chi[i + 1] = child;
        flag = 0;
        break;
      }
      tmp->key[i] = tmp->key[i - 1];
      tmp->chi[i + 1] = tmp->chi[i];
    }

    if (flag)
    {
      tmp->key[0] = key;
      tmp->chi[1] = child;
    }
  }
}

void move_to_temp(struct NODE *left, struct TEMP *tmp)
{
  for (int i = 0; i < N - 1; i++)
  {
    tmp->key[i] = left->key[i];
    tmp->chi[i] = left->chi[i];
    tmp->nkey++;

    left->key[i] = 0;
    left->chi[i] = NULL;
  }
  left->nkey = 0;

  if (!left->isLeaf)
  {
    tmp->chi[N - 1] = left->chi[N - 1];
  }
  left->chi[N - 1] = NULL;

  tmp->isLeaf = left->isLeaf;
}

void inner_split(struct NODE *left, unsigned long int key, struct NODE *child, struct NODE *right)
{
  struct TEMP tmp;
  struct NODE *next;

  if (left->isLeaf && left->chi[N - 1])
  {
    next = left->chi[N - 1];
  }

  move_to_temp(left, &tmp);
  insert_in_temp(&tmp, key, child);
  copy_from_temp(left, &tmp, right);

  if (left->isLeaf)
  {
    left->chi[N - 1] = right;
    right->chi[N - 1] = next;
  }
}

void insert_in_parent(struct NODE *parent, unsigned long int key, struct NODE *child)
{
  if (parent->nkey < N - 1)
  {
    int flag = 1;

    for (int i = parent->nkey; 0 < i; i--)
    {
      if (parent->key[i - 1] < key)
      {
        parent->key[i] = key;
        parent->chi[i + 1] = child;
        flag = 0;
        break;
      }
      parent->key[i] = parent->key[i - 1];
      parent->chi[i + 1] = parent->chi[i];
    }

    if (flag)
    {
      parent->key[0] = key;
      parent->chi[1] = child;
    }

    parent->nkey++;
    child->parent = parent;
  }
  else
  {
    struct NODE *right = alloc_node(NULL);
    struct NODE *left = parent;
    unsigned long int minKey;

    right->isLeaf = 0;

    inner_split(left, key, child, right);

    minKey = right->key[0];
    for (int i = 0; i < right->nkey - 1; i++)
    {
      right->key[i] = right->key[i + 1];
    }
    right->key[right->nkey - 1] = 0;
    right->nkey--;

    if (left->parent)
    {
      insert_in_parent(left->parent, minKey, right);
    }
    else
    {
      struct NODE *parent = alloc_parent(left, minKey, right);
      Root = parent;
    }
  }
}

void insert_in_leaf(struct NODE *leaf, unsigned long int key)
{
  if (leaf->nkey < N - 1)
  {
    int flag = 1;

    for (int i = leaf->nkey; 0 < i; i--)
    {
      if (leaf->key[i - 1] < key)
      {
        leaf->key[i] = key;
        leaf->chi[i] = (struct NODE *)NULL;
        flag = 0;
        break;
      }
      leaf->key[i] = leaf->key[i - 1];
      leaf->chi[i] = leaf->chi[i - 1];
    }

    if (flag)
    {
      leaf->key[0] = key;
      leaf->chi[0] = (struct NODE *)NULL;
    }

    leaf->nkey++;
  }
  else
  {
    struct NODE *right = alloc_node(NULL);
    struct NODE *left = leaf;
    unsigned long int minKey;

    inner_split(left, key, (struct NODE *)NULL, right);

    minKey = get_min_key(right);

    if (left->parent)
    {
      insert_in_parent(left->parent, minKey, right);
    }
    else
    {
      struct NODE *parent = alloc_parent(left, minKey, right);
      Root = parent;
    }
  }
}

struct NODE *set_node(unsigned long int key, struct NODE *node)
{
  if (Root == NULL)
  {
    return alloc_node(NULL);
  }
  else
  {
    return find_leaf(Root, key);
  }
}

void bptree_insert(unsigned long int key)
{
  struct NODE *leaf = set_node(key, leaf);

  if (Root == NULL)
  {
    Root = leaf;
  }

  insert_in_leaf(leaf, key);
}
