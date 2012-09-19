// Sherif Ghali

#ifndef NODE_H
#define NODE_H

#include "boost/shared_ptr.hpp"

template<typename HEDS_traits>
struct Halfedge;

template<typename HEDS_traits>
struct Face;

// A Node in a Half-Edge Data Structure (HEDS) is parameterized via
// HEDS_traits by the attributes of a Node, Parameter_node_type.
// The only data member it otherwise stores is a reference to one
// of it ougoing edges.

template<typename HEDS_traits>
struct Node
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

    My_edge_ptr outgoing_edge;
    Parameter_node_type nt;

    Node(const Parameter_node_type & _nt = Parameter_node_type())
        : nt(_nt) {}

    const Parameter_node_type info() const { return nt; }
    Parameter_node_type& info() { return nt; }

    unsigned int valence() const
    {
        My_edge_ptr e = outgoing_edge;
        if(!e)
            return 0;

        int valence = 0;
        do {
            // e = e->pred->twin;
            e = e->twin->succ;
            ++valence;
        } while(e != outgoing_edge);

        return valence;
    }

    // Precondition: Node has at least one outgoing edge.
    My_edge_ptr get_ccw_nth_outgoing_edge(int n) // first edge is #0
    {
        My_edge_ptr e = outgoing_edge;

        while(n) {
            e = e->pred->twin;
            --n;
        } 

        return e;
    }

    std::vector<My_node_ptr>
    get_adjacent_nodes() const
    {
        std::vector<My_node_ptr> N;
        My_edge_ptr e = outgoing_edge;
        if(!e)
            return N;
        N.push_back(e->target);

        do {
            e = e->twin->succ;  // or: e = e->pred->twin;
            N.push_back(e->target);
        } while(e != outgoing_edge);

        return N;
    }
};

#endif // NODE_H
