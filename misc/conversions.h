// Sherif Ghali

#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

template<typename I>
std::string
integer_to_zero_padded_string(const I & i, int width = 4)
{
    std::stringstream s;
    s << std::setfill('0') << std::setw(width) << i;
    return s.str();
}

template<typename I>
char const *
integer_to_char_const_ptr(const I & i, int width = 4)
{
    std::stringstream s;
    s << std::setw(width) << i;
    return s.str().c_str();
}

// Use double_to_string for a human-readable output.
template<typename D>
std::string
double_to_string(const D& d, int width = 3, int precision = 1 )
{
    std::stringstream s;
    s << std::fixed << std::setw(width)
      << std::setprecision(precision) << d;
    return s.str();
}

// Use either of the following two functions to generate
// zero-padded strings, which are useful as filenames.
template<typename D>
std::string
double_to_string_with_period(const D& d, int width = 3, int precision = 1 )
{
    std::stringstream s;
    s.setf(std::ios::showpos);
    s << std::setfill('0') << std::fixed << std::setw(width)
      << std::setprecision(precision) << d;
    return s.str();
}

// Avoid a period as it often signals an extension in a filename.
template<typename D>
std::string
double_to_string_fn(const D& d, int width = 3, int precision = 1 )
{
    std::stringstream s;
    s.setf(std::ios::showpos);
    s << std::setfill('0') << std::fixed << std::setw(width)
      << std::setprecision(precision) << d;

    std::string str = s.str();
    std::string::size_type st = str.find(".");
    if( st )
        str.replace(st, 1, "p");
    return str;
}

template<typename T>
float
to_float(const T& st)
{
    return float(st);
}

template<typename T>
double
to_double(const T& st)
{
    return double(st);
}

#ifdef __GMP_PLUSPLUS__
template<>
double
to_double(const mpq_class& st)
{
    return st.get_d();
}
#endif

#endif // CONVERSIONS_H
