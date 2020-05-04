// Copyright (C) 2014 University of Kaiserslautern
// Microelectronic System Design Research Group
//
// This file is part of the FPGAHS Course
// de.uni-kl.eit.course.fpgahs
//
// Matthias Jung <jungma@eit.uni-kl.de>
// Christian De Schryver <schryver@eit.uni-kl.de>

#include <stdio.h>
#include <stdlib.h>     // qsort
#include <image.h>

#define k_size 3
#define ELEMENTS k_size*k_size

int compare (const void * a, const void * b)
{
    return ( *(unsigned char*)a - *(unsigned char*)b );
}

void medianFilter(const unsigned char input[][HEIGHT], unsigned char output[][HEIGHT], unsigned int width, unsigned int height)
{
    unsigned char kernel[ELEMENTS];
    int k_index;

    // Place in a position not in the border
    for(int i=1; i<width-1; i++){
        for(int j=1; j<height-1; j++){
            // Get the values of the 3x3 kernel
            for(int ik=-1; ik<2; ik++){
                for(int jk=-1; jk<2; jk++){
                    k_index = (ik+1)*3 + jk+1;
                    kernel[k_index] = input[i+ik][j+jk];
                }
            }
            // Calculate the median
            qsort(kernel, ELEMENTS, sizeof(unsigned char), compare);
            output[i][j] = kernel[ELEMENTS/2];
        }
    }

    // Ensure borders are set to 0
    // (not needed if output matrix were to be initialized to 0)
    for(int i=0; i<width; i++){
        output[i][0] = 0;
        output[i][height-1] = 0;
    }
    for(int j=0; j<height; j++){
        output[0][j] = 0;
        output[width-1][j] = 0;
    }

}