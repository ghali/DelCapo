// Sherif Ghali

#ifndef ARRAY2_H
#define ARRAY2_H

// comment out the next line to avoid bounds checking
// #define NDEBUG
#include <cassert>

template<typename Color_unit> struct Color_4;
template<typename PixelType> class Image;

//----------------Array2_1_Begin----------------
template<class Type>
class Array2 {
    typedef Color_4<unsigned char> Color_4uc;
    friend class Image<Color_4uc>;
    int nrows, ncols;
    Type* table;
public:
    void* getTable() const
    { return (void*) table; }

    void init(const Type& t)
    {
        table = new Type[nrows * ncols];
        for(int i=0; i< nrows * ncols; ++i)
            table[i] = t;
    }

    Array2() : nrows(2), ncols(2)
    { init(Type()); }

    Array2(
           int nrowsin,
           int ncolsin,
           const Type& t = Type())
        : nrows(nrowsin), ncols(ncolsin)
    { init(t); }

    virtual ~Array2()
    {
        delete[] table;
    }

    Array2(const Array2& rhs)
        : nrows(rhs.nrows), ncols(rhs.ncols)
    {
        table = new Type[nrows * ncols];
        for(int i=0; i<nrows*ncols; ++i)
            table[i] = rhs.table[i];
    }
//----------------Array2_1_End----------------

//----------------Array2_2_Begin----------------
    Array2& operator=(const Array2& rhs)
    {
        if(this != &rhs) {
            delete[] table;
            nrows = rhs.nrows;
            ncols = rhs.ncols;
            table = new Type[nrows * ncols];
            for(int i=0; i<nrows*ncols; ++i)
                table[i] = rhs.table[i];
        }
        return *this;
    }

    int getNrows() const { return nrows; }
    int getNcols() const { return ncols; }
    bool withinBounds(int row, int col) const
    {
        return
            row>=0 && row<nrows &&
            col>=0 && col<ncols;
    }
    // matrix is stored in row-order
    const Type&
    operator()(int row, int col) const
    {
        assert( withinBounds(row,col) );
        return table[col + row * ncols];
    }
    Type&
    operator()(int row, int col)
    {
        assert( withinBounds(row,col) );
        return table[col + row * ncols];
    }
};
//----------------Array2_2_End----------------

#endif // ARRAY2_H
