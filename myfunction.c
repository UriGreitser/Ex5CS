//  20928989    Uri Greitser
#include <stdbool.h>
#include "readBMP.h"
#include "writeBMP.h"

typedef struct {
     short red;
     short green;
     short blue;
} pixel;

void charsToPixels(Image *charsImg, pixel* pixels) {

	int row, col,x,y,z,a;
	for (row = 0 ; row < m ; row++) {  // change
        a = row*n;
        y = 3*a;
        for (col = 0 ; col < n ; col++) {
             x = a+ col;
             z = 3*col;
			pixels[x].red =(unsigned char) image->data[y + z];
			pixels[x].green =(unsigned char ) image->data[y + z + 1];
			pixels[x].blue = (unsigned char)image->data[y + z + 2];
		}
	}
}

void pixelsToChars(pixel* pixels, Image *charsImg) {

	int row, col;
    int x,y,z,a;
	for (row = 0 ; row < m ; row++) {
        a = row*n;
        x = 3*row*n;
        for (col = 0 ; col < n ; col++) {
             y = 3*col;
             z = a + col;
			image->data[x + y] = (unsigned char)pixels[z].red;
			image->data[x + y + 1] = (unsigned char)pixels[z].green;
			image->data[x + y + 2] = (unsigned char) pixels[z].blue;
		}
	}
}


