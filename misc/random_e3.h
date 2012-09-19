// Sherif Ghali

#ifndef RANDOM_E3_H
#define RANDOM_E3_H

#include "geometry_E3/point_e3.h"
#include "geometry_E3/direction_e3.h"
#include "geometry_E3/segment_e3.h"

// srand48(0) must be called explicitly on cygwin before drand48 is used.

template<typename NT>
void
set_to_random( Point_E3<NT>& P )
{
    NT x = NT(rand()/(RAND_MAX+1.0));
    NT y = NT(rand()/(RAND_MAX+1.0));
    NT z = NT(rand()/(RAND_MAX+1.0));
    P = Point_E3<NT>(x,y,z);
}

template<typename NT>
void
set_to_random( Direction_E3<NT>& D )
{
    NT x, y, z;
    do {
        x = NT(2.0 * double(rand())/(RAND_MAX+1.0) - 1.0);
        y = NT(2.0 * double(rand())/(RAND_MAX+1.0) - 1.0);
        z = NT(2.0 * double(rand())/(RAND_MAX+1.0) - 1.0);
    } while( x==0 && y==0 && z==0 );
    D = Direction_E3<NT>(x,y,z);
}

template<typename NT>
void
set_to_random( Segment_E3<NT>& S )
{
    Point_E3<NT> rnd_src, rnd_tgt;
    set_to_random(rnd_src);
    do { set_to_random(rnd_tgt); } while( rnd_src == rnd_tgt );

    S = Segment_E3<NT>(rnd_src, rnd_tgt);
}

#endif // RANDOM_E3_H
