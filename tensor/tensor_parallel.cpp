#include <unsupported/Eigen/CXX11/Tensor>
//#include <unsupported/Eigen/CXX11/ThreadPool>
#include <iostream>
#include <future>
#include <Eigen/Core>

#define SIZE 1000

using namespace std;
Eigen::Tensor<int, 2> a(SIZE,SIZE);

Eigen::Tensor<int, 2> b(SIZE,SIZE);

Eigen::Tensor<int, 2> c(SIZE,SIZE);

Eigen::array<Eigen::IndexPair<int>, 1> dimension = {Eigen::IndexPair<int>(1,0)};

int main(void)
{
  //cout<<&c(0,0).value<<endl;
  //cout<<&c(0,1).value<<endl;
  //cout<<&c(1,0).value<<endl;
  Eigen::ThreadPool tpi(64);
  Eigen::ThreadPoolDevice my_device(&tpi,64);
  for(int i = 0; i < SIZE; i++)
  {
    for(int j = 0; j < SIZE; j++)
    {
      a(i,j) = i+j;
      b(i,j) = i-j;
    }
  }
  c.device(my_device) = a.contract(b, dimension);
  //c1 = a1.contract(b1, dimension);
  /*cout<<a<<endl;
  cout<<b<<endl;
  cout<<c<<endl;*/
 
  return 0;
}
