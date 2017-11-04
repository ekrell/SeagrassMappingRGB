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

void cannyThreshold(const Mat src, Mat &dst, int lowThreshold, int highThreshold) {
    src.copyTo (dst);
    Canny (dst, dst, 20, 0, 3);
}



float entropy(Mat seq, Size size, int index)
{
  int cnt = 0;
  float entr = 0;
  float total_size = size.height * size.width; //total size of all symbols in an image

  for(int i=0;i<index;i++)
  {
    float sym_occur = seq.at<float>(0, i); //the number of times a sybmol has occured
    if(sym_occur>0) //log of zero goes to infinity
      {
        cnt++;
        entr += (sym_occur/total_size)*(log2(total_size/sym_occur));
      }
  }
  return entr;

}

// myEntropy calculates relative occurrence of different symbols within given input sequence using histogram
Mat myEntropy(Mat seq, int histSize)
{ 

  float range[] = { 0, 256 } ;
  const float* histRange = { range };

  bool uniform = true; bool accumulate = false;

  Mat hist;

  /// Compute the histograms:
  calcHist( &seq, 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate );

  return hist;
}

double getEntropy(const Mat src) {
    Mat hist;
    float e;
    
    /// Establish the number of bins
    int histSize = 256;

    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );

    hist = myEntropy(src, histSize);

    
    e= entropy(hist,src.size(), histSize);


    return (double) e;

}

		


