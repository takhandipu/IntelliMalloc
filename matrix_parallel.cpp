#include <pthread.h>
#include <stdio.h>
#include "matrix.h"

int  a[SIZE][SIZE], b[SIZE][SIZE], c[SIZE][SIZE];

class Index
{
public:
  int i1;int i2; int j1; int j2; int k1; int k2;
  Index(int i1,int i2, int j1, int j2, int k1, int k2):
	i1(i1),
	i2(i2),
	j1(j1),
	j2(j2),
	k1(k1),
	k2(k2)
  {
  }
};

void *run(void *index)
{
  int i = *(int *)index;
  int j,k;
#if PIN_MODE == 1
  char str[20];
  FILE *fp;
  sprintf(str, "parallel_thread_%d.txt",i);
  fp = fopen(str, "w");
  printf("%d\n",i);
#endif
  for(int l = i; l < SIZE; l+=CORE_COUNT)
  {
    for(j=0;j<SIZE;j++)
    {
      //int threadLocal = 0;
      for(k=0;k<SIZE;k++)
      {
#if PIN_MODE == 1
        fprintf(fp, "%p\n", &c[i][j]);
#endif
        c[l][j]+= a[l][k]*b[k][j];
      }
      //c[l][j]=threadLocal;
    }
  }
#if PIN_MODE == 1
  fclose(fp);
#endif
/*
Index * castedIndex = (Index *)index;
c[castedIndex->k1][castedIndex->k2] += a[castedIndex->i1][castedIndex->i2]*b[castedIndex->j1][castedIndex->j2];
*/
}

void printResult()
{
  int i,j;
  FILE *fp = fopen("parallel_result.txt", "w");
  for(i=0;i<SIZE;i++)
  {
    for(j=0;j<SIZE;j++)
    {
      fprintf(fp,"%d ",c[i][j]);
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
      c[i][j]=0;
    }
  }
  pthread_t threads[THREAD_SIZE];
  int tmp[THREAD_SIZE];
  //int num=0;
#if PIN_MODE == 1
  FILE *fp = fopen("parallel_thread_main.txt","w");
#endif
  for(i=0;i<THREAD_SIZE;i++)
  {
    //Index index(i,k,k,j,i,j);
    tmp[i]=i;
#if PIN_MODE == 1
    fprintf(fp, "%p\n",&tmp[i]);
#endif
    pthread_create(&threads[i], NULL, run, &tmp[i]);
  }
  for(i=0;i<THREAD_SIZE;i++)pthread_join(threads[i], NULL);
  if(argc>1)printResult();
#if PIN_MODE == 1
  fclose(fp);
#endif
  return 0;
}
