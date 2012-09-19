// Sherif Ghali

#ifndef HEDS_H
#define HEDS_H

#include <iostream>
#include <vector>
#include <map>
#include "boost/shared_ptr.hpp"

#include "node.h"
#include "halfedge.h"
#include "face.h"

template<typename HEDS_traits>
struct HEDS;

template<typename HEDS_traits>
struct HEDS_reader;

template<typename HEDS_traits>
std::ostream&
operator<< (std::ostream& os, const HEDS<HEDS_traits>& myHEDS);

// The HEDS class is an implementation of the Half-Edge Data
// Structure. The only unusual idea in this implementation is the use
// of traits as a common interface for the parameter types.

template<typename HEDS_traits>
struct HEDS
{
    friend class HEDS_reader<HEDS_traits>;

    typedef typename HEDS_traits::Node_type  Parameter_node_type;
    typedef typename HEDS_traits::Edge_type  Parameter_edge_type;
    typedef typename HEDS_traits::Face_type  Parameter_face_type;

    typedef Node<HEDS_traits>      My_node;
    typedef Halfedge<HEDS_traits>  My_edge;
    typedef Face<HEDS_traits>      My_face;

    typedef boost::shared_ptr<My_node> My_node_ptr;
    typedef boost::shared_ptr<My_edge> My_edge_ptr;
    typedef boost::shared_ptr<My_face> My_face_ptr;


    typedef std::vector< My_node_ptr >  My_node_container;
    typedef std::vector< My_edge_ptr >  My_edge_container;
    typedef std::vector< My_face_ptr >  My_face_container;

    My_node_container V;
    My_edge_container E;
    My_face_container F;


    unsigned int number_of_nodes() const { return V.size(); }
    unsigned int number_of_halfedges() const { return E.size(); }
    unsigned int number_of_faces() const { return F.size(); }

    void stitch_twins();

    void set_halfedge_succ_pred_links();
    void set_node_outgoing_edge();
    bool check_twins() const;

    // Euler operators
    void
    mev(My_edge_ptr adjpred,
        My_edge_ptr adjsucc,
        Node<HEDS_traits>     * vert);

    void
    mef(My_edge_ptr pred,
        My_edge_ptr succ);

    void
    kef(My_edge_ptr e);         // e->adjFace is the one deleted.

    friend
    std::ostream&
    operator<< <>(std::ostream& os, const HEDS<HEDS_traits>& myHEDS);

    //----------------------------------------------------------------
    // It is a bad idea to use the following more primitive interface.
    //----------------------------------------------------------------
#if 0
    virtual My_node *
    insert_node(const Parameter_node_type& nt =
                Parameter_node_type())
    {
        My_node* mynode = new My_node( nt );
        V.push_back(mynode);
        return mynode;
    }

    virtual My_edge *
    insert_edge(My_node* source, My_node* target,
                const Parameter_edge_type& et =
                Parameter_edge_type() )
    {
        My_edge* newedge = new My_edge(source, target, et);

        source->sourceOf.push_back(newedge);
        target->targetOf.push_back(newedge);

        E.push_back(newedge);
        return newedge;
    }
#endif

};

template<typename HEDS_traits>
std::ostream&
operator<< (std::ostream& os, const HEDS<HEDS_traits>& H)
{
    typedef typename HEDS<HEDS_traits>::My_node_ptr  My_node_ptr;
    typedef typename HEDS<HEDS_traits>::My_edge_ptr  My_edge_ptr;
    typedef typename HEDS<HEDS_traits>::My_face_ptr  My_face_ptr;

    typedef typename HEDS<HEDS_traits>::My_node_container::const_iterator  NodeP_iterator;
    typedef typename HEDS<HEDS_traits>::My_edge_container::const_iterator  EdgeP_iterator;
    typedef typename HEDS<HEDS_traits>::My_face_container::const_iterator  FaceP_iterator;

    int n = 0; std::map<My_node_ptr,int> MN;
    int e = 0; std::map<My_edge_ptr,int> ME;
    int f = 0; std::map<My_face_ptr,int> MF;

    for(NodeP_iterator vi = H.V.begin(); vi != H.V.end(); ++vi) MN[*vi] = n++;
    for(EdgeP_iterator ei = H.E.begin(); ei != H.E.end(); ++ei) ME[*ei] = e++;
    for(FaceP_iterator fi = H.F.begin(); fi != H.F.end(); ++fi) MF[*fi] = f++;

    os << "----------------Nodes----------------" << std::endl;
    for(NodeP_iterator vi = H.V.begin(); vi!= H.V.end(); ++vi)
        os << "Node " << MN[*vi]
           << "  outgoing_edge: " << ME[(*vi)->outgoing_edge]
           << "  valence: " << (*vi)->valence()
           << "  info: " << (*vi)->info() << std::endl;

    os << "----------------Halfedges----------------" << std::endl;
    for(EdgeP_iterator ei = H.E.begin(); ei!=H.E.end(); ++ei)
        os << "Edge " << ME[*ei]
           << "  source: " << MN[(*ei)->source]
           << "  target: " << MN[(*ei)->target]
           << "  adjFace: " << MF[(*ei)->adjFace]
           << "  pred: " << ME[(*ei)->pred]
           << "  succ: " << ME[(*ei)->succ]
           << "  twin: " << ME[(*ei)->twin]
           << "  info: " << (*ei)->info() << std::endl;

    os << "----------------Faces----------------" << std::endl;
    for(FaceP_iterator fi = H.F.begin(); fi!=H.F.end(); ++fi)
        os << "Face " << MF[*fi]
           << "  edge: " << ME[(*fi)->edge]
           << "  info: " << (*fi)->info() << std::endl;

    return os << std::endl;
}


