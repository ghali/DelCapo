// Sherif Ghali

#include "raster_image/image.h"

typedef Image<Color_4<float> > ColorImage; // floats range from 0 to 1
typedef Image<char> GrayscaleImage;

std::ostream& operator<< (std::ostream& os, const Image<bool>& I)
{
    for(int row=I.nrows()-1; row>=0; --row) {
        for(int col=0; col<I.ncols(); ++col)
            os << (I(row,col) ? 'x' : '.');
        os << std::endl;
    }
    return os;
}
