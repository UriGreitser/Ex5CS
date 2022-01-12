#include <stdbool.h>
#include "readBMP.h"
#include "writeBMP.h"

typedef struct {
     short red;
     short green;
     short blue;
} pixel;

typedef struct {
    int red;
    int green;
    int blue;
    // int num;
} pixel_sum;


/* Compute min and max of two integers, respectively */
int min(int a, int b) { return (a < b ? a : b); }
int max(int a, int b) { return (a > b ? a : b); }

int calcIndex(int i, int j, int n) {
	return ((i)*(n)+(j));
}

/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
void initialize_pixel_sum(pixel_sum *sum) {
	sum->red = sum->green = sum->blue = 0;
	// sum->num = 0;
	return;
}

/*
 * assign_sum_to_pixel - Truncates pixel's new value to match the range [0,255]
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum, int kernelScale) {

	// divide by kernel's weight
	sum.red = sum.red / kernelScale;
	sum.green = sum.green / kernelScale;
	sum.blue = sum.blue / kernelScale;

	// truncate each pixel's color values to match the range [0,255]
	current_pixel->red = (unsigned char) (min(max(sum.red, 0), 255));
	current_pixel->green = (unsigned char) (min(max(sum.green, 0), 255));
	current_pixel->blue = (unsigned char) (min(max(sum.blue, 0), 255));
	return;
}

/*
* sum_pixels_by_weight - Sums pixel values, scaled by given weight
*/
static void sum_pixels_by_weight(pixel_sum *sum, pixel p, int weight) {
	sum->red += ((int) p.red) * weight;
	sum->green += ((int) p.green) * weight;
	sum->blue += ((int) p.blue) * weight;
	// sum->num++;
	return;
}

/*
 *  Applies kernel for pixel at (i,j)
 */
static pixel applyKernel(int dim, int i, int j, pixel *src, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {

	int ii, jj;
	int currRow, currCol;
	pixel_sum sum;
	pixel current_pixel;
	int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
	int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
	int min_row, min_col, max_row, max_col;
	pixel loop_pixel;

	initialize_pixel_sum(&sum);

	for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) {
		for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) {

			int kRow, kCol;

			// compute row index in kernel
			if (ii < i) {
				kRow = 0;
			} else if (ii > i) {
				kRow = 2;
			} else {
				kRow = 1;
			}

			// compute column index in kernel
			if (jj < j) {
				kCol = 0;
			} else if (jj > j) {
				kCol = 2;
			} else {
				kCol = 1;
			}

			// apply kernel on pixel at [ii,jj]
			sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);
		}
	}

	if (filter) {
		// find min and max coordinates
		for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) {
			for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) {
				// check if smaller than min or higher than max and update
				loop_pixel = src[calcIndex(ii, jj, dim)];
				if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) <= min_intensity) {
					min_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
					min_row = ii;
					min_col = jj;
				}
				if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) > max_intensity) {
					max_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
					max_row = ii;
					max_col = jj;
				}
			}
		}
		// filter out min and max
		sum_pixels_by_weight(&sum, src[calcIndex(min_row, min_col, dim)], -1);
		sum_pixels_by_weight(&sum, src[calcIndex(max_row, max_col, dim)], -1);
	}

	// assign kernel's result to pixel at [i,j]
	assign_sum_to_pixel(&current_pixel, sum, kernelScale);
	return current_pixel;
}

