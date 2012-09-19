// Sherif Ghali

#ifndef EPSILON_DOUBLE_H
#define EPSILON_DOUBLE_H

#include <cmath>
#include <iostream>

//----------------Epsilon_double_Begin----------------
static const double EPSILON = 1e-7;
class Epsilon_double
{
    double d;
public:
    Epsilon_double(double d = 0.0) : d(d) {}

    bool operator==(const Epsilon_double& d2) const
    { return (fabs(d2.d - d) < EPSILON); }
    bool operator!=(const Epsilon_double& d2) const
    { return !operator==(d2); }

    bool operator<(const Epsilon_double& d2) const
    { return d - d2.d < -EPSILON; }
    bool operator<=(const Epsilon_double& d2) const
    { return d - d2.d <= -EPSILON; }
    bool operator>(const Epsilon_double& d2) const
    { return d - d2.d > EPSILON; }
    bool operator>=(const Epsilon_double& d2) const
    { return d - d2.d >= EPSILON; }
//----------------Epsilon_double_End----------------

    Epsilon_double operator+() const { return   d; }
    Epsilon_double operator-() const { return - d; }

    Epsilon_double operator+(Epsilon_double d2) const { return d + d2.d; }
    Epsilon_double operator-(Epsilon_double d2) const { return d - d2.d; }
    Epsilon_double operator*(Epsilon_double d2) const { return d * d2.d; }
    Epsilon_double operator/(Epsilon_double d2) const { return d / d2.d; }

    // Epsilon_double  operator+=(Epsilon_double d2) { d += d2.d; return *this; }
    Epsilon_double& operator+=(const Epsilon_double& rhs) { d += rhs.d; return *this; }
    Epsilon_double& operator-=(const Epsilon_double& rhs) { d -= rhs.d; return *this; }

    double get_d() const { return d; }
};

Epsilon_double  cos(const Epsilon_double& d2) { return Epsilon_double(std::cos(d2.get_d())); }
Epsilon_double  sin(const Epsilon_double& d2) { return Epsilon_double(std::sin(d2.get_d())); }
Epsilon_double sqrt(const Epsilon_double& d2) { return Epsilon_double(std::sqrt(d2.get_d())); }

Epsilon_double fabs(const Epsilon_double& d2) { return d2.get_d() < 0.0 ? -d2.get_d() : d2.get_d(); }

double to_double(const Epsilon_double& d2) { return d2.get_d(); }

std::ostream&
operator<<(std::ostream& os, const Epsilon_double& d) { return os << d.get_d(); }


/// Specialization for Epsilon_double
namespace std {
    template<>
    struct numeric_limits<Epsilon_double>
    {
        static double min() throw()
        { return __DBL_MIN__; }
        static double max() throw()
        { return __DBL_MAX__; }
    };
}

#endif // EPSILON_DOUBLE_H
