// Sherif Ghali

#include "heds_types.h"

// All HEDS classes are parameterized. Here we only have the three
// non-parameterized overloaded output operators.

//----------------------------------------------------------------
std::ostream&
operator<< (std::ostream& os, const Junction & J)
{
    return os << "  coords: " << J.coords;
}

std::ostream&
operator<< (std::ostream& os, const Route & R)
{
    return os << "  length: " << R.length
              << "  direction: " << R.direction;
}

std::ostream&
operator<< (std::ostream& os, const myPolygon)
{
    return os;                  // do nothing
}

