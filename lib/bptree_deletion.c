#include <stdio.h>
#include <stdlib.h>
#include "bptree.h"

unsigned long int get_head_key(struct NODE *node)
{
  if (node->isLeaf)
  {
    return node->key[0];
  }

  return get_head_key(node->chi[0]);
}

int remove_key_from_leaf(struct NODE *leaf, unsigned long int key)
{
  int match = 0;
  int idx;

  for (int i = 0; i < leaf->nkey - 1; i++)
  {
    if (leaf->key[i] == key)
    {
      idx = i;
      match = 1;
    }

    if (match)
    {
      leaf->key[i] = leaf->key[i + 1];
      leaf->chi[i] = leaf->chi[i + 1];
      leaf->key[i + 1] = 0;
      leaf->chi[i + 1] = NULL;
    }
  }
  leaf->nkey--;

  if (leaf->nkey == 0 && Root == leaf)
  {
    printf("Root come to be null: []");
    exit(1);
  }

  if (leaf->nkey == 0)
  {
    // the leaf came to be null
    return 0;
  }
  else if (idx == 0)
  {
    // head of the leaf has changed
    return 1;
  }

  // no side effect
  return -1;
}

int find_affordable_neighbor_node(struct NODE *parent, int poor, int mixLine)
{
  struct NODE *left;
  struct NODE *right;

  if (poor == 0)
  {
    right = parent->chi[poor + 1];

    if (mixLine < right->nkey)
    {
      return poor + 1;
    }
  }
  else if (poor == parent->nkey)
  {
    left = parent->chi[poor - 1];

    if (mixLine < left->nkey)
    {
      return poor - 1;
    }
  }
  else
  {
    left = parent->chi[poor - 1];
    right = parent->chi[poor + 1];

    if (mixLine < left->nkey)
    {
      return poor - 1;
    }
    else if (mixLine < right->nkey)
    {
      return poor + 1;
    }
  }

  return -1;
}

void swap_the_key(struct NODE *node, int chiIdx)
{
  unsigned long int minKey = get_head_key(node->chi[chiIdx]);

  node->key[chiIdx - 1] = minKey;
}

void deal_with_the_head_change(struct NODE *child)
{
  if (!child->parent)
  {
    return;
  }

  struct NODE *parent = child->parent;
  int nkey = parent->nkey;
  int chiIdx;

  for (int i = 0; i < nkey + 1; i++)
  {
    if (parent->chi[i] == child)
    {
      chiIdx = i;
    }
  }

  if (chiIdx == 0)
  {
    deal_with_the_head_change(parent);
  }
  else
  {
    swap_the_key(parent, chiIdx);
  }
}

void remove_leaf_from_parent(struct NODE *parent, int nullLeaf)
{
  for (int i = nullLeaf; i < parent->nkey; i++)
  {
    if (0 < i)
    {
      parent->key[i - 1] = parent->key[i];
    }

    parent->chi[i] = parent->chi[i + 1];

    parent->key[i] = 0;
    parent->chi[i + 1] = NULL;
  }
  parent->nkey--;

  if (nullLeaf == 0)
  {
    deal_with_the_head_change(parent->chi[nullLeaf]);
  }
}

void push_leaf(struct NODE *parent, struct NODE *present)
{
  int tail = parent->nkey;
  // set child
  parent->chi[tail + 1] = present;
  parent->chi[tail + 1]->parent = parent;

  // set key
  parent->key[tail] = present->key[0];
  parent->nkey++;
}

void unshift_leaf(struct NODE *parent, struct NODE *present)
{
  for (int i = parent->nkey + 1; 0 < i; i--)
  {
    if (i != parent->nkey + 1)
    {
      parent->key[i] = parent->key[i - 1];
    }
    parent->chi[i] = parent->chi[i - 1];
  }

  // set child
  parent->chi[0] = present;
  parent->chi[0]->parent = parent;

  // set key
  parent->key[0] = get_min_key(parent->chi[1]);
  parent->nkey++;

  deal_with_the_head_change(parent->chi[0]);
}

void pop_leaf(struct NODE *parent)
{
  int nkey = parent->nkey;

  // remove child
  parent->chi[nkey] = NULL;

  // remove key
  parent->key[nkey - 1] = 0;
  parent->nkey--;
}

void shift_leaf(struct NODE *parent)
{
  for (int i = 0; i < parent->nkey + 1; i++)
  {
    if (i < parent->nkey)
    {
      parent->key[i] = parent->key[i + 1];
    }
    parent->chi[i] = parent->chi[i + 1];
  }
  parent->nkey--;

  deal_with_the_head_change(parent->chi[0]);
}

