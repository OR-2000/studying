#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define N 5
// #define MAX_OBJ (10000ul * 10000ul)
#define MAX_OBJ 30ul
#define DELTA 3
#define MOD (MAX_OBJ * 10ul)
#define STR_SIZE 10

void die()
{
  exit(1);
};

int interactive()
{
  unsigned long int key;

  printf("key: ");
  scanf("%lu", &key);

  return key;
}

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

struct NODE *root = NULL;

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

unsigned long int get_min_key(struct NODE *node)
{
  if (node->isLeaf)
  {
    return node->key[0];
  }

  return get_min_key(node->chi[1]);
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
      root = parent;
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
      root = parent;
    }
  }
}

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

struct NODE *set_node(unsigned long int key, struct NODE *node)
{
  if (root == NULL)
  {
    return alloc_node(NULL);
  }
  else
  {
    return find_leaf(root, key);
  }
}

void start_insert(unsigned long int key)
{
  struct NODE *leaf = set_node(key, leaf);
  if (root == NULL)
  {
    root = leaf;
  }
  insert_in_leaf(leaf, key);
}

void search_key(struct NODE *root, unsigned long int key)
{
  struct NODE *leaf = find_leaf(root, key);

  for (int i = 0; i < leaf->nkey; i++)
  {
    if (leaf->key[i] == key)
    {
      return;
    }
  }

  printf("Key not found: %lu", key);
  die();
}

void range_search(struct NODE *root, unsigned long int min, unsigned long int max)
{
  struct NODE *leaf = find_leaf(root, min);
  int index = 0;
  unsigned long int curKey = -1;

  for (int i = 0; i < leaf->nkey; i++)
  {
    if (min <= leaf->key[i] && leaf->key[i] <= max)
    {
      curKey = leaf->key[i];
      printf("%lu ", curKey);
    }
  }

  leaf = leaf->chi[N - 1];

  while (min <= curKey && curKey <= max)
  {
    if (index % (N - 1) == 0 && index != 0)
    {
      leaf = leaf->chi[N - 1];
      index = 0;
    }

    curKey = leaf->key[index];

    if (curKey)
    {
      printf("%lu ", curKey);
    }

    index++;
  }
}

int get_key_index(struct NODE *node, unsigned long int key)
{
  for (int i = 0; i < node->nkey; i++)
  {
    if (node->key[i] == key)
    {
      return i;
    }
  }

  return -1;
}

unsigned long int get_head_key(struct NODE *node)
{
  if (node->isLeaf)
  {
    return node->key[0];
  }

  return get_head_key(node->chi[0]);
}

void change_parent_key(struct NODE *child, unsigned long int key)
{
  struct NODE *parent = child->parent;

  if (parent->parent)
  {
    change_parent_key(parent, get_head_key(parent));
  }

  for (int i = 1; i < parent->nkey + 1; i++)
  {
    if (child == parent->chi[i])
    {
      parent->key[i - 1] = key;
      break;
    }
  }
}

void remove_from_parent(struct NODE *child, unsigned long int key)
{
  struct NODE *parent = child->parent;
  int flag = 0;
  int isHead = 0;

  for (int i = 0; i < parent->nkey; i++)
  {
    if (parent->chi[i] == child)
    {
      if (i == 0)
      {
        isHead = 1;
      }
      flag = 1;
    }

    if (flag)
    {
      if (i != 0)
      {
        parent->key[i - 1] = parent->key[i];
        parent->key[i] = 0;
      }

      parent->chi[i] = parent->chi[i + 1];
      parent->chi[i + 1] = NULL;
    }
  }

  if (!flag)
  {
    parent->key[parent->nkey - 1] = 0;
    parent->chi[parent->nkey] = NULL;
  }
  parent->nkey--;

  if (parent->parent)
  {
    if (parent->nkey == 0)
    {
      remove_from_parent(parent, key);
    }

    if (0 < parent->nkey && isHead)
    {
      change_parent_key(parent, get_head_key(parent));
    }
  }
}

