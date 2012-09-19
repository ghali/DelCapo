// Sherif Ghali

#ifndef PTR_TYPES_H
#define PTR_TYPES_H

// Important:
// Do not scatter Prey* and Intelligence* throughout the program.
// Use the typedefs below.
// 
// This way changing between ordinary and shared pointers can be done
// by modifying this one file.

// Boost
#include <boost/shared_ptr.hpp>

// QDelCapo
#include "../critters/prey.h"
#include "../critters/intelligence.h"

typedef boost::shared_ptr<Prey>                 Prey_ptr;

// Notice that we rely on Intelligence::update() _not_ creating a new
// (shared) pointer, so we cannot use a shared pointer for Intelligence.
// If we want to, Intelligence::update() must query Scene asking for
// the pointer stored both in Scene and in Maze::node_to_intelli.

typedef Intelligence * Intelligence_ptr;

#endif // PTR_TYPES_H
