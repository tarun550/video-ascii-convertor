//fprintf(stderr, "Can not load image %s\n", "/Users/shubhamjain/Development/images/smile.jpg");
//#include <CL/cl.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"
#include "fstream"
#include "string"
#include "shubham.hpp"

#define SDK_SUCCESS 0
#define SDK_FAILURE 1
using namespace std;
using namespace cv;



  
//    namedWindow( "Test window", WINDOW_AUTOSIZE );// Create a window for display.
//    imshow( "Test window", letterImage );


static int toLetterFactor =16;
static int toImageFactor = 15;


int main()
{
    Mat img = imread("/Users/shubhamjain/Development/tut/BTPtrial/BTPtrial/pent.jpg",1);
    Mat im_gray;
    cout<<"No of channels in img1 "<<img.channels()<<endl;
    cout<<img.rows<<" "<<img.cols<<endl;
    
    //create array for the ascii vector
    char **asciiArray = (char**)malloc(sizeof(char*)*img.rows/toLetterFactor);
    for (int i = 0; i < img.rows/toLetterFactor; i++)
    {
        asciiArray[i] = (char*) malloc(sizeof(char)*img.cols/toLetterFactor);
    }
    
//    
//    char **asciiArray = (char**)malloc(sizeof(char*)*1);
//    for (int i = 0; i < 1; i++)
//    {
//        asciiArray[i] = (char*) malloc(sizeof(char)*256);
//    }
    
    
        
    //function calls
    cvtColor(img, im_gray, CV_BGR2GRAY);
    //fillAsciiArrayViaIntensity(im_gray,asciiArray,toLetterFactor);
    fillAsciiArrayViaDistanceMap(im_gray,asciiArray,toLetterFactor);
    
//    for (uchar t = 0; t<255; t++) {
//        asciiArray[0][t]=t;
//    }
    printAsciiImage(img, asciiArray,toLetterFactor,toImageFactor);
    
    
    
    
    
    
    
    //free the allocated memories
   
    for (int i = 0; i < img.rows/toLetterFactor; i++)
    {
        free(asciiArray[i]);
    }
    free(asciiArray);
    return 0;
         
}

