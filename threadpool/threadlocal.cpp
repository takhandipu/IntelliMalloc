#include <pthread.h>
#include <stdio.h>
#include "matrix.h"

#ifdef PADDING_DISTANCE
#define DISTANCE PADDING_DISTANCE
#else
#define DISTANCE 1
#endif

int  a[SIZE][SIZE], b[SIZE][SIZE];
int c[SIZE][SIZE*DISTANCE];

struct padded
{
int value;
int padding[15];
};

void *run(void *index)
{
  int i = *(int *)index;
  int j,k;
  int mod = SIZE % CORE_COUNT;
  int number = SIZE / CORE_COUNT;
  int start;
  if(i < mod)
  {
    number += 1;
    start = i * number;
  }
  else
  {
    start = (i * number)+mod;
  }
  for(int l = start; l - start < number; l++)
  { 
    for(j=0;j<SIZE;j++)
    {
      int threadLocal = 0;
      for(k=0;k<SIZE;k++)
      {
        //c[i][j].set(a[i][k]*b[k][j]+c[i][j].get());
        //c[i][j].incr(a[i][k]*b[k][j]);
        threadLocal+=a[l][k]*b[k][j];
      }
      c[l][j*DISTANCE] = threadLocal; 
    }
  }
}

void printResult()
{
  int i,j;
  FILE *fp = fopen("padded_parallel_result.txt", "w");
  for(i=0;i<SIZE;i++)
  {
    for(j=0;j<SIZE;j++)
    {
      fprintf(fp,"%d ",c[i][j*DISTANCE]);
    }
    fprintf(fp,"\n");
  }
  fclose(fp);
}

int main(int argc, char *argv[])
{
  int i,j,k;
  for(i=0;i<SIZE;i++)
  {
    for(j=0;j<SIZE;j++)
    {
      a[i][j]=i+j;
      b[i][j]=i-j;
      c[i][j*DISTANCE]=0;
    }
  }
  //printf("%lu\n%p\n%p\n",sizeof(PaddedInt),&c[0][0],&c[0][1]);
  pthread_t threads[THREAD_SIZE];
  //int num=0;
  struct padded tmp[THREAD_SIZE];
  for(i=0;i<THREAD_SIZE;i++)
  {
    //Index index(i,k,k,j,i,j);
    tmp[i].value=i;
    pthread_create(&threads[i], NULL, run, &tmp[i].value);
  }
  for(i=0;i<THREAD_SIZE;i++)pthread_join(threads[i], NULL);
  if(argc>1)printResult();
  return 0;
}
