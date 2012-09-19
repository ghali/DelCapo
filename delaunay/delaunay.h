// delaunay.h

#ifndef DELAUNAY_H
#define DELAUNAY_H

#include <vector>

#include "misc/orientation.h"

// template< typename Point_input_iterator > class Delaunay
// computes the Delaunay triangulation of n points in optimal
// O(n log(n)) time and space. The class is suitable for computing
// the Delaunay triangulation in either the plane or on a sphere.
// Point_input_iterator should resolve to an iterator of either
// a point in the plane or a point on a sphere.


//----------------Delaunay_Begin----------------
template<typename Iterator>
struct Geometry_iterator_traits : std::iterator_traits<Iterator>
{
    typedef typename Iterator::value_type::number_type number_type;
};

template< typename Point_input_iterator >
class Delaunay
{
    typedef typename Geometry_iterator_traits<Point_input_iterator>::value_type Point;

public:
    static const int NO_EDGE = -1;

    // Each Delaunay_point stores a circular doubly linked list of the
    // vertices adjacent to it. (int v) is the index of one adjacent
    // vertex, and (*pred) and (*succ) are pointers to the two
    // adjacent nodes in the circular list.

    struct Vertex {
        int v;
        Vertex *pred, *succ;
        Vertex() {}
    };

    // Delaunay_point stores the (geometric) point p as well as
    // (*first), the first vertex adjacent to it.
    // 
    // If the Delaunay_point is a convex hull point, then
    // (Vertex *first) is the first vertex when traversing
    // the hull in ccw order.
    // 
    struct Delaunay_point {
        Point p;
        Vertex *first;
        Delaunay_point() : p(), first(NULL) {}

        Delaunay_point(Point _p, Vertex *_first)
            : p(_p), first(_first)
        {}
    };

    std::vector<Delaunay_point>  ver;
//----------------Delaunay_End----------------

    struct EDGE {
        int a, b;
    };

    int    num_points;           // starts from 0

private:
    static bool
    lex_compare_vertices( Delaunay_point a, Delaunay_point b )
    {
        return lex_xy_compare(a.p, b.p);
    }

    //----------------------------------------------------------------
    void simple2(int begin, int end)
    {
        Vertex * one     =  new Vertex();
        one->succ        = one->pred = one;
        one->v           = end;
        ver[begin].first = one;

        Vertex * two     =  new Vertex();  
        two->succ        = two->pred = two;
        two->v           = begin;
        ver[end].first   = two;
    }

    //----------------------------------------------------------------
    // We choose to stop the recursion when n is either 2 or 3. Implementing
    // the case for n=3 (function simple3()) cuts the recursion short.
    // simple2() would be enough.
    void simple3(int begin, int end)
    {
        int mid = begin + 1;

        int mid_is_left = oriented_side(ver[begin].p, ver[end].p, ver[mid].p) == ON_POSITIVE_SIDE;

        Vertex * vmid =  new Vertex();
        Vertex * vend =  new Vertex();
        vmid->succ = vmid->pred = vend;
        vend->succ = vend->pred = vmid;
        vmid->v = mid;
        vend->v = end;
  
        if (mid_is_left)
            ver[begin].first = vend;
        else
            ver[begin].first = vmid;

        Vertex * vdeb =  new Vertex();
        vend =  new Vertex();
        vdeb->succ = vdeb->pred = vend;
        vend->succ = vend->pred = vdeb;
        vdeb->v = begin;
        vend->v = end;
  
        if (mid_is_left)
            ver[mid].first = vdeb;
        else
            ver[mid].first = vend;

        vdeb =  new Vertex();
        vmid =  new Vertex();
        vdeb->succ = vdeb->pred = vmid;
        vmid->succ = vmid->pred = vdeb;
        vdeb->v = begin;
        vmid->v = mid;
  
        if (mid_is_left)
            ver[end].first = vmid;
        else
            ver[end].first = vdeb;
    }

    //----------------------------------------------------------------
    // Given two convex hulls implicitly defined in [deb1, end1] and [deb2, end2],
    // we find the lower tangent (lower supporting line) to the two hulls.
    EDGE lower_common(/*int deb1,*/ int end1, int deb2 /*, int end2*/)
    {
        int X = end1;               // RM(Vl)
        int Y = deb2;               // LM(Vr)
  
        int Z = first(Y);
        int Zd = first(X);
        int Zdd = pred(X, Zd);

        while(1) {
            if (oriented_side(ver[X].p, ver[Y].p, ver[Z].p) != ON_POSITIVE_SIDE) {
                int oldZ = Z;
                Z = succ(Z, Y);
                Y = oldZ;
            }
            else {
                if (oriented_side(ver[X].p, ver[Y].p, ver[Zdd].p) != ON_POSITIVE_SIDE) {
                    int oldZdd = Zdd;
                    Zdd = pred(Zdd, X);
                    X = oldZdd;
                }
                else {
                    EDGE ret;
                    ret.a = X;
                    ret.b = Y;
                    return(ret);
                }
            }
        }
    }

