
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

//Geen echte edge detection, wel iets anders cool
void kernelEdgeScaleCPU(const unsigned char *in, unsigned char *out,int width,  int size){

    int hor[3][3] = { { -1,-2,-1 }, { 0, 0, 0 }, { 1,2,1 } };

    for(int i=0;i<size;i+=4){

        for(int j=0; j<3; j++) {

            int in_matrix[3][3] = {{in[i] - width - 1 + j, in[i] - width + j, in[i] - width - 1 + j},
                                   {in[i] - 1 + j,         in[i] + j,         in[i] + 1 + j},
                                   {in[i] + width - 1 + j, in[i] + width + j, in[i] + width - 1 + j}};

            for (int c = 0; c < 3; c++) {
                for (int d = 0; d < 3; d++) {
                    for (int k = 0; k < 3; k++) {
                        out[i+j] += out[i+j] + hor[c][k] * in_matrix[k][d];
                    }
                }
            }

        }


        out[i+3]=in[i+3];
    }
}

//Contrast Enhancement
void kernelGaussianScaleCPU(const unsigned char *in, unsigned char *out, int size, int radius){

    for(int i=0;i<size;i+=4){

        int somRood = in[i-8] + in[i-4] + in[i] + in[i+4] + in[i+8];
        int somGroen = in[i-7] + in[i-3] + in[i+1] + in[i+5] + in[i+9];
        int somBlauw = in[i-6] + in[i-2] + in[i+2] + in[i+6] + in[i+10];

        out[i] = static_cast<unsigned char>(somRood / (2 * radius + 1));
        out[i+1] = static_cast<unsigned char>(somGroen / (2 * radius + 1));
        out[i+2] = static_cast<unsigned char>(somBlauw / (2 * radius + 1));

        out[i+3]=in[i+3];
    }
}


int main(){

    const char* filename = "/home/wouter/CLionProjects/HelloWorld/8k.png";
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

    clock_t begin = clock();
    kernelGaussianScaleCPU(image, out_host,length,-10);
    clock_t end = clock();

    double milliseconds_cpu_kernel = (double) 1000*(end - begin)/CLOCKS_PER_SEC;

    lodepng_encode32_file(filenameOut, out_host, width, height);

    cout << "Miliseconds op CPU: " << milliseconds_cpu_kernel << "ms "<<endl;

    free(image);
    return 0;
}
