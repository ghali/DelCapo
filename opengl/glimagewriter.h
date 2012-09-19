// Sherif Ghali

#ifndef GLIMAGEWRITER_H
#define GLIMAGEWRITER_H

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "../misc/conversions.h"

class GLimageWriter
{
    int currentImageNumber;
public:
    GLimageWriter() : currentImageNumber(0) {}

    void
    writeImage(const std::string& filename, int viewportWidth, int viewportHeight)
    {
        std::string currentImageString = integer_to_zero_padded_string(currentImageNumber);
        std::string ppmfilename = filename + currentImageString + ".ppm";

        FILE* outfile = fopen( ppmfilename.c_str(), "w" );

        if ( !outfile ) {
            fprintf( stderr, "Unable to open file '%s'\n", ppmfilename.c_str() );
            return;
        }

        fprintf(outfile,"P6\n%d %d\n255\n", viewportWidth, viewportHeight);

        int n = 3 * viewportWidth * viewportHeight;

        {
            GLubyte* pixels = (GLubyte *) malloc( n * sizeof( GLubyte ));
            if ( !pixels ) {
                fprintf( stderr, "Unable to malloc() bytes for pixels\n" );
                return;
            }

            glReadPixels(0,0, viewportWidth, viewportHeight,
                         GL_RGB, GL_UNSIGNED_BYTE, pixels);

            // We output from top to bottom as that's what ppm dictates
            // (but the first row for glReadPixels is the bottom one.)
            int oneRow = 3 * viewportWidth * sizeof(GLubyte);
            for(GLubyte* topToBottom = pixels + n - oneRow; topToBottom >= pixels; topToBottom -= oneRow)
                fwrite( topToBottom, sizeof( GLubyte ), oneRow, outfile );

            free(pixels);
        }
        fclose( outfile );

        // The following lines assume ImageMagick is installed.
        // compress with: 'convert someimage.ppm someimage.png'
        const std::string pngfilename = filename + currentImageString + ".png";

        // convert
        std::string convstr = "convert " + ppmfilename + " " + pngfilename;
        std::cout << convstr << std::endl;
//        if( !std::system(convstr.c_str()) ) {
//            std::string deletestr = "rm -f " + ppmfilename;
//            std::cout << deletestr << std::endl;
//            std::system(deletestr.c_str());
//        }

        currentImageNumber++;
    }
};

#endif // GLIMAGEWRITER_H
