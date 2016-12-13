// Sherif Ghali

#include <cassert>

#include "geometry_E2/bbox_e2.hpp"

#include "geometry_E2/point_e2.hpp"
#include "geometry_E2/vector_e2.hpp"
#include "geometry_E2/direction_e2.hpp"
#include "geometry_E2/segment_e2.hpp"
#include "geometry_E2/line_e2.hpp"
#include "geometry_E2/triangle_e2.hpp"
#include "geometry_E2/polygon_e2.hpp"

#include "misc/epsilon_double.hpp"
#include "geometry_E2/transformation_e2.hpp"
#include "geometry_E2/interpolation_e2.hpp"

#include "geometry_E2/predicates_e2.hpp"
#include "geometry_E2/intersection_e2.hpp"

typedef Epsilon_double MyDouble;

typedef Point_E2<MyDouble>         Point_E2d;
typedef Vector_E2<MyDouble>       Vector_E2d;
typedef Direction_E2<MyDouble> Direction_E2d;
typedef Segment_E2<MyDouble>     Segment_E2d;
typedef Line_E2<MyDouble>           Line_E2d;
typedef Triangle_E2<MyDouble>   Triangle_E2d;
typedef Polygon_E2<MyDouble>     Polygon_E2d;

typedef Transformation_E2<MyDouble> Transformation_E2d;

typedef Bbox_E2<MyDouble>   Bbox_E2d;

void test_operator_add()
{
    Point_E2d  A(3.0,4.0);
    Vector_E2d V(8.0,2.0);
    A += V;
    assert( Point_E2d(11.0, 6.0) == A );
    A -= V;
    assert( Point_E2d(3.0, 4.0) == A );
}

void test_operator_mult()
{
    const Vector_E2d V(2.0, 1.0);
    const Vector_E2d Vm(6.0, 3.0);
    const MyDouble d = 3.0;
    assert( d * V == Vm );
    assert( V * d == Vm );
}

void test_Transformation_E2_operator_mult()
{
    Point_E2d P(3,4);

    Transformation_E2d T1(2,3,4, 5,6,7);
    Transformation_E2d T2(8,9,10, 11,12,13);

    Transformation_E2d T21 = T2 * T1; // T1 is applied first

    assert(T2(T1(P)) == T21(P));

    // Test inverse
    Transformation_E2d T3 = T1.inverse();
    Transformation_E2d T4 = T1 * T3;
    assert(T4.get_m00() == 1
           && T4.get_m11() == 1);
    assert(T4.get_m01() == 0
           && T4.get_m10() == 0);
    assert(T4.get_m02() == 0
           && T4.get_m12() == 0);
}

void test_translation()
{
    Point_E2d P(5,6);

    Transformation_E2d T(TRANSLATION, Vector_E2d(2,3));
    Point_E2d Q = T(P);

    assert( (Q.x() == P.x() + 2)  &&  (Q.y() == P.y() + 3) );
}

void test_rotation()
{
    Point_E2d P1(5,7);
    Vector_E2d V1(11,13);

    Point_E2d P2 = P1 + V1;

    Transformation_E2d T(ROTATION, 33.0);

    Point_E2d P1t = T(P1);
    Vector_E2d V1t = T(V1);

    Point_E2d P2t = T(P2);

    assert( P2t == P1t + V1t );
}

void test_Bbox_E2_operator_incr()
{
    Point_E2d P1(-5,-4);
    Point_E2d P2( 5, 4);

    Bbox_E2d box(P1);
    box += P2;

    assert( box == Bbox_E2d(P1, P2) );
}        

void test_polygon_Bbox()
{
    Point_E2d P1(-5,-4);
    Point_E2d P2( 5, 4);

    Point_E2d P[] = { P1, P2, Point_E2d(-2, 2) };
    Polygon_E2d poly(P, P+3);
    Bbox_E2d box = poly.get_Bbox();
    assert( box == Bbox_E2d(P1, P2) );
}

void test_direction()
{
    Direction_E2d D(8,6);
    assert( D.x() == 0.8 && D.y() == 0.6 );
}

void test_center()
{
    Bbox_E2d scene(Point_E2d(0,0), Point_E2d(1,1)); // AR=1
    {
        Bbox_E2d window1(Point_E2d(0,0), Point_E2d(200,100)); // AR=2
        Bbox_E2d scene_in_w1 = center(scene, window1);
        assert( scene_in_w1 == Bbox_E2d(Point_E2d(-0.5,0), Point_E2d(1.5,1)) );
    }
    {
        Bbox_E2d window2(Point_E2d(0,0), Point_E2d(100,200)); // AR=0.5
        Bbox_E2d scene_in_w2 = center(scene, window2);
        assert( scene_in_w2 == Bbox_E2d(Point_E2d(0,-0.5), Point_E2d(1,1.5)) );
    }
}

