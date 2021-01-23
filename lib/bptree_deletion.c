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
  int idx = -1;

  for (int i = 0; i < leaf->nkey; i++)
  {
    if (leaf->key[i] == key)
    {
      idx = i;
      leaf->key[i] = 0;
      leaf->chi[i] = NULL;
      leaf->nkey--;
      break;
    }
  }

  if (idx == -1)
  {
    printf("deletion: Key not found: %lu:", key);
    exit(1);
  }

  for (int i = idx; i < leaf->nkey; i++)
  {
    leaf->key[i] = leaf->key[i + 1];
    leaf->chi[i] = leaf->chi[i + 1];
    leaf->key[i + 1] = 0;
    leaf->chi[i + 1] = NULL;
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

void swap_the_key(struct NODE *node, int chiIdx)
{
  unsigned long int minKey = get_head_key(node->chi[chiIdx]);

  node->key[chiIdx - 1] = minKey;
}

void deal_with_head_change(struct NODE *node)
{
  if (!node->parent)
  {
    return;
  }

  struct NODE *parent = node->parent;
  int idx;

  for (int i = 0; i < parent->nkey + 1; i++)
  {
    if (parent->chi[i] == node)
    {
      idx = i;
    }
  }

  if (idx == 0)
  {
    deal_with_head_change(parent);
  }
  else
  {
    swap_the_key(parent, idx);
  }
}

void remove_from_parent(struct NODE *poor)
{
  struct NODE *parent = poor->parent;
  int poorIdx;

  for (int i = 0; i < parent->nkey + 1; i++)
  {
    if (parent->chi[i] == poor)
    {
      poorIdx = i;
    }
  }

  for (int i = poorIdx; i < parent->nkey; i++)
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

  if (poorIdx == 0)
  {
    deal_with_head_change(parent->chi[0]);
  }
}

struct NODE *find_affordable_parent(struct NODE *parent)
{
  if (parent == Root)
  {
    return Root;
  }

  if (2 < parent->nkey)
  {
    return parent;
  }
  else
  {
    return find_affordable_parent(parent->parent);
  }

  return NULL;
}

void push_leaf(struct NODE *parent, struct NODE *present)
{
  int tail = parent->nkey;

  // set child
  parent->chi[tail + 1] = present;
  parent->chi[tail + 1]->parent = parent;

  // set key
  parent->key[tail] = get_head_key(present);
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
  parent->key[0] = get_head_key(parent->chi[1]);
  parent->nkey++;

  deal_with_head_change(parent);
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
  for (int i = 0; i < parent->nkey; i++)
  {
    if (i < parent->nkey - 1)
    {
      parent->key[i] = parent->key[i + 1];
    }
    parent->chi[i] = parent->chi[i + 1];
  }
  parent->nkey--;

  if (0 < parent->nkey)
  {
    deal_with_head_change(parent);
  }
}

void pass_keys_to_null(struct NODE *rich, struct NODE *null)
{
  struct TEMP tmp;
  struct NODE *parent, *left, *right;
  int nullIdx, richIdx, num, rightIdx;

  parent = rich->parent;

  num = rich->nkey;

  for (int i = 0; i < parent->nkey + 1; i++)
  {
    if (parent->chi[i] == null)
    {
      nullIdx = i;
    }

    if (parent->chi[i] == rich)
    {
      richIdx = i;
    }
  }

  for (int i = 0; i < rich->nkey; i++)
  {
    tmp.key[i] = rich->key[i];
    tmp.chi[i] = rich->chi[i];
    rich->key[i] = 0;
    rich->chi[i] = NULL;
  }
  rich->nkey = 0;

  if (nullIdx < richIdx)
  {
    left = null;
    right = rich;
    rightIdx = richIdx;
  }
  else
  {
    left = rich;
    right = null;
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

  deal_with_head_change(left);
  deal_with_head_change(right);
}

void pass_leaf(struct NODE *present, struct NODE *to)
{
  struct NODE *parent, *from;
  int fromIdx, toIdx;

  parent = to->parent;
  from = present->parent;

  for (int i = 0; i < parent->nkey + 1; i++)
  {
    if (parent->chi[i] == from)
    {
      fromIdx = i;
    }

    if (parent->chi[i] == to)
    {
      toIdx = i;
    }
  }

  if (fromIdx < toIdx)
  {
    unshift_leaf(to, present);
    pop_leaf(from);
  }
  else
  {
    push_leaf(to, present);
    shift_leaf(from);
  }
}

struct NODE *find_rich_neighbor(struct NODE *poor, int minNkey)
{
  struct NODE *parent = poor->parent;
  int poorIdx;

  for (int i = 0; i < parent->nkey + 1; i++)
  {
    if (parent->chi[i] == poor)
    {
      poorIdx = i;
    }
  }

  if (poorIdx == 0)
  {
    if (minNkey < parent->chi[poorIdx + 1]->nkey)
    {
      return parent->chi[poorIdx + 1];
    }
  }
  else if (poorIdx == parent->nkey)
  {
    if (minNkey < parent->chi[poorIdx - 1]->nkey)
    {
      return parent->chi[poorIdx - 1];
    }
  }
  else
  {
    if (minNkey < parent->chi[poorIdx - 1]->nkey)
    {
      return parent->chi[poorIdx - 1];
    }
    else if (minNkey < parent->chi[poorIdx + 1]->nkey)
    {
      return parent->chi[poorIdx + 1];
    }
  }

  return NULL;
}

void pass_to_rich(struct NODE *poor)
{
  struct NODE *present, *rich;

  present = poor->chi[0];

  rich = find_rich_neighbor(poor, 0);

  pass_leaf(present, rich);

  remove_from_parent(poor);
}

void pass_to_poor(struct NODE *rich, struct NODE *poor)
{
  struct NODE *parent, *present;
  int poorIdx, richIdx;

  parent = poor->parent;

  for (int i = 0; i < parent->nkey + 1; i++)
  {
    if (parent->chi[i] == poor)
    {
      poorIdx = i;
    }

    if (parent->chi[i] == rich)
    {
      richIdx = i;
    }
  }

  present = poorIdx < richIdx
                ? rich->chi[0]
                : rich->chi[rich->nkey];

  pass_leaf(present, poor);
}

void pass_between_parent(struct NODE *poor)
{
  struct NODE *parent, *rich;

  if (poor->parent)
  {
    parent = poor->parent;
  }
  else
  {
    return;
  }

  rich = find_rich_neighbor(poor, 1);

  if (rich == NULL)
  {
    pass_to_rich(poor);

    if (parent->nkey == 0)
    {
      if (parent == Root)
      {
        Root = parent->chi[0];
      }
      else
      {
        pass_between_parent(parent);
      }
    }

    return;
  }

  pass_to_poor(rich, poor);
}

void deal_with_null(struct NODE *null)
{
  struct NODE *rich, *poor;
  int nullIdx;

  poor = null->parent;

  rich = find_rich_neighbor(null, 1);

  if (rich == NULL)
  {
    remove_from_parent(null);

    if (poor->nkey < 1)
    {
      pass_between_parent(poor);
    }

    return;
  }

  pass_keys_to_null(rich, null);
  return;
}

void bptree_delete(unsigned long int key)
{
  struct NODE *leaf = find_leaf(Root, key);

  int sideEffect = remove_key_from_leaf(leaf, key);

  if (sideEffect == 0)
  {
    deal_with_null(leaf);
    return;
  }

  if (sideEffect == 1)
  {
    deal_with_head_change(leaf);
    return;
  }
}
