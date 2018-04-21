#include <unsupported/Eigen/CXX11/Tensor>
#include <iostream>
#include <future>
#include <Eigen/Core>

#define SIZE 1000

using namespace std;
namespace Eigen{
  class FloatWrapper
  {
  public:
    float value;
    FloatWrapper(float val=0)
    {
      value = val;
    }
    FloatWrapper operator+(const FloatWrapper & ref)
    {
      return FloatWrapper(value+ref.value);
    }
    FloatWrapper operator-(const FloatWrapper & ref)
    {
      return FloatWrapper(value-ref.value);
    }
    FloatWrapper& operator+=(const FloatWrapper & ref)
    {
      value += ref.value;
      return *this;
    }
    FloatWrapper operator*(const FloatWrapper & ref)
    {
      return FloatWrapper(value*ref.value);
    }
    FloatWrapper& operator=(const FloatWrapper & ref)
    {
      value = ref.value;
      return *this;
    }
    FloatWrapper& operator=(const float i)
    {
      value = i;
      return *this;
    }
    friend FloatWrapper operator+(const FloatWrapper &, const FloatWrapper &);
    friend FloatWrapper operator*(const FloatWrapper &, const FloatWrapper &);
  };
  FloatWrapper operator*(const FloatWrapper & lhs, const FloatWrapper &rhs)
  {
    return FloatWrapper(lhs.value*rhs.value);
  }
  FloatWrapper operator+(const FloatWrapper & lhs, const FloatWrapper &rhs)
  {
    return FloatWrapper(lhs.value+rhs.value);
  }
  template<> struct NumTraits<FloatWrapper> : GenericNumTraits<FloatWrapper>
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
  const FloatWrapper& real(const FloatWrapper& x)  { return x; }

}
Eigen::array<Eigen::IndexPair<int>, 1> dimension = {Eigen::IndexPair<int>(1,0)};

Eigen::Tensor<Eigen::FloatWrapper, 2> a(SIZE,SIZE);

Eigen::Tensor<Eigen::FloatWrapper, 2> b(SIZE,SIZE);

Eigen::Tensor<Eigen::FloatWrapper, 2> c(SIZE,SIZE);

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
