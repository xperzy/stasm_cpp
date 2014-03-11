//============================================================================
// Name        : stasm.cpp
// Author      : Yu Zhu
// Version     : 1.0
// Copyright   : Your copyright notice
// Description : stasm, detect facial points given image path
//============================================================================

// minimal.cpp: Display the landmarks of a face in an image.
//              This demonstrates stasm_search_single.

#include <stdio.h>
#include <stdlib.h>
#include "opencv2/opencv.hpp"
#include "stasm_lib.h"
#include "stasm_landmarks.h"
#include <iostream>
#include <fstream>

int main(int argc, char * argv[])
{

	if( argc != 4){
	     std::cout <<" Usage: stasm image_path points_out_path image_out_path " << std::endl;
	     return -1;
	}

    //static const char* path = "../data/testface.jpg";
	static const char* path = argv[1];


	printf("Path: %s\n",path);

    //cv::Mat_<unsigned char> img(cv::imread(path, CV_LOAD_IMAGE_GRAYSCALE)); //read grayscale image

    cv::Mat img_rgb = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

    if (!img_rgb.data)
    {
        printf("Cannot load %s\n", path);
        exit(1);
    }

    cv::Mat img_grey;
    cvtColor(img_rgb, img_grey, CV_BGR2GRAY);

    int foundface;
    float landmarks[2 * stasm_NLANDMARKS]; // x,y coords

    if (!stasm_search_single(&foundface, landmarks,
                             (char*)img_grey.data, img_grey.cols, img_grey.rows, path, "./res/data/"))
    {
        printf("Error in stasm_search_single: %s\n", stasm_lasterr());
        exit(1);
    }

    if (!foundface)
         printf("No face found in %s\n", path);
    else
    {
    	std::ofstream outfile;
    	outfile.open(argv[2]);
    	if (!outfile.is_open()){
    		printf("Error in opening output file: %s\n",argv[2]);
    		exit(1);
    	}

        // draw the landmarks on the image as white dots
    	stasm_force_points_into_image(landmarks, img_grey.cols, img_grey.rows);

        for (int i = 0; i < stasm_NLANDMARKS; i++){
            //img_rgb(cvRound(landmarks[i*2+1]), cvRound(landmarks[i*2])) = 255;
        	cv::circle(img_rgb,cv::Point(landmarks[i*2],landmarks[i*2+1]),1.5,cv::Scalar(0,0,255),2);
        	outfile<< landmarks[i*2+1] << ", " << landmarks[i*2]<< std::endl;
        }
        outfile.close();
    }

    cv::imshow("stasm minimal", img_rgb);
    cv::waitKey();

    bool write_success = cv::imwrite(argv[3],img_rgb);
    if (!write_success){
    	printf("Error in saving image: %s\n",argv[3]);
    	exit(1);
    }
    return 0;
}

