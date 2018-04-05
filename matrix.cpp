//#include <pthread.h>
#define SIZE 128

int  a[SIZE][SIZE], b[SIZE][SIZE], c[SIZE][SIZE];

void run(int i1,int i2, int j1, int j2, int k1, int k2)
{
  c[k1][k2] += a[i1][i2]*b[j1][j2];
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
  for(i=0;i<SIZE;i++)
  {
    for(j=0;j<SIZE;j++)
    {
      for(k=0;k<SIZE;k++)
      {
        run(i,k,k,j,i,j);
      }
    }
  }
}
