//
// Created by BeejayInes on 11/15/2017.
//

#ifndef SHAKA_SCHEME_NUMBER_HPP
#define SHAKA_SCHEME_NUMBER_HPP

#include "shaka_scheme/system/exceptions/InvalidInputException.hpp"
#include "shaka_scheme/system/exceptions/TypeException.hpp"

#include "shaka_scheme/system/base/Number/Real.hpp"
#include "shaka_scheme/system/base/Number/Rational.hpp"
#include "shaka_scheme/system/base/Number/Integer.hpp"

#include <memory>
#include <cmath>

namespace shaka {
class Real;
class Rational;
class Integer;

class Number;

class Number {
public:
  enum class Type : int {
    REAL,
    RATIONAL,
    INTEGER
  };
private:
  Type type_tag;

  union {
    shaka::Real real = shaka::Real(0);
    shaka::Rational rational = shaka::Rational(0,0);
    shaka::Integer integer = shaka::Integer(0);
  };

public:
  Number(shaka::Real other) {
    new(&real) shaka::Real(other);
    this->type_tag = Type::REAL;
  }

  Number(shaka::Rational other) {
    new(&rational) shaka::Rational(other);
    this->type_tag = Type::RATIONAL;
  }

  Number(shaka::Integer other) {
    new(&integer) shaka::Integer(other);
    this->type_tag = Type::INTEGER;
  }

  // arithmetic operators R7RS 6.2.6
  friend Number operator+(const Number& n1, const Number& n2);
  friend Number operator-(const Number& n1, const Number& n2);
  friend Number operator*(const Number& n1, const Number& n2);
  friend Number operator/(const Number& n1, const Number& n2);
  friend Number operator|(const Number& n1, const Number& n2);
  friend Number operator%(const Number& n1, const Number& n2);

  // comparison operators R7RS 6.2.6
  friend bool operator==(const Number& n1, const Number& n2);
  friend bool operator!=(const Number& n1, const Number& n2);
  friend bool operator>(const Number& n1, const Number& n2);
  friend bool operator<(const Number& n1, const Number& n2);
  friend bool operator>=(const Number& n1, const Number& n2);
  friend bool operator<=(const Number& n1, const Number& n2);

  friend std::ostream& operator<< (std::ostream& lhs, const Number& rhs);


};

Number operator+(const Number& n1, const Number& n2) {
  Number result(n1 + n2);
  return result;
}

Number operator-(const Number& n1, const Number& n2) {
  Number result(n1 - n2);
  return result;
}

Number operator*(const Number& n1, const Number& n2) {
  Number result(n1 * n2);
  return result;
}

Number operator/(const Number& n1, const Number& n2) {
  Number result(n1 / n2);
  return result;
}

Number operator|(const Number& n1, const Number& n2) {
  Number result(n1 | n2);
  return result;
}

Number operator%(const Number& n1, const Number& n2) {
  Number result(n1 % n2);
  return result;
}
//---------------------------------------------------
// Comparison operator overloads
//--------------------------------------------------
bool operator==(const Number& n1, const Number& n2) {
  return (n1 == n2);
}

bool operator!=(const Number& n1, const Number& n2) {
  return !(n1 == n2);
}

bool operator<(const Number& n1, const Number& n2) {
  return !(n1 == n2) && !(n1 > n2);
}

bool operator>(const Number& n1, const Number& n2) {
  return !(n1 == n2) && !(n1 < n2);
}

bool operator>=(const Number& n1, const Number& n2) {
  return (n1 > n2) || (n1 == n2);
}

bool operator<=(const Number& n1, const Number& n2) {
  return (n1 < n2) || (n1 == n2);
}


//template<> shaka::Real& shaka::Number::get<shaka::Real>();
//template<> shaka::Rational& shaka::Number::get<shaka::Rational>();
//template<> shaka::Integer& shaka::DataNumebr::get<shaka::Integer>();

//std::ostream& operator<<(std::ostream& lhs, shaka::Number rhs);

} //namespace shaka

#endif //SHAKA_SCHEME_NUMBER_HPP
