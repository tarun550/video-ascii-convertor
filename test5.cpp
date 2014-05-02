#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv/cv.h"
#include <iostream>
#include<cstdlib>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    VideoCapture cap("tnj.mp4"); // open the video file for reading

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the video file" << endl;
         return -1;
    }

    //cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms

    double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video

     cout << "Frame per seconds : " << fps << endl;

    namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

    while(1)
    {
        Mat frame;

        bool bSuccess = cap.read(frame); // read a new frame from video

         if (!bSuccess) //if not success, break loop
        {
                        cout << "Cannot read the frame from video file" << endl;
                       break;
        }

   Mat img2(frame.rows,frame.cols,CV_8UC3);
   FileStorage file("some_name.txt", cv::FileStorage::WRITE);
   cout<<img2.rows<<" "<<img2.cols<<endl;
   Mat im_gray;
   
   string s="";
   string hello = "HELLO";
   
   cvtColor(frame,im_gray,CV_BGR2GRAY);
   uchar intensity;
   //float data[img.rows][img.cols];
   for(int i = 0;i < frame.rows;i++){
       for(int j = 0;j < frame.cols;j++){
           Scalar pixel = im_gray.at<uchar>(i,j);
           intensity = pixel.val[0];
           if (intensity>160) {
               //cout<<" ";
               s+=" ";
           }
           else if (intensity>80){
               //cout<<".";
               s+=".";
           }
           else if (intensity>10){
               //cout<<"+";
               s+="+";
           }
           else{
               s+="#";
               //cout<<"#";
           }
       }
       s+="\n";
       
   }

	std::system ( "clear" );
        cout<<s;
        //imshow("MyVideo", frame); //show the frame in "MyVideo" window

        if(waitKey(30) == 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
       {
                cout << "esc key is pressed by user" << endl; 
                break; 
       }
    }

    return 0;

}
