
#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <stdio.h>
#include "lodepng.h"
#include <math.h>

using namespace std;

void kernelGrayScaleCPU(unsigned char *in, unsigned char *out, int size){
    for(int i=0;i<size;i+=4){
        unsigned char waarde = 0.2126 * in[i] + 0.7152 * in[i+1] + 0.0722 * in[i+2];
        out[i] = waarde;
        out[i+1] = waarde;
        out[i+2] = waarde;
    }
}

int main(void){

    const char* filename = "/home/wouter/CLionProjects/HelloWorld/test.png";
    const char* filenameOut = "/home/wouter/CLionProjects/HelloWorld/testOut.png";

    unsigned width, height;
    unsigned char  *out_host;

    unsigned char *image= (unsigned char*) malloc(50000);;

    //decode
    unsigned error = lodepng_decode32_file(&image, &width, &height, filename);

    //if there's an error, display it
    if(error)
        cout << "decoder error " << error << ": " << lodepng_error_text(error) << endl;
    else
        cout << "correct ingelezen" << endl;

    out_host = image;

    int length = width*height*4 ;

    clock_t begin = clock();
    kernelGrayScaleCPU(image, out_host, length);
    clock_t end = clock();

    double milliseconds_cpu_kernel = (double) 1000*(end - begin)/CLOCKS_PER_SEC;

    lodepng_encode32_file(filenameOut, out_host, width, height);

    cout << "Miliseconds op CPU: " << milliseconds_cpu_kernel << "ms "<<endl;

    free(image);
    return 0;
}
