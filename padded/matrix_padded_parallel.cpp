#include <pthread.h>
#include <stdio.h>
#include <inttypes.h>
#include "matrix.h"

class PaddedInt
{
public:
  int value;
  int padding[15];
  void set(int val){value=val;}
  int get(){return value;}
  void incr(int val){value+=val;}
};

struct padded
{
int value;
int padding[15];
};

int  a[SIZE][SIZE], b[SIZE][SIZE];
/*PaddedInt*/struct padded c[SIZE][SIZE];

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
  sprintf(str, "padded_thread_%d.txt",i);
  fp = fopen(str, "w");
  fprintf(fp,"%d\n",i);
  fprintf(fp, "%" PRIuPTR  "\n",(uintptr_t)index);
#endif
  int val = 0;
  for(j=0;j<SIZE;j++)
  {
    for(k=0;k<SIZE;k++)
    {
      //c[i][j].set(a[i][k]*b[k][j]+c[i][j].get());
      //c[i][j].incr(a[i][k]*b[k][j]);
      val+=a[i][k]*b[k][j];
    }
  }
  c[i][j].value=val;
#if PIN_MODE == 1
  fprintf(fp, "%" PRIuPTR  "\n",(uintptr_t)&c[i][j].value);
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
  FILE *fp = fopen("padded_parallel_result.txt", "w");
  for(i=0;i<SIZE;i++)
  {
    for(j=0;j<SIZE;j++)
    {
      fprintf(fp,"%d ",c[i][j].value);
    }
    fprintf(fp,"\n");
  }
  fclose(fp);
}

int main(void)
{
  int i,j,k;
  for(i=0;i<SIZE;i++)
  {
    for(j=0;j<SIZE;j++)
    {
      a[i][j]=i+j;
      b[i][j]=i-j;
      //c[i][j].value=0;
    }
  }
  //printf("%lu\n%p\n%p\n",sizeof(PaddedInt),&c[0][0],&c[0][1]);
  pthread_t threads[SIZE];
  //int num=0;
  struct padded tmp[SIZE];
  //int num=0;
#if PIN_MODE == 1
  FILE *fp = fopen("padded_thread_main.txt","w");
#endif
  for(i=0;i<SIZE;i++)
  {
    //Index index(i,k,k,j,i,j);
    tmp[i].value=i;
#if PIN_MODE == 1
    fprintf(fp, "%" PRIuPTR  "\n",(uintptr_t)&tmp[i].value);
#endif
    pthread_create(&threads[i], NULL, run, &tmp[i].value);
  }
  for(i=0;i<SIZE;i++)pthread_join(threads[i], NULL);
  //printResult();
#if PIN_MODE == 1
  fclose(fp);
#endif
  return 0;
}
