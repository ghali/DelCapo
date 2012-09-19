// Sherif Ghali

#ifndef COLOR_3_H
#define COLOR_3_H

template<typename Color_unit>
class Color_3
{
    Color_unit _r, _g, _b;

public:
    Color_3(const Color_unit rr = Color_unit(),
            const Color_unit gg = Color_unit(),
            const Color_unit bb = Color_unit())
        : _r(rr), _g(gg), _b(bb)
    {}

    Color_unit r() const { return _r; }
    Color_unit g() const { return _g; }
    Color_unit b() const { return _b; }

    void set_r(const Color_unit& u) { _r = u; }
    void set_g(const Color_unit& u) { _g = u; }
    void set_b(const Color_unit& u) { _b = u; }

    Color_3<Color_unit>&
    operator=(const Color_3<Color_unit>& c)
    {
        if(this != &c) {
            _r = c._r;
            _g = c._g;
            _b = c._b;
        }

        return *this;
    }

    bool
    operator==(const Color_3<Color_unit>& c) const
    {
        return (this == &c) || (_r == c._r && _g == c._g && _b == c._b);
    }
};

#endif // COLOR_3_H
