#include <unsupported/Eigen/CXX11/Tensor>
#include <iostream>
#include <future>
#include <Eigen/Core>
#include <pthread.h>
#include <stdio.h>
#include <inttypes.h>
#include <map>
#include <mutex>

#define SIZE 64
std::map<pthread_t,std::map<float *, int>> threadIds;
std::mutex threadIdsProtector;
using namespace std;
namespace Eigen{
  class FloatWrapper
  {
  private:
    float value;
    bool flagEnable = true;
    void insertThreadId()
    {
      if (!flagEnable) return;
      threadIdsProtector.lock();
      pthread_t self = pthread_self();
      auto threadMap = threadIds.find(self);
      if(threadMap != threadIds.end())
      {
        auto data_addr = threadMap->second.find(&value);
        if (data_addr != threadMap->second.end())
        {
          ++data_addr->second;
        }
        else
        {
          threadMap->second[&value] = 1;
        }
      }
      else
      {
        std::map<float *,int> myMap;
        myMap[&value]=1;
        threadIds[self]=myMap;
        
      }
      threadIdsProtector.unlock();
    }
  public:
    FloatWrapper(float val=0)
    {
      //insertThreadId();
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
      insertThreadId();
      value += ref.value;
      return *this;
    }
    FloatWrapper operator*(const FloatWrapper & ref)
    {
      return FloatWrapper(value*ref.value);
    }
    FloatWrapper& operator=(const FloatWrapper & ref)
    {
      insertThreadId();
      value = ref.value;
      return *this;
    }
    FloatWrapper& operator=(const float i)
    {
      insertThreadId();
      value = i;
      return *this;
    }
    FloatWrapper(const FloatWrapper & tmp)
    {
      insertThreadId();
      value = tmp.value;
    }
    float &getValue()
    {
      return value;
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
        IsInteger = 0,
        IsSigned = 1,
        RequireInitialization = 1,
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

//typedef Eigen::FloatWrapper(0,true) CTYPE;
Eigen::Tensor<Eigen::FloatWrapper, 2> c(SIZE,SIZE);

void printThreadIds()
{
  for(auto &threadMaps: threadIds)
  {
    printf("%lu\n",threadMaps.first);
    for(auto &dataAddrs: threadMaps.second)
    {
      printf("  %" PRIuPTR " ", (uintptr_t)dataAddrs.first);
      printf("%d\n",dataAddrs.second);
    }
  }
}
void printResult()
{
  int i,j;
  FILE *fp = fopen("wrapper.txt", "w");
  for(i=0;i<SIZE;i++)
  {
    for(j=0;j<SIZE;j++)
    {
      fprintf(fp,"%f ",c(i,j).getValue());
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
  printThreadIds();
  //printResult();
  return 0;
}
