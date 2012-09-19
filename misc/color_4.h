// Sherif Ghali

#include <iostream>

#ifndef COLOR_4_H
#define COLOR_4_H

template<typename Color_unit>
class Color_4;

template<typename Color_unit>
std::ostream& operator<< (std::ostream& os, const Color_4<Color_unit>& c);

template<typename Color_unit>
class Color_4
{
    Color_unit _r,_g,_b,_a;
public:
    Color_4()
        : _r(Color_unit()), _g(Color_unit()),
          _b(Color_unit()), _a(Color_unit())
    {}
    Color_4(const Color_unit rr, const Color_unit gg,
            const Color_unit bb, const Color_unit aa)
        : _r(rr), _g(gg), _b(bb), _a(aa)
    {}

    Color_unit r() const { return _r; }
    Color_unit g() const { return _g; }
    Color_unit b() const { return _b; }
    Color_unit a() const { return _a; }

    void set_r(const Color_unit& u) { _r = u; }
    void set_g(const Color_unit& u) { _g = u; }
    void set_b(const Color_unit& u) { _b = u; }
    void set_a(const Color_unit& u) { _a = u; }

    Color_4<Color_unit>&
    operator=(const Color_4<Color_unit>& c)
    {
        if(this != &c) {
            _r = c._r;
            _g = c._g;
            _b = c._b;
            _a = c._a;
        }
        return *this;
    }

    bool
    operator==(const Color_4<Color_unit>& c) const
    {
        return (this == &c) || (_r == c._r && _g == c._g && _b == c._b && _a == c._a);
    }

    friend std::ostream& operator<< <>(std::ostream& os, const Color_4<Color_unit>& c);
};

template<typename Color_unit>
std::ostream& operator<< (std::ostream& os, const Color_4<Color_unit>& c)
{
    return os << int(c._r) << " " << int(c._g) << " " << int(c._b) << " " << int(c._a);
}

#endif // COLOR_4_H
