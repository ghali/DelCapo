// Sherif Ghali

#ifndef RANDOM_E2_H
#define RANDOM_E2_H

#include "geometry_E2/point_e2.h"
#include "geometry_E2/vector_e2.h"
#include "geometry_E2/line_e2.h"
#include "geometry_E2/segment_e2.h"

// srand48(0) must be called explicitly on cygwin before drand48 is used.

template<typename NT>
void
set_to_random( Point_E2<NT>& P )
{
    NT x = NT(rand()/(RAND_MAX+1.0));
    NT y = NT(rand()/(RAND_MAX+1.0));
    P = Point_E2<NT>(x,y);
}

template<typename NT>
void
set_to_random( Vector_E2<NT>& V )
{
    NT x = NT(rand()/(RAND_MAX+1.0));
    NT y = NT(rand()/(RAND_MAX+1.0));
    V = Vector_E2<NT>(x,y);
}

template<typename NT>
void
set_to_random( Line_E2<NT>& L )
{
    Point_E2<NT> rnd_src, rnd_tgt;
    set_to_random(rnd_src);
    do { set_to_random(rnd_tgt); } while( rnd_src == rnd_tgt );

    L = Line_E2<NT>(rnd_src, rnd_tgt);
}

template<typename NT>
void
set_to_random( Segment_E2<NT>& S )
{
    Point_E2<NT> rnd_src, rnd_tgt;
    set_to_random(rnd_src);
    do { set_to_random(rnd_tgt); } while( rnd_src == rnd_tgt );

    S = Segment_E2<NT>(rnd_src, rnd_tgt);
}

#endif // RANDOM_E2_H