    //----------------------------------------------------------------
    // Given two convex hulls implicitly defined in [deb1, end1] and [deb2, end2],
    // we find the upper tangent (upper supporting line) to the two hulls.
    EDGE upper_common(/*int deb1,*/ int end1, int deb2 /*, int end2*/)
    {
        int X = end1;               // RM(Vl)
        int Y = deb2;               // LM(Vr
  
        int Zd = first(Y);
        int Z = pred(Y, Zd);
        int Zdd = first(X);

        while(1) {
            if (oriented_side(ver[X].p, ver[Y].p, ver[Z].p) == ON_POSITIVE_SIDE) {
                int oldZ = Z;
                Z = pred(Z, Y);
                Y = oldZ;
            }
            else {
                if (oriented_side(ver[X].p, ver[Y].p, ver[Zdd].p) == ON_POSITIVE_SIDE) {
                    int oldZdd = Zdd;
                    Zdd = succ(Zdd, X);
                    X = oldZdd;
                }
                else {
                    EDGE ret;
                    ret.a = X;
                    ret.b = Y;
                    return(ret);
                }
            }
        }
    }

    //----------------------------------------------------------------
    // Insert vertex a in b's list while maintaining the circular adjacency.
    void minsert(int a, int b)
    {
        Vertex *k, *cr, *j, *start;
  
        k = ver[a].first;
        cr = new Vertex();
        cr->v = b;
        if (k->succ->v == k->v) {
            // (void) printf("BRANCH 1\n");
            cr->succ = k;
            cr->pred = k;
            k->succ = cr;
            k->pred = cr;  
            if (oriented_side(ver[a].p, ver[k->v].p, ver[b].p) != ON_POSITIVE_SIDE)
                ver[a].first = cr;
        }
        else {
            if (oriented_side(ver[a].p, ver[k->v].p, ver[b].p) != ON_POSITIVE_SIDE &&
                oriented_side(ver[a].p, ver[k->pred->v].p, ver[b].p) != ON_POSITIVE_SIDE &&
                oriented_side(ver[a].p, ver[k->pred->v].p, ver[k->v].p) != ON_POSITIVE_SIDE ) {
                // (void) printf("BRANCH 2\n");
                cr->succ = k;
                cr->pred = k->pred;
                k->pred->succ = cr;
                k->pred = cr;
                ver[a].first = cr;
            }
            else {
                // (void) printf("BRANCH 3\n");
                start = k;
                j = k->succ;
                while (!(oriented_side(ver[a].p, ver[k->v].p, ver[b].p) == ON_POSITIVE_SIDE &&
                         oriented_side( ver[a].p, ver[b].p, ver[j->v].p) == ON_POSITIVE_SIDE)) {
                    if (j == start) break;
                    k = j;
                    j = j->succ;
                }
                if (j->v != b && k->v != b) {
                    cr->succ = j;
                    cr->pred = k;
                    j->pred = cr;
                    k->succ = cr;
                }
                else
                    delete cr;		// already inserted
            }
        }
    }

    // Insert vertex a in b's list while maintaining the circular adjacency.
    // Also insert b in a's list.
    void INSERT(int a, int b)
    {
        minsert(a, b);
        minsert(b, a);
    }

    //----------------------------------------------------------------
    // Delete vertex a from b's list.
    void mdelete(int a, int b)
    {
        Vertex *k = ver[a].first;
        if (k->v == b) {		// first node is deleted
            k->pred->succ = k->succ;
            k->succ->pred = k->pred;
            ver[a].first = k->succ;
            delete k;
        }
        else {
            while (k->v != b)
                k = k->succ;
            k->pred->succ = k->succ;
            k->succ->pred = k->pred;
            delete k;
        } 
    }

    void myDELETE(int a, int b)
    // Delete vertex a from b's list and b from a's list
    {
        mdelete(a, b);
        mdelete(b, a);
    }

