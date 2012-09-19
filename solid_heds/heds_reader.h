// Sherif Ghali

#ifndef HEDS_READER_H
#define HEDS_READER_H

#include <fstream>
#include <string>
#include <stdexcept>

#include "heds_types.h"

// HEDS_reader is an elementary parser for an OFF file in plain text.
// The interesting parts, stitching twin halfedges and setting each
// node's outgoing edge, are performed by the HEDS class itself.

template<typename HEDS_traits>
struct HEDS_reader
{
    // If false is returned, it means the file was read successfully.
    static bool
    read(HEDS<HEDS_traits>& heds,
         const std::string& filename)
    {
        std::ifstream in( filename.c_str() );

        if ( !in) {
            std::cerr << "Cannot open '"<< filename << std::endl;
            return true;
        }

        std::string offident;
        in >> offident;
        if(offident != "OFF") {
            std::cerr << "OFF file does not start with keyword 'OFF'\n";
            return true;
        }

        int numV, numF, unusedNumE;
        in >> numV >> numF >> unusedNumE;

        double x,y,z;
        in >> x >> y >> z;

        Point_E3f myP(static_cast<NumberType>(x),
                      static_cast<NumberType>(y),
                      static_cast<NumberType>(z));

        heds.V.push_back( My_node_ptr(new My_node(myP)) );

        // Read OFF vertices
        for(int i=1; i<numV; i++) {
            in >> x >> y >> z;
            Point_E3f myP2(static_cast<NumberType>(x),
                           static_cast<NumberType>(y),
                           static_cast<NumberType>(z));

            heds.V.push_back( My_node_ptr(new My_node(myP2)) );
        }
        // Read OFF polygons
        for(int f=0; f<numF; f++) {
            int numSides;
            in >> numSides;

            int v0, v1;
            in >> v0 >> v1;
            
            // Create e01, but we can't set its pred or its pred's succ

            // set source and target
            My_edge_ptr e01( new My_edge(heds.V[v0], heds.V[v1]) );
            heds.E.push_back(e01);
            My_face_ptr face( new My_face(e01, Polygon()) );
            // face->set_color(Color(0.5,0.5,0.5));
            heds.F.push_back(face);
            e01->adjFace = face;

            int lastV = v1;
            My_edge_ptr lastE = e01;
            for(int v=2; v<numSides; v++) {
                int vindex;
                in >> vindex;
                // Create e and set its pred (e->pred) and lastE->succ
                My_edge_ptr e( new My_edge(heds.V[lastV], heds.V[vindex], face) );
                heds.E.push_back(e);
                e->pred = lastE;
                lastE->succ = e;

                lastV = vindex;
                lastE = e;
            }

            // Create e_numSides_0 and set succ/pred
            My_edge_ptr eclose( new My_edge(heds.V[lastV], heds.V[v0], face) );
            heds.E.push_back(eclose);
            eclose->pred = lastE;
            lastE->succ = eclose;

            // Now set e01's pred
            e01->pred = eclose;
            eclose->succ = e01;

            // Set Normal directions
            Point_E3f p1 = face->edge->source->nt.coords;
            Point_E3f p2 = face->edge->target->nt.coords;
            Point_E3f p3 = face->edge->succ->target->nt.coords;
            Direction_E3f mydir = Direction_E3f(cross_product(Vector_E3f(p1,p2),Vector_E3f(p2,p3)));
            face->ft.normal = mydir;
        }    
        
        heds.stitch_twins();

        heds.set_node_outgoing_edge();

        if ( !in) { 
            std::cerr << "offview: read error: while reading file '" << filename << "'." << std::endl;
            return true;
        }
        return false;
    }
};

#endif // HEDS_READER_H
