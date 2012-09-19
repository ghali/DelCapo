// Sherif Ghali

#ifndef HALFEDGE_H
#define HALFEDGE_H

#include "boost/shared_ptr.hpp"

template<typename HEDS_traits>
struct Face;

// A Halfedge in a Half-Edge Data Structure (HEDS) is parameterized via
// HEDS_traits by the attributes of an edge, Parameter_edge_type.
// 
// It also stores references to adjacent nodes and halfedges, as well
// as to the adjancent Face.

template<typename HEDS_traits>
struct Halfedge
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

    My_node_ptr   source, target;
    My_face_ptr          adjFace;
    My_edge_ptr       pred, succ;
    My_edge_ptr             twin;

    Parameter_edge_type et;

    Halfedge(My_node_ptr source,
             My_node_ptr target,
             const Parameter_edge_type & _et = Parameter_edge_type())
        : source(source), target(target), adjFace(My_face_ptr()), et(_et)
    {}

    Halfedge(My_node_ptr source,
             My_node_ptr target,
             My_face_ptr adjFace,
             const Parameter_edge_type & _et = Parameter_edge_type())
        : source(source), target(target), adjFace(adjFace), et(_et)
    {}

    const Parameter_edge_type info() const { return et; }
    Parameter_edge_type& info() { return et; }
};

#endif // HALFEDGE_H
