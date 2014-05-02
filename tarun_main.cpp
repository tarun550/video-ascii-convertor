//fprintf(stderr, "Can not load image %s\n", "/Users/shubhamjain/Development/images/smile.jpg");
#include <CL/cl.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/ocl/ocl.hpp"
#include "opencv/cv.h"
#include "fstream"
#include "string"
#include "shubham.hpp"
#include "tarun.hpp"
#define SDK_SUCCESS 0
#define SDK_FAILURE 1
using namespace std;
using namespace cv;
using namespace ocl;

char **asciiArray=NULL;
char symbolOfIntensity[256];
int r=0;

static int toLetterFactor=2;
static int toImageFactor=16;

int main()
{
    
    VideoCapture cap("mickey.mp4"); // open the video file for reading

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the video file" << endl;
         return -1;
    }

    //cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms

    double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video

     //cout << "Frame per seconds : " << fps << endl;

    namedWindow("ASCII Video",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
    namedWindow("Original Video",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
    
	
    vector<ocl::Info> oclinfo;
    int devnums = ocl::getDevice(oclinfo);
    if( devnums < 1 )
    {
        std::cout << "no device found\n";
        return -1;
    }
    
    //if you want to use undefault device, set it here
    setDevice(oclinfo[0]);
    ocl::setBinpath("./");
    cv::VideoWriter output;
    output.open ( "outputVideo.avi", CV_FOURCC('F', 'M', 'P', '4'), fps, cv::Size (640,480), false );
    bool b=0;
     //Main Program starts here
     int frameNo=1;
     ofstream outfile;
     outfile.open ("output.txt");
     
     int c=0;
     
    while(1)
    {
    cout<<frameNo<<endl;
    frameNo++;	    
    Mat img;
     bool bSuccess = cap.read(img); // read a new frame from video

         if (!bSuccess) //if not success, break loop
        {
                        cout << "Video Completed!" << endl;
                       break;
        }
    imshow( "Original Video", img );
    if(b==0)
    {
    //cin>>c;
    r=img.rows/toLetterFactor;
    asciiArray = (char**)malloc(sizeof(char*)*r);
    for (int i = 0; i < r; i++)
    {
        asciiArray[i] = (char*) malloc(sizeof(char)*(img.cols/toLetterFactor));
    }
    
    for (int i=0; i<256; i++) {
        symbolOfIntensity[i]=0;
    }
    fontIntensity(1, symbolOfIntensity);
    //imshow( "ASCII Video", asciiImg );
    }
    b=1;
    
    Mat im_gray;
    //cout<<"No of channels in img1 "<<img.channels()<<endl;
    //cout<<img.rows<<" "<<img.cols<<endl;
    
    //create array for the ascii vector

    //function calls
    oclMat im_gray2(img);
    const oclMat img2(img);
    ocl::cvtColor(img2,im_gray2, CV_RGB2GRAY);
    im_gray=Mat(im_gray2);
    //cout<<"Lol"<<endl;
    fillAsciiArrayViaIntensity(im_gray,asciiArray,symbolOfIntensity,toLetterFactor);
    
    //print to file
    /*for(int i=0;i<im_gray.rows;i++)
    {
    	for(int j=0;j<im_gray.cols;j++)
    		outfile<<asciiArray[i][j];
    	outfile<<"\n";
    }*/
    
    appendAsciiImageToVideo(img,asciiArray,output,toLetterFactor);
    
    
    
    
    
    if(waitKey(1) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
       {
                cout << "esc key is pressed by user" << endl; 
                break; 
       }
    }
    //free the allocated memories
    //free(symbolOfIntensity);
    /*for (int i = 0; i < r; i++)
    {
        free(asciiArray[i]);
    }
    free(asciiArray);
    */
    cap.release();
    output.release();
    outfile.close();
    return 0;
         
}

