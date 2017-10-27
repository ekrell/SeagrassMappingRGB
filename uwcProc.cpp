/*  Image processing functions for Underwater Camera
    Intended for online and offline use.

    Evan Krell

    This program is free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License version
    3 as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see
    <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "uwcProc.h" 

void whiteBalance_grayWorld(const Mat src, Mat &dst){
	uint H = src.rows,
	     W = src.cols,
	     N = H * W;

	// Create output matrix
	dst = Mat(src.size(), src.type());

	// Calculate sum of values
	ulong sum1 = 0, sum2 = 0, sum3 = 0;
	Vec3b pixel;
	for (uint j = 0; j < H; j++)
		for (uint i = 0; i < W; i++)
		{
			pixel = src.at<Vec3b>(j, i);
			sum1 += pixel[0];
			sum2 += pixel[1];
			sum3 += pixel[2];
		}

	// Find inverse of averages
	float inv1 = sum1 == 0 ? 0.f : (float)N / (float)sum1,
	      inv2 = sum2 == 0 ? 0.f : (float)N / (float)sum2,
	      inv3 = sum3 == 0 ? 0.f : (float)N / (float)sum3;

	// Find maximum
	float inv_max = max(inv1, max(inv2, inv3));

	// Scale by maximum
	if (inv_max > 0)
	{
		inv1 /= inv_max;
		inv2 /= inv_max;
		inv3 /= inv_max;
	}

	// Scale input pixel values
	for (uint j = 0; j < H; j++)
		for (uint i = 0; i < W; i++)
		{
			pixel = src.at<Vec3b>(j, i);
			pixel[0] = pixel[0] * inv1;
			pixel[1] = pixel[1] * inv2;
			pixel[2] = pixel[2] * inv3;
			dst.at<Vec3b>(j, i) = pixel;
		}
		
}

void gaussianBlur(const Mat src, Mat &dst) {
	GaussianBlur(src, dst, Size (9, 9), 0, 0);
}

void laplacian(const Mat src, Mat &dst) {
	Laplacian(src, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT);
}

void sobelFilter(const Mat src, Mat &dst) {
    //Source: https://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/sobel_derivatives/sobel_derivatives.html
    int scale = 30;
    int delta = 0;
    int ddepth = CV_16S;

	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;

    Sobel (src, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
    convertScaleAbs(grad_x, abs_grad_x);
    Sobel (src, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
    convertScaleAbs(grad_y, abs_grad_y);

    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);
}



void ShowManyImages(string title, int nArgs, ...) {
int size;
int i;
int m, n;
int x, y;

// w - Maximum number of images in a row
// h - Maximum number of images in a column
int w, h;

// scale - How much we have to resize the image
float scale;
int max;

// If the number of arguments is lesser than 0 or greater than 12
// return without displaying
if(nArgs <= 0) {
    printf("Number of arguments too small....\n");
    return;
}
else if(nArgs > 14) {
    printf("Number of arguments too large, can only handle maximally 12 images at a time ...\n");
    return;
}
// Determine the size of the image,
// and the number of rows/cols
// from number of arguments
else if (nArgs == 1) {
    w = h = 1;
    size = 300;
}
else if (nArgs == 2) {
    w = 2; h = 1;
    size = 300;
}
else if (nArgs == 3 || nArgs == 4) {
    w = 2; h = 2;
    size = 300;
}
else if (nArgs == 5 || nArgs == 6) {
    w = 3; h = 2;
    size = 200;
}
else if (nArgs == 7 || nArgs == 8) {
    w = 4; h = 2;
    size = 200;
}
else {
    w = 4; h = 3;
    size = 150;
}

// Create a new 3 channel image
Mat DispImage = Mat::zeros(Size(100 + size*w, 60 + size*h), CV_8UC3);

// Used to get the arguments passed
va_list args;
va_start(args, nArgs);

// Loop for nArgs number of arguments
for (i = 0, m = 20, n = 20; i < nArgs; i++, m += (20 + size)) {
    // Get the Pointer to the IplImage
    Mat img = va_arg(args, Mat);

    // Check whether it is NULL or not
    // If it is NULL, release the image, and return
    if(img.empty()) {
        printf("Invalid arguments");
        return;
    }

    // Find the width and height of the image
    x = img.cols;
    y = img.rows;

    // Find whether height or width is greater in order to resize the image
    max = (x > y)? x: y;

    // Find the scaling factor to resize the image
    scale = (float) ( (float) max / size );

    // Used to Align the images
    if( i % w == 0 && m!= 20) {
        m = 20;
        n+= 20 + size;
    }

    // Set the image ROI to display the current image
    // Resize the input image and copy the it to the Single Big Image
    Rect ROI(m, n, (int)( x/scale ), (int)( y/scale ));
    Mat temp; resize(img,temp, Size(ROI.width, ROI.height));
    temp.copyTo(DispImage(ROI));
}

// Create a new window, and show the Single Big Image
namedWindow( title, 1 );
imshow( title, DispImage);
waitKey();

// End the number of arguments
va_end(args);
}


		


