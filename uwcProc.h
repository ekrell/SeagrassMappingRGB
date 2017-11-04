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
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>
#include <sstream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h> 

using namespace cv;
using namespace std;

#ifndef UWCPROC_H_
#define UWCPROC_H_

/* Constants */
#define CHANNEL_R 3 // Red channel flag
#define CHANNEL_G 2 // Green channel flag
#define CHANNEL_B 1 // Blue channel flag
#define CHANNEL_GRAY -1 // Gray "channel" flag
#define CHANNEL_ALL 0 // BGR full flag

struct image_t {
	string sFileHandle;
	int id;

    /* texture features */
	double sobelMax;
    double sobelMean;
    double sobelSum;
    double laplacianMax;
    double laplacianMean;
    double laplacianSum;
    double cannyMean;
    double cannySum;
    double cannyEntropy;

    /* color features */
    double Bsum;
    double Bprop;
    double Gsum;
    double Gprop;
    double Rsum;
    double Rprop;
    double totalColor;

    double hueMean;
    double hueMax;
    double hueMin;
    double hueRangeLen;
};




/*  Section: Processes 
    The functions in this category modify a "source" image
    and save the changes to a "destination" image.
*/

void whiteBalance_grayWorld(const Mat src, Mat &dst);

void gaussianBlur(const Mat src, Mat &dst);

void laplacian(const Mat src, Mat &dst);

void sobelFilter(const Mat src, Mat &dst);

void cannyThreshold(const Mat src, Mat &dst, int lowThreshold, int highThreshold);



/*  Section: Measurements
    The functions in this category report information
    about an image.
*/

double getEntropy(const Mat src);


float entropy(Mat seq, Size size, int index);
Mat myEntropy(Mat seq, int histSize);


/* Section: Utilities 
*/

void ShowManyImages(string title, int nArgs, ...);

#endif // UWCPROC_H_
