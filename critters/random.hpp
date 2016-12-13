// Sherif Ghali

#include <QtGlobal>

// These convenience functions are used by the Critters (Player and
// Prey) as well as by the Intelligence objects.

template<typename NT>
NT
get_int_random(int max) // return a random number in [0..max-1]
{
    // rand()/qrand()               in [0,2^15-1] - int
    // RAND_MAX                                    2^15
    return NT( double(max) * qrand()/(RAND_MAX+1.0) );
}


template<typename NT>
NT
get_real_random( const NT & from,
                 const NT & span)
// return a random number in [from, from+span)
{
    return from + span * NT( qrand()/(RAND_MAX+1.0) );
}