void test_interpolation()
{
    std::vector<Point_E2d> myP;
    myP.push_back(Point_E2d(2,3));
    myP.push_back(Point_E2d(5,7));
    std::vector<MyDouble> myT;
    MyDouble t = 0.4;
    myT.push_back(MyDouble(1.0)-t);
    myT.push_back(t);
    assert( interpolate(myP[0],myP[1],t)
            == interpolate(&myP[0], &myT[0], 2) );

    myP.push_back(Point_E2d(4,9));
    myT.push_back(MyDouble(0.45));
    myT.push_back(MyDouble(0.35));
    myT.push_back(MyDouble(0.20));
    assert( interpolate(myP[0],myT[0], myP[1],myT[1], myP[2],myT[2])
            == interpolate(&myP[0], &myT[0], 3) );
}

void test_viewport_mapping()
{
    Point_E2d A(3,4), B(7,8);
    Point_E2d C(2,1), D(5,4);
    Bbox_E2d from(A,B);
    Bbox_E2d to(C,D);
    Transformation_E2d T = interpolate(from, to);
    assert( T(A) == C );
    assert( T(B) == D );
}

void test_Line_equality()
{
    Point_E2d P1(2,1);
    Point_E2d P2(7,4);
    Point_E2d P3(12,7);
    Point_E2d P4(17,10);
    Line_E2d L1(P1,P3);
    Line_E2d L2(P2,P4);
    Line_E2d L3(P4,P2);

    assert( L1 == L2 );        // -6 10 2
    assert( L1 != L3 );
}


const Point_E2d A(4,4);        // E--D--C
const Point_E2d B(6,4);        //    | /|
const Point_E2d C(6,6);        //    |/ |
const Point_E2d D(4,6);        //    A--B
const Point_E2d E(2,6);

const Line_E2d AB(A,B);
const Line_E2d BC(B,C);
const Line_E2d CA(C,A);

const Line_E2d AC(A,C);
const Line_E2d CD(C,D);
const Line_E2d DA(D,A);

void test_oriented_side_line_point()
{
    assert( oriented_side(AB, A) == ON_ORIENTED_BOUNDARY );
    assert( oriented_side(AB, C) == ON_POSITIVE_SIDE );
    assert( oriented_side(AB, D) == ON_POSITIVE_SIDE );

    assert( oriented_side(CA, A) == ON_ORIENTED_BOUNDARY );
    assert( oriented_side(CA, C) == ON_ORIENTED_BOUNDARY );
    assert( oriented_side(CA, B) == ON_POSITIVE_SIDE );
    assert( oriented_side(CA, D) == ON_NEGATIVE_SIDE );
}

void test_inside_circle_1()
{
    assert( inside_circle(D, A, B, C) == COCIRCULAR );
    assert( inside_circle(D, A, C, B) == COCIRCULAR );

    assert( inside_circle(C, A, B, D) == COCIRCULAR );
    assert( inside_circle(C, A, D, B) == COCIRCULAR );

    assert( inside_circle(E, A, B, C) == OUTSIDE_CIRCLE );
    assert( inside_circle(E, A, C, B) == OUTSIDE_CIRCLE );

    assert( inside_circle(C, E, A, B) == INSIDE_CIRCLE );
    assert( inside_circle(C, E, B, A) == INSIDE_CIRCLE );

    assert( inside_circle(D, E, A, B) == INSIDE_CIRCLE );
    assert( inside_circle(D, E, B, A) == INSIDE_CIRCLE );
}

void test_inside_circle_2()
{
    Point_E2d P0(4.0, 9.0);
    Point_E2d P1(3.0, 4.0);
    Point_E2d P2(3.0, 9.0);
    Point_E2d P3(1.0, 6.0);
    Point_E2d P4(1.0, 4.0);

    assert( inside_circle(P0, P1, P2, P4) == OUTSIDE_CIRCLE );
    assert( inside_circle(P0, P1, P4, P2) == OUTSIDE_CIRCLE );
    assert( inside_circle(P3, P1, P2, P4) == INSIDE_CIRCLE );
    assert( inside_circle(P3, P1, P4, P2) == INSIDE_CIRCLE );
}

void test_intersection_line_line()
{
    Point_E2d Ai = intersection(CA, AB);
    Point_E2d Bi = intersection(AB, BC);
    Point_E2d Ci = intersection(BC, CA);

    assert( A == Ai );
    assert( B == Bi );
    assert( C == Ci );
}

