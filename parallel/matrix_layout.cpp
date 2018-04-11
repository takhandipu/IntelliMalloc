#include <pthread.h>
#include <stdio.h>
#include <inttypes.h>
#include "matrix.h"

int  a[SIZE][SIZE], b[SIZE][SIZE];//, c[SIZE][SIZE];

struct wrapperC{
int c[SIZE*SIZE*64];
int changedIndex[SIZE][SIZE];
wrapperC(){
  for(int i = 0; i < 6; i++)changedIndex[0][i]=i;
  for(int i = 0; i < 6; i++)changedIndex[4][i]=i+6;
  for(int i = 0; i < 6; i++)changedIndex[1][i]=i+16;
  for(int i = 0; i < 6; i++)changedIndex[5][i]=i+22;
  for(int i = 0; i < 6; i++)changedIndex[2][i]=i+32;
  for(int i = 0; i < 6; i++)changedIndex[3][i]=i+48;
}
int& getC(int i, int j){return c[changedIndex[i][j]];}
};

struct wrapperC c;

void *run(void *index)
{
  int i = *(int *)index;
  int j,k;
#if PIN_MODE == 1
  char str[20];
  FILE *fp;
  sprintf(str, "parallel_thread_%d.csv",i+1);
  fp = fopen(str, "w");
  //fprintf(fp,"%d\n",i);
  fprintf(fp,"R,%" PRIuPTR  "\n",(uintptr_t)index);
#endif
  for(int l = i; l < SIZE; l+=CORE_COUNT)
  {
    for(j=0;j<SIZE;j++)
    {
      for(k=0;k<SIZE;k++)
      {
  #if PIN_MODE == 1
        fprintf(fp, "W,%" PRIuPTR  "\n",(uintptr_t)&c.getC(l,j));
  #endif
        c.getC(l,j)+= a[l][k]*b[k][j];
      }
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

/*void printResult()
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
}*/

int main(void)
{
  int i,j,k;
  //printf("%lu, %lu, %lu\n",sizeof(int), sizeof(int*), sizeof(void*));
  for(i=0;i<SIZE;i++)
  {
    for(j=0;j<SIZE;j++)
    {
      a[i][j]=i+j;
      b[i][j]=i-j;
      c.getC(i,j)=0;
    }
  }
  pthread_t threads[CORE_COUNT];
  int tmp[CORE_COUNT];
  //int num=0;
#if PIN_MODE == 1
  char str[20];
  FILE *fp;
  sprintf(str, "parallel_thread_%d.csv",0);
  fp = fopen(str, "w");
#endif
  for(i=0;i<CORE_COUNT;i++)
  {
    //Index index(i,k,k,j,i,j);
    tmp[i]=i;
#if PIN_MODE == 1
    fprintf(fp, "W,%" PRIuPTR  "\n",(uintptr_t)&tmp[i]);
#endif
    pthread_create(&threads[i], NULL, run, &tmp[i]);
  }
  for(i=0;i<CORE_COUNT;i++)pthread_join(threads[i], NULL);
  //printResult();
#if PIN_MODE == 1
  fclose(fp);
#endif
  return 0;
}
