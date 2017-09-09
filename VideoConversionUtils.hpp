#include "shubham.hpp"
#include <CL/cl.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/ocl/ocl.hpp"
#include "opencv/cv.h"
#include "fstream"
#include "string"
using namespace ocl;

void appendAsciiImageToVideo(Mat& img, char** asciiArray,VideoWriter& output,int toImageFactor)
{
    int r = img.rows;
    int c = img.cols;
    int row = 15*r+10;
    int col = 15*c;
    Mat asciiImg(row,col,CV_8UC1, Scalar(255));
    //Mat asciiImageSmall;
       
    stringToMat(asciiImg, asciiArray,r,c,toImageFactor);
    //ocl::resize(asciiImg,asciiImg,Size(640,480));
    try{
    //cout<<"Yo3\n";
    ocl::oclMat inp(asciiImg);
    ocl::oclMat opt(asciiImg);
    //cout<<opt.rows<<endl;
    //cout<<"Yo5\n";
    ocl::resize(inp,opt,Size(640,480));
    asciiImg=Mat(opt);
    //cout<<"Yo4\n";
    }
    catch(Exception e)
    {
    cout<<"Error"<<endl;
    }
    //for(int i=0;i<r;i++)
    	//cout<<asciiArray[i]<<endl;
    imshow( "ASCII Video", asciiImg );
    //while(!output.isOpened());
    //output.write ( asciiImg );
}
