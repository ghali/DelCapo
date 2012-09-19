// Sherif Ghali

#ifndef IMAGE_COLOR_4_H
#define IMAGE_COLOR_4_H

#include <fstream>

#include "raster_image/image.h"
#include "misc/color_4.h"

//----------------------------------------------------------------
template<typename Color_unit>
class Image<Color_4<Color_unit> >
{
    int _nrows, _ncols;
    Array2<Color_4<Color_unit> > H;
    unsigned int maxval;

public:
    int nrows() const { return _nrows; }
    int ncols() const { return _ncols; }
    const Color_4<Color_unit>& operator()(int x, int y) const
    {
        return H(x,y);
    }
    Color_4<Color_unit>& operator()(int x, int y)
    {
        return H(x,y);
    }

    unsigned int get_maxval() const { return maxval; }

    const void* getData() const { return H.table; }

    Image() : _nrows(2), _ncols(2), maxval(0),
              H( _nrows,_ncols,Color_4<Color_unit>() ) {}

    Image(int nrows, int ncols)
        : _nrows(nrows), _ncols(ncols), H(nrows, ncols), maxval(255) {}

    Image<Color_4<Color_unit> >(const char* ppmfilename)
    {
        std::ifstream IS(ppmfilename);
        if(!IS) {
            std::cerr << "Cannot open ppm file: " << ppmfilename << " for reading\n";
            exit(1);
        }
        char ppmTypechars[500];
        IS >> ppmTypechars;
        std::string ppmType(ppmTypechars);

        // P3 is ascii. P6 is binary
        if( !( ppmType == "P3" || ppmType == "P6" ) ) {
            std::cerr << "Signature of " << ppmfilename << " indicates it is not a ppm file\n";
            exit(1);
        }
        bool ppmTypeP3 = ppmType == "P3";

        // handle up to 2 lines of comments
        char tokenchars[500];
        IS >> tokenchars;
        std::string token(tokenchars);
        if(token == "#") {
            IS.ignore(1000, '\n');
            IS >> tokenchars;
            token = std::string(tokenchars);
            if(token =="#") {
                IS.ignore(1000, '\n');
                IS >> tokenchars;
                token = std::string(tokenchars);
            }
        }
        _nrows = atoi(token.c_str());
        IS >> _ncols;
    
        // check size
        if(_nrows<2 || _ncols<2) {
            std::cerr << "Image " << ppmfilename << " is too small\n";
            exit(1);
        }

        // Create array
        H = Array2<Color_4<Color_unit> >(_nrows, _ncols);

        // read maximum height
        IS >> maxval;
        char dummy;
        IS.get(dummy);          // read the end of line
        if(maxval<1 || maxval>65535) {
            std::cerr << maxval << "is illegal as Maxval for file " << ppmfilename << std::endl;
            exit(1);
        }

        // Read the file:
        //   coordinates:
        //     (0,1) (1,2) (2,2)
        //     (0,0) (1,0) (2,0)
        //   reading order:
        //       0     1     2
        //       3     4     5
        //
        //    ^j
        //    |
        //    |    . (*H)(i,j)
        //    |
        //    |          i
        //  --+---------->
        //    |
        for(int j=_ncols-1; j>=0; j--)
            for(int i=0; i<_nrows; i++) {
                if(ppmTypeP3) { // P3: ascii file
                    int r,g,b;
                    IS >> r >> g >> b;  // read into integers
                    H(i,j).set_r( (Color_unit)(r) );  // cast
                    H(i,j).set_g( (Color_unit)(g) );
                    H(i,j).set_b( (Color_unit)(b) );
                    H(i,j).set_a( (Color_unit)(0) );  // ppm files don't store an alpha channel
                }
                else {               // ppmType is P6: binary file
                    if(maxval<256) { // fits in one byte
                        unsigned char r,g,b;
                        r = IS.get();
                        g = IS.get();
                        b = IS.get();

                        H(i,j).set_r( (Color_unit)(r) );
                        H(i,j).set_g( (Color_unit)(g) );
                        H(i,j).set_b( (Color_unit)(b) );
                        H(i,j).set_a( (Color_unit)(0) );
                    }
                    else {      // fits in two bytes
                        char r1, r2, g1, g2, b1, b2;
                        IS.get(r1);
                        IS.get(r2);
                        IS.get(g1);
                        IS.get(g2);
                        IS.get(b1);
                        IS.get(b2);
                        H(i,j).set_r( (Color_unit)(int(r1) >> 8 + int(r2)) ); // The most significant byte is first
                        H(i,j).set_g( (Color_unit)(int(g1) >> 8 + int(g2)) );
                        H(i,j).set_b( (Color_unit)(int(b1) >> 8 + int(b2)) );
                        H(i,j).set_a( (Color_unit)(0) );
                    }                    
                }                    
                if(H(i,j).r() > maxval || H(i,j).g() > maxval || H(i,j).b() > maxval || H(i,j).a() > maxval) {
                    std::cerr << "Value read is larger than the max value of " << maxval << std::endl;
                    exit(1);
                }
            }
    }

    void write(const char* ppmfilename)
    {
        std::ofstream OS(ppmfilename);
        if(!OS) {
            std::cerr << "Cannot open ppm file " << ppmfilename << " for output\n";
            exit(1);
        }
        OS << "P6" << std::endl
           << "# Created by Image<Color_4<> >" << std::endl
           << _nrows << " " << _ncols << std::endl
           << maxval << std::endl;
        for(int j=_ncols-1; j>=0; j--) // write in English-reading order
            for(int i=0; i<_nrows; i++)
                if(maxval<256)
                    OS << char(H(i,j).r()) << char(H(i,j).g()) << char(H(i,j).b());
                else // "The most significant byte is first."
                    OS << char(int(H(i,j).r()) >> 8) << char(int(H(i,j).r()) && 0xFF)
                       << char(int(H(i,j).g()) >> 8) << char(int(H(i,j).g()) && 0xFF)
                       << char(int(H(i,j).b()) >> 8) << char(int(H(i,j).b()) && 0xFF);
        // Notice that there is no CR/LF at the end of each raster line.
        OS.close();
    }
};

#endif // IMAGE_COLOR_4_H
