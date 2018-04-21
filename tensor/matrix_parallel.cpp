#include <Eigen/Dense>
#include <iostream>

#ifdef MSIZE
#define SIZE MSIZE
#else
#define SIZE 1000
#endif

using namespace std;
Eigen::MatrixXf a(SIZE,SIZE);

Eigen::MatrixXf b(SIZE,SIZE);

Eigen::MatrixXf c(SIZE,SIZE);

void printResult()
{
  int i,j;
  FILE *fp = fopen("parallel_result.txt", "w");
  for(i=0;i<SIZE;i++)
  {
    for(j=0;j<SIZE;j++)
    {
      fprintf(fp,"%f ",c(i,j));
    }
    fprintf(fp,"\n");
  }
  fclose(fp);
}

int main(void)
{
  //cout<<&c(0,0).value<<endl;
  //cout<<&c(0,1).value<<endl;
  //cout<<&c(1,0).value<<endl;
  //Eigen::ThreadPool tpi(64);
  //Eigen::ThreadPoolDevice my_device(&tpi,64);
  omp_set_num_threads(64);
  Eigen::setNbThreads(64);
  for(int i = 0; i < SIZE; i++)
  {
    for(int j = 0; j < SIZE; j++)
    {
      a(i,j) = i+j;
      b(i,j) = i-j;
      c(i,j) = 0;
    }
  }
  c.noalias() += a*b;
  //c1 = a1.contract(b1, dimension);
  /*cout<<a<<endl;
  cout<<b<<endl;
  cout<<c<<endl;*/
  //printResult();
  return 0;
}
