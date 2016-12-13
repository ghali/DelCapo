// Sherif Ghali

#ifndef FACE_H
#define FACE_H

#include "boost/shared_ptr.hpp"

template<typename HEDS_traits>
struct HEDS;

template<typename HEDS_traits>
struct Halfedge;

// A Face in a Half-Edge Data Structure (HEDS) is parameterized via
// HEDS_traits by the attributes of a Face, Parameter_face_type.
// The only data member it otherwise stores is a reference to one
// adjacent edge.

template<typename HEDS_traits>
struct Face
{
    typedef typename HEDS_traits::Node_type  Parameter_node_type;
    typedef typename HEDS_traits::Edge_type  Parameter_edge_type;
    typedef typename HEDS_traits::Face_type  Parameter_face_type;

    typedef Node<HEDS_traits>      My_node;
    typedef Halfedge<HEDS_traits>  My_edge;
    typedef Face<HEDS_traits>      My_face;

    typedef boost::shared_ptr<My_node> My_node_ptr;
    typedef boost::shared_ptr<My_edge> My_edge_ptr;
    typedef boost::shared_ptr<My_face> My_face_ptr;

    My_edge_ptr edge;
    Parameter_face_type ft;

    Face(My_edge_ptr myedge,
         const Parameter_face_type & _ft = Parameter_face_type())
        : edge(myedge), ft(_ft) {}

    const Parameter_face_type info() const { return ft; }
    Parameter_face_type& info() { return ft; }

    unsigned int valence() const
    {
        My_edge_ptr e = edge;
        if(!e)
            return 0;

        int valence = 0;
        do {
            e = e->succ;
            ++valence;
        } while(e != edge);

        return valence;
    }
};

#endif // FACE_H
