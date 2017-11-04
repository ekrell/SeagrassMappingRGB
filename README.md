## Onboard Seagrass Mapping and Hole Detection

### Version 0.1


**Note**

This project is related to the Airboat Shallow Water Science Agent project. See the [Airboat blog](https://airboat-blog.netlify.com/) for more details. 


### Introduction

See the Doc folder for project details. 

### Files Overview

This is very much in progress and has not been made portable. 
For example, some pathnames are absolute paths for Evan Krell's computer. 
Such issues will be resolved on the release of version 1.0. 

**uwcProc.h, uwcProc.cpp, uwcProc_driver.cpp, uwcProc_driver-features.csv**

Implements the image processing portion of the project. 
Various enhancement methods and feature extractions are implemented. 
The driver does the following. 
Reads input images from the Input. 
Outputs a CSV of the features and writes the enhances images to the Results directory.
The Makefile is set to build, run, and save the CSV to uwcProc_driver-features.csv.

**uwcCtrl.h, uwcCtrl.cpp, uwcCtrl_driver.cpp**

Not yet implemented, but will handle "control" of platform.
In the most simple case, would pull frames and GPS coordinates from the video stream
at the set sampling rate.

**uwcClassification.R**

Experiments in image classification. 
Done in R since I am familiar with it, but final version will be in C++. 

**uwcMain.cpp**

Not yet written, but will use the methods demonstrated in the drivers to 
implement a complete system that using a live stream or archived video as input.