const Segment_E2d sAB(A,B);
const Segment_E2d sBC(B,C);
const Segment_E2d sCA(C,A);

const Segment_E2d sAC(A,C);
const Segment_E2d sCD(C,D);
const Segment_E2d sDA(D,A);

void test_oriented_side_segment_point()
{
    assert( oriented_side(sAB, A) == ON_ORIENTED_BOUNDARY );
    assert( oriented_side(sAB, C) == ON_POSITIVE_SIDE );
    assert( oriented_side(sAB, D) == ON_POSITIVE_SIDE );

    assert( oriented_side(sCA, A) == ON_ORIENTED_BOUNDARY );
    assert( oriented_side(sCA, C) == ON_ORIENTED_BOUNDARY );
    assert( oriented_side(sCA, B) == ON_POSITIVE_SIDE );
    assert( oriented_side(sCA, D) == ON_NEGATIVE_SIDE );
}

void test_intersection_of_lines_segment_segment()
{
    Point_E2d Ai = intersection_of_lines(sCA, sAB);
    Point_E2d Bi = intersection_of_lines(sAB, sBC);
    Point_E2d Ci = intersection_of_lines(sBC, sCA);

    assert( A == Ai );
    assert( B == Bi );
    assert( C == Ci );
}

void test_Triangle_E2_get_signed_area()
{
    Triangle_E2d T1(A,B,C);
    Triangle_E2d T2(A,C,B);
    Triangle_E2d T3(A,C,D);

    // | 4 4 1 | 
    // | 6 4 1 | = (36-24) - (0) + (16-24) = 52 - 48 = 4
    // | 6 6 1 | 

    assert( T1.get_signed_area() ==  2 );
    assert( T2.get_signed_area() == -2 );
    assert( T3.get_signed_area() ==  2 );
}

void test_segment_parallelism()
{
    assert( are_parallel(sAB, sCD) );
    assert( are_parallel(sBC, sDA) );
}

void test_dominant()
{
    assert( dominant(sAB) == E2_POSX );
    assert( dominant(sBC) == E2_POSY );
    assert( dominant(sCD) == E2_NEGX );
    assert( dominant(sDA) == E2_NEGY );
}

void test_projected_difference()
{
    const Point_E2d P1(2,1), P2(4,2);
    const Point_E2d P3(2,0), P4(4,3);
    const Point_E2d P5(0,1), P6(-2,-2);
    const Segment_E2d S1(P1,P2), S2(P3,P4), S3(P5,P6);

    MyDouble d1 = find_larger_projected_difference(S1, S2);
    MyDouble d2 = find_larger_projected_difference(S1, S3);

    assert( d1 == 1 );
    assert( d2 == -3 );
}

void test_line_convex_clip()
{
    const Point_E2d P1(6,1), P2(7,3);
    const Point_E2d P3(8,3), P4(7,5);
    const Point_E2d P5(8,6), P6(4,4);
    const Point_E2d P7(4,5), P8(3,2);
    const Point_E2d P9(2,1), P10(6,2);
    const Segment_E2d S0(P1,P2), S1(P3,P4), S2(P5,P6);
    const Segment_E2d S3(P7,P8), S4(P9,P10);

    {
        const Segment_E2d segs[] = {S0,S1,S2,S3,S4};
        const std::vector<Segment_E2d> region(&(segs[0]), &(segs[5]));
        {
            // clipped at both ends
            const Point_E2d A(2,2), B(9,4);
            {
                const Segment_E2d AB(A,B);
                std::pair<bool,Segment_E2d> segment = clip_line_to_convex_region(AB, region);
                assert( segment.first );

                const Point_E2d s = intersection_of_lines(AB, S3);
                const Point_E2d t = intersection_of_lines(AB, S0);

                assert( segment.second.source() == s );
                assert( segment.second.target() == t );
            }
            {
                const Segment_E2d BA(B,A);
                std::pair<bool,Segment_E2d> segment = clip_line_to_convex_region(BA, region);
                assert( segment.first );

                const Point_E2d s = intersection_of_lines(BA, S0);
                const Point_E2d t = intersection_of_lines(BA, S3);

                assert( segment.second.source() == s );
                assert( segment.second.target() == t );
            }
        }
        {
            // coincidence
            const Point_E2d C(P9), D(P10);
            {
                const Segment_E2d CD(C, D);
                std::pair<bool,Segment_E2d> segment = clip_line_to_convex_region(CD, region);
                assert( segment.first );

                const Point_E2d s = intersection_of_lines(CD, S3);
                const Point_E2d t = intersection_of_lines(CD, S0);

                assert( segment.second.source() == s );
                assert( segment.second.target() == t );
            }
            {
                const Segment_E2d DC(D, C);
                std::pair<bool,Segment_E2d> segment = clip_line_to_convex_region(DC, region);
                assert( segment.first );

                const Point_E2d s = intersection_of_lines(DC, S0);
                const Point_E2d t = intersection_of_lines(DC, S3);

                assert( segment.second.source() == s );
                assert( segment.second.target() == t );
            }
        }
    }
    {
        // clipped out
        const Point_E2d A(2,1), B(9,3);
        const Segment_E2d carrying_line(A,B);

        const Point_E2d P11(2,2), P12(9,4);
        const Segment_E2d S0n(P11,P12);

        const Segment_E2d segs[] = {S0n,S1,S2,S3};
        const std::vector<Segment_E2d> region(&(segs[0]), &(segs[4]));

        std::pair<bool,Segment_E2d> segment = clip_line_to_convex_region(carrying_line, region);
        assert( !segment.first );
    }
}

