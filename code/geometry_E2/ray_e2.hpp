// Sherif Ghali

#ifndef RAY_E2_H
#define RAY_E2_H

typedef<typename NT>
class Ray_E2
{
private:
    Point_E2<NT>        _source;
    Direction_E2<NT> _direction;
public:
    Ray_E2(const Point_E2<NT>& source, const Direction_E2<NT>& direction)
        : _source(source), _direction(direction)
    {}

    Point_E2<NT> source() const { return _source; }
    Direction_E2<NT> direction() const { return _direction; }
};

#endif // RAY_E2_H