void pass_keys(struct NODE *parent, int nullIdx, int neighborIdx)
{
  struct TEMP tmp;

  struct NODE *pass = parent->chi[neighborIdx];

  struct NODE *left;
  struct NODE *right;
  int num = pass->nkey;

  for (int i = 0; i < pass->nkey; i++)
  {
    tmp.key[i] = pass->key[i];
    tmp.chi[i] = pass->chi[i];
    pass->key[i] = 0;
    pass->chi[i] = NULL;
  }
  pass->nkey = 0;

  int rightIdx;

  if (nullIdx < neighborIdx)
  {
    left = parent->chi[nullIdx];
    right = parent->chi[neighborIdx];
    rightIdx = neighborIdx;
  }
  else
  {
    left = parent->chi[neighborIdx];
    right = parent->chi[nullIdx];
    rightIdx = nullIdx;
  }

  for (int i = 0; i < num / 2; i++)
  {
    left->key[i] = tmp.key[i];
    left->chi[i] = tmp.chi[i];
    left->nkey++;
  }

  for (int i = num / 2; i < num; i++)
  {
    right->key[i - num / 2] = tmp.key[i];
    right->chi[i - num / 2] = tmp.chi[i];
    right->nkey++;
  }

  parent->key[rightIdx - 1] = right->key[0];

  if (nullIdx == 0)
  {
    deal_with_the_head_change(parent->chi[0]);
  }
}

void pass_leaf(struct NODE *grandpa, int from, int to, struct NODE *present)
{
  if (from < to)
  {
    unshift_leaf(grandpa->chi[to], present);
    pop_leaf(grandpa->chi[from]);
  }
  else
  {
    // 右から左にパス
    push_leaf(grandpa->chi[to], present);
    shift_leaf(grandpa->chi[from]);
  }
}

void pass_leaf_to_rich_node(struct NODE *parent, int poorLeaf, int poorNode)
{
  struct NODE *grandpa = parent->parent;
  struct NODE *present = poorLeaf == 0 ? parent->chi[1] : parent->chi[0];

  int adoptableNode = find_affordable_neighbor_node(grandpa, poorNode, 0);

  pass_leaf(grandpa, poorNode, adoptableNode, present);
  remove_leaf_from_parent(grandpa, poorNode);
}

void fetch_leaf_from_rich_node(struct NODE *parent, int nullLeaf, int poorIdx, int richIdx)
{
  struct NODE *grandpa = parent->parent;

  struct NODE *richNode = grandpa->chi[richIdx];

  struct NODE *present = poorIdx < richIdx
                             ? richNode->chi[0]
                             : richNode->chi[richNode->nkey];

  remove_leaf_from_parent(parent, nullLeaf);

  pass_leaf(grandpa, richIdx, poorIdx, present);
}

void passing_between_nodes(struct NODE *parent, int poorLeaf)
{
  struct NODE *grandpa = parent->parent;

  int poorNode;

  for (int i = 0; i < grandpa->nkey + 1; i++)
  {
    if (grandpa->chi[i] == parent)
    {
      poorNode = i;
    }
  }

  int richNode = find_affordable_neighbor_node(grandpa, poorNode, 1);

  if (richNode == -1)
  {
    pass_leaf_to_rich_node(parent, poorLeaf, poorNode);
    return;
  }

  fetch_leaf_from_rich_node(parent, poorLeaf, poorNode, richNode);
}

void deal_with_the_null_leaf(struct NODE *nullLeaf)
{
  int nullIdx, neighborIdx;

  struct NODE *parent = nullLeaf->parent;

  for (int i = 0; i < parent->nkey + 1; i++)
  {
    if (parent->chi[i] == nullLeaf)
    {
      nullIdx = i;
    }
  }

  neighborIdx = find_affordable_neighbor_node(parent, nullIdx, 1);

  if (neighborIdx == -1)
  {
    if (2 <= parent->nkey)
    {
      remove_leaf_from_parent(parent, nullIdx);

      struct NODE *granpa = parent->parent ? parent->parent : Root;

      if (granpa->nkey == 0)
      {
        Root = parent->chi[0] ? parent->chi[0] : NULL;
        Root->parent = NULL;
      }

      return;
    }

    passing_between_nodes(parent, nullIdx);
    return;
  }

  pass_keys(parent, nullIdx, neighborIdx);
  return;
}

void bptree_delete(unsigned long int key)
{
  struct NODE *leaf = find_leaf(Root, key);

  int sideEffect = remove_key_from_leaf(leaf, key);

  if (sideEffect == -1)
  {
    return;
  }

  if (sideEffect == 1)
  {
    deal_with_the_head_change(leaf);
    return;
  }

  deal_with_the_null_leaf(leaf);
}