void test_segment_convex_clip()
{
    const Point_E2d P1(6,1), P2(7,3);
    const Point_E2d P3(8,3), P4(7,5);
    const Point_E2d P5(8,6), P6(4,4);
    const Point_E2d P7(4,5), P8(3,2);
    const Point_E2d P9(2,1), P10(6,2);
    const Segment_E2d S0(P1,P2), S1(P3,P4), S2(P5,P6);
    const Segment_E2d S3(P7,P8), S4(P9,P10);

    {
        const Segment_E2d segs[] = {S0,S1,S2,S3,S4};
        const std::vector<Segment_E2d> region(&(segs[0]), &(segs[5]));
        {
            // clipped at both ends
            const Point_E2d A(5,3), B(9,4);
            {
                const Segment_E2d AB(A,B);
                std::pair<bool,Segment_E2d> segment = clip_segment_to_ccw_convex_region(AB, region);
                assert( segment.first );

                const Point_E2d s = A;
                const Point_E2d t = intersection_of_lines(AB, S0);

                assert( segment.second.source() == s );
                assert( segment.second.target() == t );
            }
        }
        {
            // coincidence
            const Point_E2d C(P9), D(P10);
            {
                const Segment_E2d CD(C, D);
                std::pair<bool,Segment_E2d> segment = clip_segment_to_ccw_convex_region(CD, region);
                assert( segment.first );

                const Point_E2d s = intersection_of_lines(CD, S3);
                const Point_E2d t = P10;

                assert( segment.second.source() == s );
                assert( segment.second.target() == t );
            }
            {
                const Segment_E2d DC(D, C);
                std::pair<bool,Segment_E2d> segment = clip_segment_to_ccw_convex_region(DC, region);
                assert( segment.first );

                const Point_E2d s = P10;
                const Point_E2d t = intersection_of_lines(DC, S3);

                assert( segment.second.source() == s );
                assert( segment.second.target() == t );
            }
        }
    }
    {
        const Point_E2d P0(-6,-1), P1(-6,6);
        const Point_E2d P2(0,2), P3(4,2);
        const Point_E2d P4(5,6), P5(5,-1);
        const Point_E2d P6(5,-1), P7(-6,-1);
        const Segment_E2d S0(P0,P1), S1(P2,P3);
        const Segment_E2d S2(P4,P5), S3(P6,P7);
        {
            const Segment_E2d segs[] = {S0,S1,S2,S3};
            const std::vector<Segment_E2d> region(&(segs[0]), &(segs[4]));
            {
                // coincidence
                const Point_E2d A(4,2), B(0,2);
                const Segment_E2d AB(A,B);

                std::pair<bool,Segment_E2d> segment = clip_segment_to_cw_convex_region(AB, region);
                assert( segment.first );

                assert( segment.second.source() == A );
                assert( segment.second.target() == B );
            }
        }
    }
}

int main()
{
    test_operator_add();
    test_operator_mult();

    test_Transformation_E2_operator_mult();
    test_translation();
    test_rotation();
    test_Bbox_E2_operator_incr();
    test_polygon_Bbox();
    test_direction();
    test_center();

    // Interpolation
    test_interpolation();
    test_viewport_mapping();

    // Predicates
    test_Line_equality();
    test_oriented_side_line_point();
    test_inside_circle_1();
    test_inside_circle_2();
    test_intersection_line_line();
    test_oriented_side_segment_point();
    test_intersection_of_lines_segment_segment();
    test_Triangle_E2_get_signed_area();
    test_segment_parallelism();
    test_projected_difference();
    test_line_convex_clip();
    test_segment_convex_clip();
}