// Stitch_Twins_By_Associative_Container
template<typename HEDS_traits>
void
HEDS<HEDS_traits>::stitch_twins()
{
    typedef typename My_edge_container::iterator My_edge_iterator;

    typedef std::pair<My_node_ptr, My_node_ptr > My_vertex_pair;

    typedef std::map<My_vertex_pair, My_edge_ptr>      My_map;
    typedef typename My_map::iterator            My_iterator;
    My_map M;

    for(My_edge_iterator hei = E.begin(); hei!= E.end(); ++hei) {
	My_vertex_pair my_vertex_pair_key( (*hei)->source, (*hei)->target );
	M.insert(std::make_pair(my_vertex_pair_key, *hei));
    }

    for(My_edge_iterator hei = E.begin(); hei!= E.end(); ++hei) {
	My_vertex_pair my_vertex_pair_key( (*hei)->target, (*hei)->source );
	My_iterator i = M.find(my_vertex_pair_key);
	if(i != M.end())
	    (*hei)->twin = i->second;
    }
}

template<typename HEDS_traits>
bool
is_twin(Halfedge<HEDS_traits>* e1,
        Halfedge<HEDS_traits>* e2)
{
    if(e1->source == e2->target && e2->source == e1->target)
        return true;
    return false;
}

template<typename HEDS_traits>
void
HEDS<HEDS_traits>::set_halfedge_succ_pred_links()
{
    typedef typename My_node_container::iterator     NodeP_iterator;
    typedef typename My_edge_container::iterator HalfedgeP_iterator;

}

template<typename HEDS_traits>
void
HEDS<HEDS_traits>::set_node_outgoing_edge()
{
    typedef typename My_node_container::iterator     NodeP_iterator;
    typedef typename My_edge_container::iterator HalfedgeP_iterator;

    std::map<My_node_ptr, My_edge_ptr> M;
    for(HalfedgeP_iterator hei = E.begin(); hei!= E.end(); ++hei)
        if(!M[(*hei)->source])
            M[(*hei)->source] = (*hei);

    for(NodeP_iterator vi = V.begin(); vi!= V.end(); ++vi)
        (*vi)->outgoing_edge = M[(*vi)];
}

template<typename HEDS_traits>
bool
HEDS<HEDS_traits>::check_twins() const
{
    typedef typename My_edge_container::const_iterator HalfedgeP_iterator;
    for(HalfedgeP_iterator it = E.begin(); it != E.end(); ++it)
    {
        if(*it != (*it)->succ->pred)
            return false;
        if(*it != (*it)->twin->twin)
            return false;
    }
    return true;
}

template<typename HEDS_traits>
void
HEDS<HEDS_traits>::mev(My_edge_ptr adjpred,
                       My_edge_ptr adjsucc,
                       Node<HEDS_traits> * vert)
{
    // Makes an edge node:
    // The vertex is the new node.
    // The two halfedges are the predecessor and successor of the node
    //  that will become the new source node of the new halfedge.

    My_edge_ptr     edge( new My_edge(adjpred->target,vert,adjpred->adjFace) );
    My_edge_ptr twinedge( new My_edge(vert,adjsucc->source,adjsucc->adjFace) );
    edge->pred = adjpred;
    adjpred->succ = edge;
    edge->succ = twinedge;
    twinedge->pred = edge;
    twinedge->succ = adjsucc;
    adjsucc->pred = twinedge;
    E.push_back(edge);
    E.push_back(twinedge);
}

template<typename HEDS_traits>
void
HEDS<HEDS_traits>::mef(My_edge_ptr pred,
                       My_edge_ptr succ)
{
    // Make edge face:
    // The pred and succ halfedge pointers are the predecessor
    //  and the successor of the half edge that will be created, with
    //  new face being adjacent to that half edge.

    My_edge_ptr     edge( new My_edge(pred->target,succ->source) );
    My_edge_ptr twinedge( new My_edge(succ->source,pred->target,pred->adjFace) );
    twinedge->succ = pred->succ;
    twinedge->pred = succ->pred;
    twinedge->succ->pred = twinedge;
    twinedge->pred->succ = twinedge;

    twinedge->adjFace->e = twinedge;  
    edge->pred = pred;
    edge->succ = succ;
    edge->pred->succ = edge;
    edge->succ->pred = edge;

    My_face_ptr newFace( new My_face(edge) );
    F.push_back(newFace);
    E.push_back(twinedge);
    E.push_back(edge);
    My_edge_ptr nextedge = edge;
    do {
        nextedge->adjFace = newFace;
        nextedge = nextedge->succ;
    } while(nextedge != edge); 
    My_face * te = twinedge->adjFace; 
}

template<typename HEDS_traits>
void
HEDS<HEDS_traits>::kef(My_edge_ptr e)
{
    e->twin->pred->succ = e->succ;
    e->succ->pred = e->twin->pred;

    e->twin->succ->pred = e->pred;
    e->pred->succ = e->twin->succ;

    if(e->source->outgoing_edge == e)
        e->source->outgoing_edge = e->twin->succ;        
    if(e->target->outgoing_edge == e->twin)
        e->target->outgoing_edge = e->succ;

    for(My_edge_ptr p = e->succ; p != e->pred; p = p->succ)
        p->adjFace = e->twin->adjFace;
}        

#endif // HEDS_H
