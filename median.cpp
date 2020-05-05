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

#define element_swap(a,b){unsigned char t=a;a=b;b=t; }

int compare (const void * a, const void * b){
    return ( *(unsigned char*)a - *(unsigned char*)b );
}

unsigned char median_calc (const unsigned char kernel[]){
    /*
    // Using quick sort
    qsort(kernel, ELEMENTS, sizeof(unsigned char), compare);
    return kernel[ELEMENTS/2];
     */

    // Using Quick select
    int low, median, high, ll, middle, hh;

    // Copy array -- Improvement: look for a better way to do it
    unsigned char k[ELEMENTS];
    for (int i=0; i<ELEMENTS; i++){
        k[i] = kernel[i];
    }

    low = 0;
    high = ELEMENTS-1;
    median = (low + high) / 2;

    for (;;){
        if (high <= low) return k[median];  // One element only
        if (high == low +1){
            // Two elements only
            if (k[low] > k[high]) element_swap(k[low],k[high]);
            return k[median];
        }
        // Find median of low, middle and high items; swap into position low
        middle = (low + high) / 2;
        if (k[middle] > k[high]) element_swap(k[middle],k[high])
        if (k[low] > k[high]) element_swap(k[low],k[high])
        if (k[middle] > k[low]) element_swap(k[middle],k[low])
        // Swap low item (now in position middle) into position (low+1)
        element_swap(k[middle],k[low+1])
        // Nibble from each end towards middle, swapping items when stuck
        ll = low + 1;
        hh = high;
        for (;;){
            do ll++;
            while (k[low] > k[ll]);
            do hh--;
            while (k[hh] > k[low]);
            if (hh < ll) break;
            element_swap(k[ll],k[hh])
        }
        // Swap middle item (in position low) back into correct position
        element_swap(k[low],k[hh])
        // Re-set active partition
        if (hh <= median) low = ll;
        if (hh >= median) high = hh -1;
    }
}

void medianFilter(const unsigned char input[][HEIGHT], unsigned char output[][HEIGHT], unsigned int width, unsigned int height)
{
    unsigned char kernel[ELEMENTS];
    int k_index;

    unsigned char aux;

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
            output[i][j] = median_calc(kernel);
            aux = output[i][j];
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