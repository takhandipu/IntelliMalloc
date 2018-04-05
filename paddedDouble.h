#ifndef PADDEDDOUBLE_H
#define PADDEDDOUBLE_H

#include <Eigen/Core>
namespace Padding
{
  class PaddedDouble
  {
  public:
    double value;
    double padding[15];
    PaddedDouble(double v=0.0)
    {
      this->value = v;
    }
  };
  inline const PaddedDouble& conj(const PaddedDouble& x) {return x;}
  inline const PaddedDouble& real(const PaddedDouble& x) {return x;}
} //namespace Padding
namespace Eigen {
template<> struct NumTraits<PaddedDouble>: NumTraits<double>
{
  typedef PaddedDouble Real;
  typedef PaddedDouble NonInteger;
  typedef PaddedDouble Nested;
  enum {
    IsComplex = 0,
    IsInteger = 0,
    IsSigned = 1,
    RequiredInitialization = 1,
    ReadCost = 1,
    AddCost = 3,
    MulCost = 3
  }; 
};
} //namespace Eigen
#endif //PADDEDDOUBLE_H