unsigned long int collision_check(unsigned long int *Hash, unsigned long int key, unsigned long int idx)
{
  //衝突がなくなるまで繰り返す
  for (unsigned long int i = 0; i < MAX_OBJ; i++)
  {
    // uniqeを保証する
    if (Hash[idx] == key)
    {
      printf("Couldn't create unique key");
      die();
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

  printf("Can't insert key in hash");
  die();
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
  die();
  return -1;
}

unsigned char *keyword_generator()
{
  unsigned char *keyword = (unsigned char *)calloc(STR_SIZE, sizeof(unsigned char *));
  int idx;
  int length = 62;

  unsigned char str[63] = {
      '0',
      '1',
      '2',
      '3',
      '4',
      '5',
      '6',
      '7',
      '8',
      '9',
      'A',
      'B',
      'C',
      'D',
      'E',
      'F',
      'G',
      'H',
      'I',
      'J',
      'K',
      'L',
      'M',
      'N',
      'O',
      'P',
      'Q',
      'R',
      'S',
      'T',
      'U',
      'V',
      'W',
      'X',
      'Y',
      'Z',
      'a',
      'b',
      'c',
      'd',
      'e',
      'f',
      'g',
      'h',
      'i',
      'j',
      'k',
      'l',
      'm',
      'n',
      'o',
      'p',
      'q',
      'r',
      's',
      't',
      'u',
      'v',
      'w',
      'x',
      'y',
      'z',
  };

  for (int i = 0; i < STR_SIZE; i++)
  {
    idx = rand() % length;
    keyword[i] = str[idx];
    // printf("%d ", keyword[i]);
  }

  // printf("%lu ", "2vH");

  return keyword;
}

unsigned long int *gen_and_check_hash(unsigned long int *t)
{
  unsigned long int *Hash = (unsigned long int *)calloc(MOD, sizeof(unsigned long int *));
  unsigned char *keyword;
  srand((unsigned int)time(NULL));

  for (unsigned long int i = 0; i < MAX_OBJ; i++)
  {
    keyword = keyword_generator();
    t[i] = (unsigned long int)keyword;
    insert_in_hash(Hash, (unsigned long int)keyword);
  }

  for (unsigned long int i = 0; i < MAX_OBJ; i++)
  {
    get_from_hash(Hash, t[i]);
  }

  return Hash;
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

int find_affordable_neighbor(struct NODE *parent, int poor, int mixLine)
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

void fetch_keys_from_affordable_neighbor(struct NODE *parent, int poor, int rich)
{
  struct TEMP tmp;

  struct NODE *pass = parent->chi[rich];

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

  if (poor < rich)
  {
    left = parent->chi[poor];
    right = parent->chi[rich];
    rightIdx = rich;
  }
  else
  {
    left = parent->chi[rich];
    right = parent->chi[poor];
    rightIdx = poor;
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
}

void remove_leaf_from_parent(struct NODE *parent, int nullIdx)
{
  for (int i = nullIdx; i < parent->nkey; i++)
  {
    if (0 < i)
    {
      parent->key[i - 1] = parent->key[i];
      parent->key[i] = 0;
    }
    parent->chi[i] = parent->chi[i + 1];
    parent->chi[i + 1] = NULL;
  }
  parent->nkey--;

  struct NODE *granpa = parent->parent ? parent->parent : root;

  if (granpa->nkey == 0)
  {
    root = parent->chi[0] ? parent->chi[0] : NULL;
    root->parent = NULL;
  }
}

struct NODE *find_affordable_parent(struct NODE *parent)
{
  if (parent == root)
  {
    return root;
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

void swap_the_key(struct NODE *node, int chiIdx)
{
  unsigned long int minKey = get_head_key(node->chi[chiIdx]);

  node->key[chiIdx - 1] = minKey;
}

void reflect_the_head_change_on_parent(struct NODE *child)
{
  if (!child->parent)
  {
    return;
  }

  struct NODE *parent = child->parent;
  int nkey = parent->nkey;
  int isHead = 0;
  int chiIdx = 0;

  for (int i = 0; i < nkey + 1; i++)
  {
    if (parent->chi[i] == child)
    {
      chiIdx = i;
    }
  }

  if (chiIdx == 0)
  {
    reflect_the_head_change_on_parent(parent);
  }
  else
  {
    swap_the_key(parent, chiIdx);
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

  reflect_the_head_change_on_parent(parent);
}

void pop_leaf(struct NODE *parent)
{
  int nkey = parent->nkey;

  // remove child
  parent->chi[nkey + 1] = NULL;

  // remove key
  parent->key[nkey] = 0;
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

  reflect_the_head_change_on_parent(parent);
}

void pass_leaf(struct NODE *grandpa, int from, int to, struct NODE *present)
{
  if (to < from)
  {
    push_leaf(grandpa->chi[to], present);
    shift_leaf(grandpa->chi[from]);
  }
  else
  {
    unshift_leaf(grandpa->chi[to], present);
    pop_leaf(grandpa->chi[from]);
  }
}

struct NODE *bereaved_leaf(struct NODE *parent)
{
  printf("%lu\n", parent->key[0]);

  if (parent->chi[0] == NULL)
  {
    return parent->chi[1];
  }

  return parent->chi[0];
}

void start_deletion(unsigned long int key)
{
  struct NODE *leaf = find_leaf(root, key);
  struct NODE *parent = leaf->parent ? leaf->parent : root;

  /*
    remove_key_from_leaf
    0: the leaf came to be null
    1: head of the leaf has changed
    -1: no side effect
  */
  int sideEffect = remove_key_from_leaf(leaf, key);

  if (sideEffect == 0) // the leaf came to be null
  {
    int poorLeaf, richLeaf;

    for (int i = 0; i < parent->nkey + 1; i++)
    {
      if (parent->chi[i] == leaf)
      {
        poorLeaf = i;
      }
    }

    richLeaf = find_affordable_neighbor(parent, poorLeaf, 1);

    if (richLeaf == -1)
    {
      if (parent->nkey < 2)
      {
        if (parent == root)
        {
          remove_leaf_from_parent(parent, poorLeaf);
        }
        else
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

          int richNode = find_affordable_neighbor(grandpa, poorNode, 1);
          struct NODE *present = poorLeaf == 0 ? parent->chi[1] : parent->chi[0];

          if (richNode == -1)
          {
            int adoptableNode = find_affordable_neighbor(grandpa, poorNode, 0);

            pass_leaf(grandpa, poorNode, adoptableNode, present);
            remove_leaf_from_parent(grandpa, poorNode);
          }
          else
          {
            remove_leaf_from_parent(parent, poorLeaf);

            if (poorNode < richNode)
            {
              present = grandpa->chi[richNode]->chi[0];
            }
            else
            {
              int nkey = grandpa->chi[richNode]->nkey;
              present = grandpa->chi[richNode]->chi[nkey];
            }
            pass_leaf(grandpa, richNode, poorNode, present);
          }
        }
      }
      else
      {
        remove_leaf_from_parent(parent, poorLeaf);
      }
    }
    else
    {
      fetch_keys_from_affordable_neighbor(parent, poorLeaf, richLeaf);
    }
  }
  else if (sideEffect == 1) // head of the leaf has changed
  {
    reflect_the_head_change_on_parent(leaf);
  }

  // // no side effect
}

int main(int argc, char *argv[])
{
  root = NULL;

  // ---------------- for hash ----------------
  unsigned long int *t = (unsigned long int *)calloc(MAX_OBJ, sizeof(unsigned long int *));
  unsigned long int *Hash = gen_and_check_hash(t);

  for (unsigned long int i = 0; i < MAX_OBJ; i++)
  {
    printf("%lu\n", t[i]);
  }

  // ---------------- for bptree ----------------
  // for (unsigned long int i = 0; i < MAX_OBJ; i++)
  // {
  //   start_insert(t[i]);
  // }

  // for (unsigned long int i = 0; i < MAX_OBJ; i++)
  // {
  //   search_key(root, t[i]);
  // }

  // ---------------- for deletion ----------------

  // unsigned long int t[30] =
  //     {
  //         1211,
  //         122,
  //         783,
  //         293,
  //         111,
  //         784,
  //         768,
  //         182,
  //         32,
  //         82, // 10
  //         471,
  //         23,
  //         112,
  //         43,
  //         8675,
  //         172,
  //         144,
  //         62,
  //         3768,
  //         971, // 20
  //         37,
  //         257,
  //         321,
  //         1434,
  //         124,
  //         1112,
  //         1232,
  //         112,
  //         10022,
  //         23};

  // for (unsigned long int i = 0; i < MAX_OBJ; i++)
  // {
  //   start_insert(t[i]);
  // }

  // for (int i = 0; i < MAX_OBJ; i++)
  // {
  //   printf("\n%d---key: %lu ---\n", i, t[i]);
  //   print_tree(root);
  //   start_deletion(t[i]);
  // }

  // printf("\n");
  // print_tree(root);
  // unsigned char a[2] = {117, 117};

  exit(0);
}