    //----------------------------------------------------------------
    // Merge the two hulls (deb1,end1) to (deb2,end2), (indices in
    // points' list). This algorithm is due to Lee and Schachter.
    void merge(/*int deb1,*/ int end1, int deb2 /*, int end2*/)
    {
        int R1, R2, L1, L2;
        int  a, b;

        EDGE BT = lower_common(/*deb1,*/ end1, deb2 /*, end2*/);
        EDGE UT = upper_common(/*deb1,*/ end1, deb2 /*, end2*/);
        int L  = BT.a;                // left_endpoint of BT
        int R  = BT.b;                // right_endpoint of BT
  
        while ((BT.a != UT.a) || (BT.b != UT.b)) {
            a = b = 0;
            INSERT(L, R);
            R1 = pred(R, L);
            if (oriented_side(ver[L].p, ver[R].p, ver[R1].p) == ON_POSITIVE_SIDE) {
                R2 = pred(R, R1);
                while (R2 >= deb2 &&
                       inside_circle( /*p:*/ver[R2].p,  ver[R1].p, ver[L].p, ver[R].p) == INSIDE_CIRCLE) {
                    myDELETE(R, R1);
                    R1 = R2;
                    R2 = pred(R, R1);
                }
            }
            else
                a = 1;
            L1 = succ(L, R);
            if (oriented_side(ver[R].p, ver[L].p, ver[L1].p) != ON_POSITIVE_SIDE) {
                L2 = succ(L, L1);
                while (L2 <= end1 &&
                       inside_circle(/*p:*/ ver[L2].p, ver[L].p, ver[R].p, ver[L1].p) == INSIDE_CIRCLE) {
                    myDELETE(L, L1);
                    L1 = L2;
                    L2 = succ(L, L1);
                }
            }
            else 
                b = 1;
            if (a) 
                L = L1;
            else if (b)
                R = R1;
            else if (inside_circle(/*p:*/ ver[L1].p, ver[L].p, ver[R].p, ver[R1].p) == OUTSIDE_CIRCLE)
                R = R1;
            else
                L = L1;
            BT.a = L; BT.b = R;
        }
        INSERT(BT.a, BT.b);
    }

    //----------------------------------------------------------------
    // This function is called by the constructor after the points are read.
    void delaunay(int begin, int end)
    {
        int mid;

        if (end - begin == 1)
            simple2(begin, end);
        else if (end - begin == 2)
            simple3(begin, end);
        else {
            // Divide and Conquer
            mid = (begin + end) / 2;
            delaunay(begin, mid);
            delaunay(mid + 1, end);
            merge(/*begin,*/ mid, mid + 1 /*, end*/);
        }
    }

public:

    //----------------------------------------------------------------
    // These convenience functions improve the code readability.
    //
    // first(x) returns the first vertex around x 
    // (see struct Vertex and struct Delaunay_point above).
    int first(int x) const
    {
        Vertex *a = ver[x].first;
        if (a)
            return(a->v);
        else
            return(NO_EDGE);
    }

    // pred(x,y) returns the vertex preceding y in x's adjacency list.
    int pred(int x, int y) const
    {
        Vertex *a = ver[x].first;
        while (a->v != y)
            a = a->pred;
        return(a->pred->v);
    }

    // pred(x,y) returns the vertex succeeding y in x's adjacency list.
    int succ(int x, int y) const
    {
        Vertex *a = ver[x].first;
        while (a->v != y)
            a = a->succ;
        return(a->succ->v);
    }

private:

    //----------------------------------------------------------------
    // We insure that the input points do not contain duplicates.
    void remove_duplicates()
    {
        int i;
        for (i=0; i<num_points-1; i++)
            if (ver[i].p.x() == ver[i+1].p.x() && ver[i].p.y() == ver[i+1].p.y())
                vor_remove(i);
    }

    //----------------------------------------------------------------
    // We make it possible for the UI to have an option to delete a point.
    void vor_remove(int i)
    {
        int j;

        for (j=i; j<num_points; j++)
            ver[j] = ver[j+1];
  
        num_points--;
    }

public:

    //----------------------------------------------------------------
    Delaunay() {}

    // Sort the points lexicographically; divide the set to two
    // halves; recursively run delaunay(begin, end) on the two halves;
    // then merge the two hulls while updating the triangulation.
    Delaunay(std::vector<Point> mypoints) : ver()
    {
        num_points = 0;
        for (typename std::vector<Point>::const_iterator pit = mypoints.begin();
             pit != mypoints.end(); ++pit) {
            ver.push_back(Delaunay_point(*pit, NULL));
            num_points++;
        }

        sort_all();
        delaunay(0, num_points - 1);
    }

private:

    
    // lex_compare_vertices(a, b) determines the lexicographic
    // ordering of two points. The standard sort algorithm is used
    // based on this comparator.
    void sort_all()
    {
        std::sort( &(ver[0]), &(ver[num_points]), lex_compare_vertices );
        remove_duplicates();
    }
};

#endif // DELAUNAY_H
