
#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <stdio.h>
#include "lodepng.h"
#include <math.h>

using namespace std;

void kernelGrayScaleCPU(const unsigned char *in, unsigned char *out, int size){
    for(int i=0;i<size;i+=4){
        auto waarde = static_cast<unsigned char>(0.2126 * in[i] + 0.7152 * in[i + 1] + 0.0722 * in[i + 2]);
        out[i] = waarde;
        out[i+1] = waarde;
        out[i+2] = waarde;
    }
}

void kernelSepiaScaleCPU(const unsigned char *in, unsigned char *out, int size){
    for(int i=0;i<size;i+=4){

        const char tr = static_cast<const char>(in[i] * 0.393 + in[i + 1] * 0.769 + in[i + 2] * 0.189);
        const char tg = static_cast<const char>(in[i] * 0.349 + in[i + 1] * 0.686 + in[i + 2] * 0.168);
        const char tb = static_cast<const char>(in[i] * 0.272 + in[i + 1] * 0.534 + in[i + 2] * 0.131);

        if(tr>255)
            out[i] = 255;
        else
            out[i] = (unsigned char) tr;

        if(tg>255)
            out[i + 1] = 255;
        else
            out[i + 1] = (unsigned char) tg;

        if(tb>255)
            out[i + 2] = 255;
        else
            out[i + 2] = (unsigned char) tb;

        out[i + 3] = in[i + 3];
    }
}

int main(){

    const char* filename = "/home/wouter/CLionProjects/HelloWorld/test.png";
    const char* filenameOut = "/home/wouter/CLionProjects/HelloWorld/testOut.png";

    unsigned width, height;
    unsigned char  *out_host;

    auto *image= (unsigned char*) malloc(50000);;

    //decode
    unsigned error = lodepng_decode32_file(&image, &width, &height, filename);

    //if there's an error, display it
    if(error)
        cout << "decoder error " << error << ": " << lodepng_error_text(error) << endl;
    else
        cout << "correct ingelezen" << endl;

    out_host = image;

    int length = width*height*4 ;

    unsigned char  *red_in;
    unsigned char  *green_in;
    unsigned char  *blue_in;

    unsigned char  *red_out;
    unsigned char  *green_out;
    unsigned char  *blue_out;


    clock_t begin = clock();
    kernelSepiaScaleCPU(image, out_host,length,-10);
    clock_t end = clock();

    double milliseconds_cpu_kernel = (double) 1000*(end - begin)/CLOCKS_PER_SEC;

    lodepng_encode32_file(filenameOut, out_host, width, height);

    cout << "Miliseconds op CPU: " << milliseconds_cpu_kernel << "ms "<<endl;

    free(image);
    return 0;
}
