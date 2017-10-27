#include "uwcProc.h"
#include <vector>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

/*  Main: Loop over an process each image in a directory.
*/
int main (){

    /* Hard-coded params. Will later be command line args. */
    string sInputDirectory = "Data";
    string sOutputDirectory = "Results";
    string sFileExtension = ".png";
        /* Could have issue is '/' is part of directory name */
    string sGlobSearch = sInputDirectory + "/*" + sFileExtension;
    String SPath (sGlobSearch.c_str());
    
    /* Store list of image files */
    vector<String> vsImageFileHandles;
    glob(SPath, vsImageFileHandles, false);
    int iImageCount = (int) vsImageFileHandles.size(); 

    
    /* Process each image sequentially */
    Mat img, res, gray, lap, sobel, hsv;
    vector<Mat> channels(3);
    vector<Mat> channelsRes(3);
    image_t imgStat;
    Scalar tempScalar;

    int j = 0;
    char * csTemp, * csTemp2, * csOutfile;
    for (int i = 0; i < iImageCount; i++) {
        /* Load image */
        img = imread (vsImageFileHandles[i], IMREAD_COLOR);

        /* Ensure loaded correctly */
        if (img.empty()) {
            printf ("Error: Unable to read image\n");
            break;
        }

    /* Perform Image Processes */

        /* Apply white balancing */
        whiteBalance_grayWorld (img, res);

        /* Apply Gaussian Blur */
        gaussianBlur(res, res);

        /* Split into channels */
        split(res, channels);

        /* Edge detection each channel */
        for (j = 0; j < 3; j++) {
            laplacian(channels[j], channelsRes[j]);
        }

        cvtColor(res, gray, COLOR_BGR2GRAY);
        laplacian(gray, lap);
        sobelFilter(gray, sobel);

    /* Collect Image Measurements */
        
        /* texture features */
        imgStat.id = i;
        tempScalar = cv::mean(sobel);
        imgStat.sobelMean = tempScalar[0];
        tempScalar = cv::sum(sobel);
        imgStat.sobelSum = tempScalar[0];
        tempScalar = cv::mean(lap);
        imgStat.laplacianMean = tempScalar[0];
        tempScalar = cv::sum(lap);
        imgStat.laplacianSum = tempScalar[0];

        /*color features */

    /* Was not useful. Too balanced.
        tempScalar = cv::sum (img);
        imgStat.Bsum = tempScalar[0];
        imgStat.Gsum = tempScalar[1];
        imgStat.Rsum = tempScalar[2];
        imgStat.totalColor = imgStat.Bsum + imgStat.Gsum + imgStat.Rsum;
        imgStat.Bprop = tempScalar[0] / imgStat.totalColor;
        imgStat.Gprop = tempScalar[1] / imgStat.totalColor;
        imgStat.Rprop = tempScalar[2] / imgStat.totalColor;
    */

        cvtColor (img, hsv, COLOR_BGR2HSV);
        split (res, channels);
        tempScalar = cv::mean (hsv);
        imgStat.hueMean = tempScalar[0];
        minMaxLoc (channels[0], &imgStat.hueMin, &imgStat.hueMax, NULL, NULL); 
        imgStat.hueRangeLen = imgStat.hueMax - imgStat.hueMin;






        /* Print texture measurements */
        printf ("%f,%f,%f,%f,", 
                imgStat.sobelMean, imgStat.sobelSum, imgStat.laplacianMean, imgStat.laplacianSum);
        /* Print color measurements */
        printf ("%f,%f,%f,%f,",
                imgStat.hueMean, imgStat.hueMax, imgStat.hueMin, imgStat.hueRangeLen);
        printf ("\n");



        /* Generate output file name based on original filename and output directory */
        csTemp = strdup (vsImageFileHandles[i].c_str());
        csTemp2 = (char *) malloc (sizeof (char) * strlen (csTemp) - strlen (sInputDirectory.c_str()) + 1);
        strncpy (csTemp2, csTemp + strlen (sInputDirectory.c_str()), strlen (csTemp) - strlen(sInputDirectory.c_str()));
        csTemp2[strlen (csTemp) - strlen (sInputDirectory.c_str())] = '\0';
        csOutfile = (char *) malloc (sizeof (char) * strlen (csTemp2) + strlen (sOutputDirectory.c_str()) + 1);
        strncpy (csOutfile, sOutputDirectory.c_str(), strlen (sOutputDirectory.c_str()));
        strcat (csOutfile, csTemp2);
        csOutfile[strlen (csTemp2) + strlen (sOutputDirectory.c_str())] = '\0';
        printf ("%s\n", csOutfile);

        /* Write result image */
        imwrite (csOutfile, res);

        /* Display */
        //ShowManyImages("Image", 3, img, res, lap);
        //namedWindow("my", CV_WINDOW_NORMAL);
        //imshow("my", res);
        //waitKey(0);
        //destroyWindow("my");

        /* Clean up memory */
        free (csTemp);
        free (csTemp2);
        free (csOutfile);

    }
    


    return 0;
}