/*
* Apply the kernel over each pixel.
* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than kernelSize/2 and/or
* column index smaller than kernelSize/2
*/
void smooth(int dim, pixel *src, pixel *dst, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {

    int i;
    int size = n * n;
    int limit = size - n;
    int sumRed = 0;
    int sumGreen = 0;
    int sumBlue = 0;

//	for (i = kernelSize / 2 ; i < dim - kernelSize / 2; i++) {
//		for (j =  kernelSize / 2 ; j < dim - kernelSize / 2 ; j++)
    for (i = n; i < limit; i++) {
        if (i % n == 0 || i % n == n - 1) {
            src[i] = dst[i];
            continue;
        }


    }
}

void charsToPixels(Image *charsImg, pixel* pixels) {

	int row, col;
	for (row = 0 ; row < m ; row++) {  // change
		for (col = 0 ; col < n ; col++) {
            int x = row*n+ col;
            int y = 3*row*n;
            int z = 3*col;
			pixels[x].red =(unsigned char) image->data[y + z];
			pixels[x].green =(unsigned char ) image->data[y + z + 1];
			pixels[x].blue = (unsigned char)image->data[y + z + 2];
		}
	}
}

void pixelsToChars(pixel* pixels, Image *charsImg) {

	int row, col;
	for (row = 0 ; row < m ; row++) {
		for (col = 0 ; col < n ; col++) {
            int x = 3*row*n;
            int y = 3*col;
            int z = row*n + col;
			image->data[x + y] = (unsigned char)pixels[z].red;
			image->data[x + y + 1] = (unsigned char)pixels[z].green;
			image->data[x + y + 2] = (unsigned char) pixels[z].blue;
		}
	}
}

void copyPixels(pixel* src, pixel* dst) {

	int row, col;
	for (row = 0 ; row < m ; row++) {
		for (col = 0 ; col < n ; col++) {
            int x = row*n + col;
			dst[x].red = src[x].red;
			dst[x].green = src[x].green;
			dst[x].blue = src[x].blue;
		}
	}
}

void doConvolution(Image *image, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {

	pixel* pixelsImg = malloc(m*n*sizeof(pixel));
	pixel* backupOrg = malloc(m*n*sizeof(pixel));

	charsToPixels(image, pixelsImg);
	//copyPixels(pixelsImg, backupOrg);

	smooth(m,pixelsImg, backupOrg , kernelSize, kernel, kernelScale, filter);

	pixelsToChars(backupOrg, image);

	free(pixelsImg);
	free(backupOrg);
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
    i = n;
    // copies the side frames
    for (i = (n*2)-1;i<limit;i += n) {
        dst[i] = src[i];
        dst[i+1] = src[i+1];
    }
    i = 0;
    int j;
    	for (i = 1 ; i < n - 1; i++) {
	    	for (j =  1 ; j < n - 1 ; j++) {
                int temp = i*n + j;
                int tempIMinus1N = (i-1)*n;
                int tempIPlus1N = (i+1)*n;
                int tempI1N = (i)*n;
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
    for (i = 1; i < n - 1; i++) {
        for (j = 1; j < n - 1; j++) {
            int temp = i * n + j;
            int tempIMinus1N = (i - 1) * n;
            int tempIPlus1N = (i + 1) * n;
            int tempI1N = (i) * n;
            dst[temp].green = (-src[tempIMinus1N + j - 1].green - src[tempIMinus1N + j].green - src[tempIMinus1N + j +1].green
                               - src[tempI1N + j - 1].green + 9* src[tempI1N + j].green - src[tempI1N+ j +1].green -
                               src[tempIPlus1N + j - 1].green - src[tempIPlus1N + j].green - src[tempIPlus1N + j +1].green);
            if (dst[temp].green > 255)
                dst[temp].green = 255;
            if (dst[temp].green < 0)
                dst[temp].green = 0;

            dst[temp].blue = (-src[tempIMinus1N + j - 1].blue - src[tempIMinus1N + j].blue - src[tempIMinus1N + j +1].blue
                              - src[tempI1N + j - 1].blue + 9* src[tempI1N + j].blue - src[tempI1N + j +1].blue -
                              src[tempIPlus1N + j - 1].blue - src[tempIPlus1N + j].blue - src[tempIPlus1N+ j +1].blue);
            if (dst[temp].blue > 255)
                dst[temp].blue = 255;
            if (dst[temp].blue < 0)
                dst[temp].blue = 0;


            dst[temp].red = (-src[tempIMinus1N + j - 1].red - src[tempIMinus1N + j].red - src[tempIMinus1N + j +1].red
                             - src[tempI1N + j - 1].red + 9* src[tempI1N + j].red - src[tempI1N+ j +1].red -
                             src[tempIPlus1N + j - 1].red - src[tempIPlus1N + j].red - src[tempIPlus1N + j +1].red);
            if (dst[temp].red > 255)
                dst[temp].red = 255;
            if (dst[temp].red < 0)
                dst[temp].red = 0;


        }
    }
}

void doConvolutionBlurFilter(pixel* src, pixel* dst) {
    int i = 0;
    for (i = 0; i < n; i++) {
        dst[i] = src[i];
    }
    int start = n * n;
    int limit = start - n;
    for (i = start; i > limit; i--) {
        dst[i] = src[i];
    }
    dst[n] = src[n];
    i = n;
    // copies the side frames
    for (i = (n * 2) - 1; i < limit; i += n) {
        dst[i] = src[i];
        dst[i + 1] = src[i + 1];
    }
    i = 0;
    int j;
    int maxSum = 0, minSum = 0;
    int maxI = 1, minI = 1;
    int sumTemp = 0;
    for (i = 1; i < n - 1; i++) {
        for (j = 1; j < n - 1; j++) {
            int temp = i * n + j;
            int tempIMinus1N = (i - 1) * n;
            int tempIPlus1N = (i + 1) * n;
            int tempI1N = (i) * n;
             maxSum = maxSum ^ maxSum;
             minSum = minSum ^ minSum;
             maxI = 1;
             minI = 1;
            sumTemp = sumTemp ^ sumTemp;
            unsigned int tempGreenMax = src[tempIMinus1N + j - 1].green;
            unsigned int tempBlueMax = src[tempIMinus1N + j - 1].blue;
            unsigned int tempRedMax = src[tempIMinus1N + j - 1].red;
            unsigned int tempGreenMin = src[tempIMinus1N + j - 1].green;
            unsigned int tempBlueMin = src[tempIMinus1N + j - 1].blue;
            unsigned int tempRedMin = src[tempIMinus1N + j - 1].red;
            // finds min and max
            maxSum = src[tempIMinus1N + j - 1].green + src[tempIMinus1N + j - 1].red + src[tempIMinus1N + j - 1].blue;
            minSum = maxSum;
            //number 2/9
            sumTemp = src[tempIMinus1N + j ].green + src[tempIMinus1N + j ].red + src[tempIMinus1N + j].blue;
            if (sumTemp > maxSum) {
                maxSum = sumTemp;
                maxI = 2;
                tempGreenMax = src[tempIMinus1N + j ].green;
                tempBlueMax = src[tempIMinus1N + j ].blue;
                tempRedMax = src[tempIMinus1N + j ].red;
            }
            else if (sumTemp <= minSum) {
                minSum = sumTemp;
                minI = 2;
                tempGreenMin = src[tempIMinus1N + j ].green;
                tempBlueMin = src[tempIMinus1N + j ].blue;
                tempRedMin = src[tempIMinus1N + j ].red;
            }
            //number 3/9
            sumTemp = src[tempIMinus1N + j+1].green + src[tempIMinus1N + j+1].red + src[tempIMinus1N + j+1].blue;
            if (sumTemp > maxSum) {
                maxSum = sumTemp;
                maxI = 3;
                tempGreenMax = src[tempIMinus1N + j + 1].green;
                tempBlueMax = src[tempIMinus1N + j + 1].blue;
                tempRedMax = src[tempIMinus1N + j + 1].red;
            }
            else if (sumTemp <= minSum) {
                minSum = sumTemp;
                minI = 3;
                tempGreenMin = src[tempIMinus1N + j + 1].green;
                tempBlueMin = src[tempIMinus1N + j + 1].blue;
                tempRedMin = src[tempIMinus1N + j + 1].red;
            }
            //number 4/9
            sumTemp = src[tempI1N + j-1].green + src[tempI1N + j-1].red + src[tempI1N + j-1].blue;
            if (sumTemp > maxSum) {
                maxSum = sumTemp;
                maxI = 4;
                tempGreenMax = src[tempI1N + j - 1].green;
                tempBlueMax = src[tempI1N + j - 1].blue;
                tempRedMax = src[tempI1N + j - 1].red;
            }
            else if (sumTemp <= minSum) {
                minSum = sumTemp;
                minI = 4;
                tempGreenMin = src[tempI1N + j - 1].green;
                tempBlueMin = src[tempI1N + j - 1].blue;
                tempRedMin = src[tempI1N + j - 1].red;
            }
            //number 5/9
            sumTemp = src[tempI1N + j].green + src[tempI1N + j].red + src[tempI1N + j].blue;
            if (sumTemp > maxSum) {
                maxSum = sumTemp;
                maxI = 5;
                tempGreenMax = src[tempI1N + j ].green;
                tempBlueMax = src[tempI1N + j ].blue;
                tempRedMax = src[tempI1N + j ].red;
            }
            else if (sumTemp <= minSum) {
                minSum = sumTemp;
                minI = 5;
                tempGreenMin = src[tempI1N + j ].green;
                tempBlueMin = src[tempI1N + j ].blue;
                tempRedMin = src[tempI1N + j ].red;
            }
            //number 6/9
            sumTemp = src[tempI1N + j+1].green + src[tempI1N + j+1].red + src[tempI1N + j+1].blue;
            if (sumTemp > maxSum) {
                maxSum = sumTemp;
                maxI = 6;
                tempGreenMax = src[tempI1N + j +1].green;
                tempBlueMax = src[tempI1N + j +1].blue;
                tempRedMax = src[tempI1N + j +1].red;
            }
            else if (sumTemp <= minSum) {
                minSum = sumTemp;
                minI = 6;
                tempGreenMin = src[tempI1N + j +1].green;
                tempBlueMin = src[tempI1N + j +1].blue;
                tempRedMin = src[tempI1N + j +1].red;
            }
            //number 7/9
            sumTemp = src[tempIPlus1N + j-1].green + src[tempIPlus1N + j-1].red + src[tempIPlus1N + j-1].blue;
            if (sumTemp > maxSum) {
                maxSum = sumTemp;
                maxI = 7;
                tempGreenMax = src[tempIPlus1N + j-1 ].green;
                tempBlueMax = src[tempIPlus1N + j -1].blue;
                tempRedMax = src[tempIPlus1N + j -1].red;
            }
            else if (sumTemp <= minSum) {
                minSum = sumTemp;
                minI = 7;
                tempGreenMin = src[tempIPlus1N + j -1].green;
                tempBlueMin = src[tempIPlus1N + j-1 ].blue;
                tempRedMin = src[tempIPlus1N + j -1].red;
            }
            //number 8/9
            sumTemp = src[tempIPlus1N + j].green + src[tempIPlus1N + j].red + src[tempIPlus1N + j].blue;
            if (sumTemp > maxSum) {
                maxSum = sumTemp;
                maxI = 8;
                tempGreenMax = src[tempIPlus1N + j ].green;
                tempBlueMax = src[tempIPlus1N + j ].blue;
                tempRedMax = src[tempIPlus1N + j ].red;
            }
            else if (sumTemp <= minSum) {
                minSum = sumTemp;
                minI = 8;
                tempGreenMin = src[tempIPlus1N + j ].green;
                tempBlueMin = src[tempIPlus1N + j ].blue;
                tempRedMin = src[tempIPlus1N + j ].red;
            }
            //number 9/9
            sumTemp = src[tempIPlus1N + j+1].green + src[tempIPlus1N + j+1].red + src[tempIPlus1N + j+1].blue;
            if (sumTemp > maxSum) {
                maxSum = sumTemp;
                maxI = 9;
                tempGreenMax = src[tempIPlus1N + j +1].green;
                tempBlueMax = src[tempIPlus1N + j +1].blue;
                tempRedMax = src[tempIPlus1N + j +1].red;
            }
            else if (sumTemp <= minSum) {
                minSum = sumTemp;
                minI = 9;
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

    /*
    * [1, 1, 1]
    * [1, 1, 1]
    * [1, 1, 1]
    */
    int blurKernel[3][3] = {{1, 1, 1},
                            {1, 1, 1},
                            {1, 1, 1}};

    /*
    * [-1, -1, -1]
    * [-1, 9, -1]
    * [-1, -1, -1]
    */
    int sharpKernel[3][3] = {{-1, -1, -1},
                             {-1, 9,  -1},
                             {-1, -1, -1}};
    pixel *pixelsImg = malloc(m * n * sizeof(pixel));
    pixel *backupOrg = malloc(m * n * sizeof(pixel));
    charsToPixels(image, pixelsImg);
    if (flag == '3') {
        // blur image
        doConvolutionBlur(pixelsImg, backupOrg);
        pixelsToChars(backupOrg, image);
        // write result image to file
        writeBMP(image, srcImgpName, blurRsltImgName);

        doConvolutionSharpen(backupOrg, pixelsImg);
        pixelsToChars(pixelsImg, image);
        writeBMP(image, srcImgpName, sharpRsltImgName);

        // sharpen the resulting image

        // write result image to file
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
}

