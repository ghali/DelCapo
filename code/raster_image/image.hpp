// Sherif Ghali

#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include "raster_image/array2.hpp"
#include "misc/color_4.hpp"

template<typename PixelType>
class Image
{
protected:
    int _nrows, _ncols;
    Array2<PixelType> H;
public:
    Image() : _nrows(2), _ncols(2), H(_nrows,_ncols,PixelType()) {}
    Image(int nrows, int ncols)
        : _nrows(nrows), _ncols(ncols), H(nrows, ncols) {}

    int nrows() const { return _nrows; }
    int ncols() const { return _ncols; }
    const PixelType& operator()(int row, int col) const
    {
        return H(row,col);
    }
    PixelType& operator()(int row, int col)
    {
        return H(row,col);
    }
};

std::ostream& operator<< (std::ostream& os, const Image<bool>& I);

#endif //IMAGE_H
