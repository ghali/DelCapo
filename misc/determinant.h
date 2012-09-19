// Sherif Ghali

#ifndef DETERMINANT_H
#define DETERMINANT_H

template<typename T>
inline
T
determinant(const T& a, const T& b,
            const T& c, const T& d)
{
    return a * d - b * c;
}

template<typename T>
inline
T
determinant(const T& a, const T& b, const T& c,
            const T& d, const T& e, const T& f,
            const T& g, const T& h, const T& i)
{
    return a * determinant(e,f,h,i)
        -  b * determinant(d,f,g,i)
        +  c * determinant(d,e,g,h);
}

template<typename T>
inline
T
determinant(const T& a, const T& b, const T& c, const T& d,
            const T& e, const T& f, const T& g, const T& h,
            const T& i, const T& j, const T& k, const T& l,
            const T& m, const T& n, const T& o, const T& p)
{
    return a * determinant(f,g,h, j,k,l, n,o,p)
        -  b * determinant(e,g,h, i,k,l, m,o,p)
        +  c * determinant(e,f,h, i,j,l, m,n,p)
        -  d * determinant(e,f,g, i,j,k, m,n,o);
}

template<typename T>
inline
T
inner_product(const T& x1, const T& y1,
              const T& x2, const T& y2)
{
    return x1 * x2 + y1 * y2;
}

template<typename T>
inline
T
inner_product(const T& x1, const T& y1, const T& z1,
              const T& x2, const T& y2, const T& z2)
{
    return x1 * x2 + y1 * y2 + z1 * z2;
}

template<typename T>
inline
T
inner_product(const T& x1, const T& y1, const T& z1, const T& w1,
              const T& x2, const T& y2, const T& z2, const T& w2)
{
    return x1 * x2 + y1 * y2 + z1 * z2 + w1 * w2;
}

template<typename T>
inline
bool
are_dependent(
              const T& a1, const T& b1, const T& c1,
              const T& a2, const T& b2, const T& c2 )
{
    T zero = static_cast<T>(0);
    return (
            determinant(a1, b1, a2, b2) == zero &&
            determinant(a1, c1, a2, c2) == zero &&
            determinant(b1, c1, b2, c2) == zero );
}

template<typename T>
inline
bool
are_dependent(
              const T& a1, const T& b1, const T& c1, const T& d1,
              const T& a2, const T& b2, const T& c2, const T& d2 )
{
    T zero = static_cast<T>(0);
    return (
            determinant(a1, b1, a2, b2) == zero &&
            determinant(a1, c1, a2, c2) == zero &&
            determinant(a1, d1, a2, d2) == zero &&
            determinant(b1, c1, b2, c2) == zero &&
            determinant(b1, d1, b2, d2) == zero &&
            determinant(c1, d1, c2, d2) == zero );
}

template<class T>
inline
void
cross_product(const T& v1x, const T& v1y, const T& v1z,
              const T& v2x, const T& v2y, const T& v2z,
              T& vx, T& vy, T& vz)
{
    vx = + determinant( v1y, v2y, v1z, v2z );
    vy = - determinant( v1x, v2x, v1z, v2z );
    vz = + determinant( v1x, v2x, v1y, v2y );
}

#endif // DETERMINANT_H
