#include <unsupported/Eigen/CXX11/Tensor>
#include <iostream>
#include <future>
#include <Eigen/Core>

#define SIZE 1000

using namespace std;
namespace Eigen{
  class IntWrapper
  {
  public:
    int value;
    char padding[60]="abc";
    IntWrapper(int val=0)
    {
      value = val;
    }
    IntWrapper operator+(const IntWrapper & ref)
    {
      return IntWrapper(value+ref.value);
    }
    IntWrapper operator-(const IntWrapper & ref)
    {
      return IntWrapper(value-ref.value);
    }
    IntWrapper& operator+=(const IntWrapper & ref)
    {
      value += ref.value;
      return *this;
    }
    IntWrapper operator*(const IntWrapper & ref)
    {
      return IntWrapper(value*ref.value);
    }
    IntWrapper& operator=(const IntWrapper & ref)
    {
      value = ref.value;
      return *this;
    }
    IntWrapper& operator=(const int i)
    {
      value = i;
      return *this;
    }
    friend IntWrapper operator+(const IntWrapper &, const IntWrapper &);
    friend IntWrapper operator*(const IntWrapper &, const IntWrapper &);
  };
  IntWrapper operator*(const IntWrapper & lhs, const IntWrapper &rhs)
  {
    return IntWrapper(lhs.value*rhs.value);
  }
  IntWrapper operator+(const IntWrapper & lhs, const IntWrapper &rhs)
  {
    return IntWrapper(lhs.value+rhs.value);
  }
  template<> struct NumTraits<IntWrapper> : GenericNumTraits<IntWrapper>
  {
    enum {
        IsComplex = 0,
        IsInteger = 1,
        IsSigned = 1,
        RequireInitialization = 0,
        ReadCost = 1,
        AddCost = 1,
        MulCost = 1
    };
  };
  inline const IntWrapper& real(const IntWrapper& x)  { return x; }

}
Eigen::array<Eigen::IndexPair<int>, 1> dimension = {Eigen::IndexPair<int>(1,0)};

Eigen::Tensor<Eigen::IntWrapper, 2> a(SIZE,SIZE);

Eigen::Tensor<Eigen::IntWrapper, 2> b(SIZE,SIZE);

Eigen::Tensor<Eigen::IntWrapper, 2> c(SIZE,SIZE);

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
