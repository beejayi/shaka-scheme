#ifndef REAL_H
#define REAL_H

#include <cmath>

namespace shaka {

class Real {
public:
	Real(double v) : value(v) {}
	
	// arithmetic operators
	friend Real operator+(const Real& lhs, const Real& rhs);
	friend Real operator-(const Real& lhs, const Real& rhs);
	friend Real operator*(const Real& lhs, const Real& rhs);
	friend Real operator/(const Real& lhs, const Real& rhs);

	// comparison operators
	friend bool operator==(const Real& lhs, const Real& rhs);
	friend bool operator>(const Real& lhs, const Real& rhs);
	friend bool operator<(const Real& lhs, const Real& rhs);
	friend bool operator>=(const Real& lhs, const Real& rhs);
	friend bool operator<=(const Real& lhs, const Real& rhs);
	friend bool operator!=(const Real& lhs, const Real& rhs);

	double get_value() const;


private:
	double value;
};

double Real::get_value() const{
	return value;
}

Real operator+(const Real& lhs, const Real& rhs) {
	Real result(lhs.value + rhs.value);
	return result;
}

Real operator-(const Real& lhs, const Real& rhs) {
	Real result(lhs.value - rhs.value);
	return result;
}

Real operator*(const Real& lhs, const Real& rhs) {
	Real result(lhs.value * rhs.value);
	return result;
}

Real operator/(const Real& lhs, const Real& rhs) {
	Real result(lhs.value / rhs.value);
	return result;
}

bool operator==(const Real& lhs, const Real& rhs) {
	return lhs.value == rhs.value;
}

bool operator>(const Real& lhs, const Real& rhs) {
	return lhs.value > rhs.value;
}

bool operator<(const Real& lhs, const Real& rhs) {
	return lhs.value < rhs.value;
}

bool operator>=(const Real& lhs, const Real& rhs) {
	return lhs.value >= rhs.value;
}

bool operator<=(const Real& lhs, const Real& rhs) {
	return lhs.value <= rhs.value;
}

bool operator!=(const Real& lhs, const Real& rhs) {
	return lhs.value != rhs.value;
}

} // namespace shaka

#endif