void doConvolutionBlur(pixel* src, pixel* dst) {
    int i = 0;
    for ( i = 0; i < n; i++) {
        dst[i] = src[i];
    }
    int start = n*n;
    int limit = start-n;
    for (i = start; i >limit;i-- ) {
        dst[i] = src[i];
    }
    dst[n] = src[n];
    // copies the side frames
    for (i = (n*2)-1;i<limit;i += n) {
        dst[i] = src[i];
        dst[i+1] = src[i+1];
    }
    i = 0;
    int j;
    int tempIMinus1N;
    int tempIPlus1N;
    int tempI1N;
    int temp;
    	for (i = 1 ; i < n - 1; i++) {
             tempIMinus1N = (i-1)*n;
             tempIPlus1N = (i+1)*n;
             tempI1N = (i)*n;
            for (j =  1 ; j < n - 1 ; j++) {
                 temp = tempI1N + j;
                dst[temp].green = (src[tempIMinus1N + j - 1].green + src[tempIMinus1N + j].green + src[tempIMinus1N + j + 1].green
                         + src[tempI1N + j - 1].green + src[tempI1N + j].green + src[tempI1N + j + 1].green +
                         src[tempIPlus1N + j - 1].green + src[tempIPlus1N + j].green + src[tempIPlus1N + j + 1].green) / 9;

                dst[temp].blue = (src[tempIMinus1N + j - 1].blue + src[tempIMinus1N + j].blue + src[tempIMinus1N + j + 1].blue
                         + src[tempI1N + j - 1].blue + src[tempI1N + j].blue + src[tempI1N + j + 1].blue +
                         src[tempIPlus1N + j - 1].blue + src[tempIPlus1N + j].blue + src[tempIPlus1N + j + 1].blue) / 9;

                dst[temp].red = (src[tempIMinus1N + j - 1].red + src[tempIMinus1N + j].red + src[tempIMinus1N + j + 1].red
                                 + src[tempI1N + j - 1].red + src[tempI1N + j].red + src[tempI1N + j + 1].red +
                                 src[tempIPlus1N + j - 1].red + src[tempIPlus1N + j].red + src[tempIPlus1N + j + 1].red) /9;
            }
        }
}
void doConvolutionSharpen(pixel* src, pixel* dst) {
    int i, j;
    int tempIMinus1N;
    int tempIPlus1N;
    int tempI1N;
    int temp;
    for (i = 1; i < n - 1; i++) {
         tempIMinus1N = (i - 1) * n;
         tempIPlus1N = (i + 1) * n;
         tempI1N = (i) * n;
        for (j = 1; j < n - 1; j++) {
             temp = tempI1N + j;
            dst[temp].green = (-src[tempIMinus1N + j - 1].green - src[tempIMinus1N + j].green - src[tempIMinus1N + j +1].green
                               - src[tempI1N + j - 1].green + 9* src[tempI1N + j].green - src[tempI1N+ j +1].green -
                               src[tempIPlus1N + j - 1].green - src[tempIPlus1N + j].green - src[tempIPlus1N + j +1].green);
            if (dst[temp].green > 255)
                dst[temp].green = 255;
            else if (dst[temp].green < 0)
                dst[temp].green = 0;

            dst[temp].blue = (-src[tempIMinus1N + j - 1].blue - src[tempIMinus1N + j].blue - src[tempIMinus1N + j +1].blue
                              - src[tempI1N + j - 1].blue + 9* src[tempI1N + j].blue - src[tempI1N + j +1].blue -
                              src[tempIPlus1N + j - 1].blue - src[tempIPlus1N + j].blue - src[tempIPlus1N+ j +1].blue);
            if (dst[temp].blue > 255)
                dst[temp].blue = 255;
            else if (dst[temp].blue < 0)
                dst[temp].blue = 0;


            dst[temp].red = (-src[tempIMinus1N + j - 1].red - src[tempIMinus1N + j].red - src[tempIMinus1N + j +1].red
                             - src[tempI1N + j - 1].red + 9* src[tempI1N + j].red - src[tempI1N+ j +1].red -
                             src[tempIPlus1N + j - 1].red - src[tempIPlus1N + j].red - src[tempIPlus1N + j +1].red);
            if (dst[temp].red > 255)
                dst[temp].red = 255;
            else if (dst[temp].red < 0)
                dst[temp].red = 0;
        }
    }
}
void doConvolutionBlurFilter(pixel* src, pixel* dst) {
    int i ;
    for (i = 0; i < n; i++) {
        dst[i] = src[i];
    }
    int start = n * n;
    int limit = start - n;
    for (i = start; i > limit; i--) {
        dst[i] = src[i];
    }
    dst[n] = src[n];
    // copies the side frames
    for (i = (n * 2) - 1; i < limit; i += n) {
        dst[i] = src[i];
        dst[i + 1] = src[i + 1];
    }
    i = 0;
    int j;
    int maxSum = 0, minSum = 0;
    int sumTemp = 0;
    int tempIMinus1N;
    int tempIPlus1N;
    int tempI1N;
    int temp;
    unsigned int tempGreenMax;
    unsigned int tempBlueMax;
    unsigned int tempRedMax;
    unsigned int tempGreenMin;
    unsigned int tempBlueMin ;
    unsigned int tempRedMin ;
    for (i = 1; i < n - 1; i++) {
         tempIMinus1N = (i - 1) * n;
         tempIPlus1N = (i + 1) * n;
         tempI1N = (i) * n;
        for (j = 1; j < n - 1; j++) {
             temp = tempI1N + j;
             tempGreenMax = src[tempIMinus1N + j - 1].green;
             tempBlueMax = src[tempIMinus1N + j - 1].blue;
             tempRedMax = src[tempIMinus1N + j - 1].red;
             tempGreenMin = src[tempIMinus1N + j - 1].green;
             tempBlueMin = src[tempIMinus1N + j - 1].blue;
             tempRedMin = src[tempIMinus1N + j - 1].red;
            // finds min and max
            maxSum = tempGreenMax + tempRedMax +tempBlueMax;
            minSum = maxSum;
            //number 2/9
            sumTemp = src[tempIMinus1N + j ].green + src[tempIMinus1N + j ].red + src[tempIMinus1N + j].blue;
            if (sumTemp > maxSum) {
                maxSum = sumTemp;
                tempGreenMax = src[tempIMinus1N + j ].green;
                tempBlueMax = src[tempIMinus1N + j ].blue;
                tempRedMax = src[tempIMinus1N + j ].red;
            }
            else if (sumTemp <= minSum) {
                minSum = sumTemp;
                tempGreenMin = src[tempIMinus1N + j ].green;
                tempBlueMin = src[tempIMinus1N + j ].blue;
                tempRedMin = src[tempIMinus1N + j ].red;
            }
            //number 3/9
            sumTemp = src[tempIMinus1N + j+1].green + src[tempIMinus1N + j+1].red + src[tempIMinus1N + j+1].blue;
            if (sumTemp > maxSum) {
                maxSum = sumTemp;
                tempGreenMax = src[tempIMinus1N + j + 1].green;
                tempBlueMax = src[tempIMinus1N + j + 1].blue;
                tempRedMax = src[tempIMinus1N + j + 1].red;
            }
            else if (sumTemp <= minSum) {
                minSum = sumTemp;
                tempGreenMin = src[tempIMinus1N + j + 1].green;
                tempBlueMin = src[tempIMinus1N + j + 1].blue;
                tempRedMin = src[tempIMinus1N + j + 1].red;
            }
            //number 4/9
            sumTemp = src[tempI1N + j-1].green + src[tempI1N + j-1].red + src[tempI1N + j-1].blue;
            if (sumTemp > maxSum) {
                maxSum = sumTemp;
                tempGreenMax = src[tempI1N + j - 1].green;
                tempBlueMax = src[tempI1N + j - 1].blue;
                tempRedMax = src[tempI1N + j - 1].red;
            }
            else if (sumTemp <= minSum) {
                minSum = sumTemp;
                tempGreenMin = src[tempI1N + j - 1].green;
                tempBlueMin = src[tempI1N + j - 1].blue;
                tempRedMin = src[tempI1N + j - 1].red;
            }
            //number 5/9
            sumTemp = src[tempI1N + j].green + src[tempI1N + j].red + src[tempI1N + j].blue;
            if (sumTemp > maxSum) {
                maxSum = sumTemp;
                tempGreenMax = src[tempI1N + j].green;
                tempBlueMax = src[tempI1N + j ].blue;
                tempRedMax = src[tempI1N + j ].red;
            }
            else if (sumTemp <= minSum) {
                minSum = sumTemp;
                tempGreenMin = src[tempI1N + j ].green;
                tempBlueMin = src[tempI1N + j ].blue;
                tempRedMin = src[tempI1N + j ].red;
            }
            //number 6/9
            sumTemp = src[tempI1N + j+1].green + src[tempI1N + j+1].red + src[tempI1N + j+1].blue;
            if (sumTemp > maxSum) {
                maxSum = sumTemp;
                tempGreenMax = src[tempI1N + j +1].green;
                tempBlueMax = src[tempI1N + j +1].blue;
                tempRedMax = src[tempI1N + j +1].red;
            }
            else if (sumTemp <= minSum) {
                minSum = sumTemp;
                tempGreenMin = src[tempI1N + j +1].green;
                tempBlueMin = src[tempI1N + j +1].blue;
                tempRedMin = src[tempI1N + j +1].red;
            }
            //number 7/9
            sumTemp = src[tempIPlus1N + j-1].green + src[tempIPlus1N + j-1].red + src[tempIPlus1N + j-1].blue;
            if (sumTemp > maxSum) {
                maxSum = sumTemp;
                tempGreenMax = src[tempIPlus1N + j-1 ].green;
                tempBlueMax = src[tempIPlus1N + j -1].blue;
                tempRedMax = src[tempIPlus1N + j -1].red;
            }
            else if (sumTemp <= minSum) {
                minSum = sumTemp;
                tempGreenMin = src[tempIPlus1N + j -1].green;
                tempBlueMin = src[tempIPlus1N + j-1 ].blue;
                tempRedMin = src[tempIPlus1N + j -1].red;
            }
            //number 8/9
            sumTemp = src[tempIPlus1N + j].green + src[tempIPlus1N + j].red + src[tempIPlus1N + j].blue;
            if (sumTemp > maxSum) {
                maxSum = sumTemp;
                tempGreenMax = src[tempIPlus1N + j ].green;
                tempBlueMax = src[tempIPlus1N + j ].blue;
                tempRedMax = src[tempIPlus1N + j ].red;
            }
            else if (sumTemp <= minSum) {
                minSum = sumTemp;
                tempGreenMin = src[tempIPlus1N + j ].green;
                tempBlueMin = src[tempIPlus1N + j ].blue;
                tempRedMin = src[tempIPlus1N + j ].red;
            }
            //number 9/9
            sumTemp = src[tempIPlus1N + j+1].green + src[tempIPlus1N + j+1].red + src[tempIPlus1N + j+1].blue;
            if (sumTemp > maxSum) {
                maxSum = sumTemp;
                tempGreenMax = src[tempIPlus1N + j +1].green;
                tempBlueMax = src[tempIPlus1N + j +1].blue;
                tempRedMax = src[tempIPlus1N + j +1].red;
            }
            else if (sumTemp <= minSum) {
                minSum = sumTemp;
                tempGreenMin = src[tempIPlus1N + j +1].green;
                tempBlueMin = src[tempIPlus1N + j +1].blue;
                tempRedMin = src[tempIPlus1N + j +1].red;
            }

            dst[temp].green =
                    ( src[tempIMinus1N + j - 1].green + src[tempIMinus1N + j].green + src[tempIMinus1N + j + 1].green
                      + src[tempI1N + j - 1].green + src[tempI1N + j].green + src[tempI1N + j + 1].green +
                      src[tempIPlus1N + j - 1].green + src[tempIPlus1N + j].green + src[tempIPlus1N + j + 1].green -tempGreenMin - tempGreenMax ) / 7;

            dst[temp].blue =
                    (src[tempIMinus1N + j - 1].blue + src[tempIMinus1N + j].blue + src[tempIMinus1N + j + 1].blue
                     + src[tempI1N + j - 1].blue + src[tempI1N + j].blue + src[tempI1N + j + 1].blue +
                     src[tempIPlus1N + j - 1].blue + src[tempIPlus1N + j].blue + src[tempIPlus1N + j + 1].blue -tempBlueMin - tempBlueMax ) / 7;

            dst[temp].red = (src[tempIMinus1N + j - 1].red + src[tempIMinus1N + j].red + src[tempIMinus1N + j + 1].red
                             + src[tempI1N + j - 1].red + src[tempI1N + j].red + src[tempI1N + j + 1].red +
                             src[tempIPlus1N + j - 1].red + src[tempIPlus1N + j].red + src[tempIPlus1N + j + 1].red -tempRedMin - tempRedMax) /
                            7;
        }
    }
}


void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {


    pixel *pixelsImg = malloc(m * n * sizeof(pixel));
    pixel *backupOrg = malloc(m * n * sizeof(pixel));
    charsToPixels(image, pixelsImg);
    if (flag == '1') {
        // blur image
        doConvolutionBlur(pixelsImg, backupOrg);
        pixelsToChars(backupOrg, image);
        // write result image to file
        writeBMP(image, srcImgpName, blurRsltImgName);
        // sharpen the resulting image
        doConvolutionSharpen(backupOrg, pixelsImg);
        pixelsToChars(pixelsImg, image);
        // write result image to file
        writeBMP(image, srcImgpName, sharpRsltImgName);


    } else {
        // apply extermum filtered kernel to blur image
        doConvolutionBlurFilter(pixelsImg, backupOrg);
        pixelsToChars(backupOrg, image);
        // write result image to file
        writeBMP(image, srcImgpName, filteredBlurRsltImgName);
        // sharpen the resulting image
        doConvolutionSharpen(backupOrg, pixelsImg);
        pixelsToChars(pixelsImg, image);
        // write result image to file
        writeBMP(image, srcImgpName, filteredSharpRsltImgName);
    }
    free(pixelsImg);
    free(backupOrg);
}

