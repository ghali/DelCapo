// Sherif Ghali

#ifndef RAY_E3_H
#define RAY_E3_H

typedef<typename T>
class Ray_E3
{
private:
    Point_E3<T>        _source;
    Direction_E3<T> _direction;
public:
    Ray_E3(const Point_E3<T>& source, const Direction_E3<T>& direction)
        : _source(source), _direction(direction)
    {}

    Point_E3<T> source() const { return _source; }
    Direction_E3<T> direction() const { return _direction; }
};

#endif // RAY_E3_H
