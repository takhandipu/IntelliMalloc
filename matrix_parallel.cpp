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
  char str[20];
  //FILE *fp;
  //sprintf(str, "p%d.txt",i);
  //fp = fopen(str, "w");
  //printf("%d\n",i);
  for(j=0;j<SIZE;j++)
  {
    for(k=0;k<SIZE;k++)
    {
      //fprintf(fp, "%p\n", &c[i][j]);
      c[i][j]+= a[i][k]*b[k][j];
    }
  }
  //fclose(fp);
/*
Index * castedIndex = (Index *)index;
c[castedIndex->k1][castedIndex->k2] += a[castedIndex->i1][castedIndex->i2]*b[castedIndex->j1][castedIndex->j2];
*/
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
      c[i][j]=0;
    }
  }
  pthread_t threads[SIZE];
  int tmp[SIZE];
  //int num=0;
  for(i=0;i<SIZE;i++)
  {
    //Index index(i,k,k,j,i,j);
    tmp[i]=i;
    pthread_create(&threads[i], NULL, run, &tmp[i]);
  }
  for(i=0;i<SIZE;i++)pthread_join(threads[i], NULL);
  return 0;
}
