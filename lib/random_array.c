#include <time.h>
#include <sys/time.h>

void random_array(unsigned long int *Array, unsigned long int MAX_OBJ)
{
  unsigned int t[2];

  struct timeval tv;
  gettimeofday(&tv, NULL);
  srand((unsigned)tv.tv_sec);

  for (int i = 0; i < MAX_OBJ; i++)
  {
    Array[i] = (unsigned long int)rand();
  }
}
