//
//  shubham.hpp
//  BTPtrial
//
//  Created by Shubham Jain on 03/04/14.
//  Copyright (c) 2014 Shubham. All rights reserved.
//

#ifndef BTPtrial_shubham_hpp
#define BTPtrial_shubham_hpp

#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv/cv.h"
#include "fstream"
#include "string"

using namespace std;
using namespace cv;

void stringToMat(Mat &img, char** letters, int row, int col, int toImageFactor){
    Point textOrg(toImageFactor,toImageFactor);
    
    for(int i = 0; i< row; i++){
        textOrg.x=0;
        for (int j = 0; j<col; j++) {
            
            string temp = "";
            temp +=letters[i][j];
            //cout<<temp<<"<-\n";
            //adjust size of letter and boldness according to the image factor
            putText(img, temp, textOrg, FONT_HERSHEY_SIMPLEX, .5, Scalar::all(0),1,8,false);
            textOrg.x+=toImageFactor;
        }
        textOrg.y+=toImageFactor;
    }
    return;
}

void fontIntensity(int font, char *symbolOfIntensity){
    int row = 15;
    int col = 15;
    Point textOrg(0,12);
    int weightedMean[][15] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
        
    };
    
    float minMean = 999999999;
    float maxMean = INT_MIN;
    int intensityValue[256]={0};
    for (char k=31; k<126; k++) {
        Mat letterImage(row,col,CV_8UC1, Scalar(255));
        string s="";
        s+=k;
        putText(letterImage, s, textOrg, FONT_HERSHEY_SCRIPT_SIMPLEX, .5, Scalar::all(0),1.5,8,false);
        float mean = 0;
        
        for (int i =0; i<15; i++) {
            for (int j=0; j<15; j++) {
                Scalar pixel = letterImage.at<uchar>(i,j);
                uchar intensity = pixel.val[0];
                mean=mean+(255-intensity)*weightedMean[i][j];
                
                
            }
        }
        if(mean>maxMean)
            maxMean=mean;
        if (mean<minMean)
            minMean=mean;
        intensityValue[k]=(int)mean;
        
    }
    //cout<<"the value of minMean is "<<minMean<<"\n";
    for (char k=32; k<126; k++){
        
        int index =((intensityValue[k]-minMean)/(maxMean-minMean))*256;
        //cout<<"ascci-"<<k<<"index="<<index<<"\n";
        symbolOfIntensity[index]=k;
    }
    
    char buff=symbolOfIntensity[0];
    //cout<<"Buff="<<buff<<"\n";
    for (int i=0; i<256; i++) {
        if (symbolOfIntensity[i]==0) {
            symbolOfIntensity[i]=buff;
        }
        else{
            buff=symbolOfIntensity[i];
        }
        //cout<<"at "<<i<<" "<<symbolOfIntensity[i]<<"\n";
    }
}



void fillAsciiArrayViaIntensity(Mat &img, char** asciiArray, int toLetterFactor)
{
    char symbolOfIntensity[256];
    for (int i=0; i<256; i++) {
        symbolOfIntensity[i]=0;
    }
    fontIntensity(1, symbolOfIntensity);
    
    //applying algo filling character array
    for(int i = 0;i < img.rows;i+=toLetterFactor){
        for(int j = 0;j < img.cols;j+=toLetterFactor){
            
            
            
            float intense = 0.0;
            for (int bi = i; bi<i+toLetterFactor; bi++) {
                for (int bj = j; bj<j+toLetterFactor; bj++) {
                    Scalar pixel = img.at<uchar>(bi,bj);
                    uchar intensity = pixel.val[0];
                    intense = intense+((float)intensity/toLetterFactor)/toLetterFactor;
                    
                }
            }
            
            
            asciiArray[i/toLetterFactor][j/toLetterFactor]=symbolOfIntensity[255-int(intense)];
            //                        if (intensity>160) {
            //                            cout<<" ";
            //                            s+=" ";
            //                            asciiArray[i][j]=' ';
            //
            //                        }
            //                        else if (intensity>80){
            //                            cout<<".";
            //                            s+=".";
            //                            asciiArray[i][j]='.';
            //                        }
            //                        else if (intensity>10){
            //                            cout<<"+";
            //                            s+="+";
            //                            asciiArray[i][j]='+';
            //                        }
            //                        else{
            //                            s+="#";
            //                            cout<<"#";
            //                            asciiArray[i][j]='#';
            //                        }
        }
        //cout<<"\n";
        
    }
}



void fillAsciiArrayViaDistanceMap(Mat &img, char** asciiArray, int toLetterFactor)
{
    
    
    for(int i = 0;i < img.rows;i+=toLetterFactor){
        for(int j = 0;j < img.cols;j+=toLetterFactor){
            //convert to binary matrix
            cout<<"binary matrix\n";
            int binaryImage[toLetterFactor][toLetterFactor];
            for (int bi = i; bi<i+toLetterFactor; bi++) {
                for (int bj = j; bj<j+toLetterFactor; bj++) {
                    Scalar pixel = img.at<uchar>(bi,bj);
                    uchar intensity = pixel.val[0];
                    if (intensity>127) {
                        binaryImage[bi%toLetterFactor][bj%toLetterFactor]= 1;
                    }
                    else
                    {
                        binaryImage[bi%toLetterFactor][bj%toLetterFactor]=0;
                    }
                    
                    cout<<binaryImage[bi%toLetterFactor][bj%toLetterFactor]<<" ";
                }
                cout<<"\n";
            }
            //create distance map using dp
            int minTopLeft[toLetterFactor][toLetterFactor];
            int minBottomRight[toLetterFactor][toLetterFactor];
            int distanceMap[toLetterFactor][toLetterFactor];
            for (int k = 0; k<toLetterFactor; k++) {
                for (int l = 0; l<toLetterFactor; l++) {
                    if(binaryImage[k][l]==0)
                        minTopLeft[k][l]=0;
                    else
                    {
                        int min = INT_MAX;
                        if(k!=0 && minTopLeft[k-1][l]<min)
                        {
                            minTopLeft[k][l]=minTopLeft[k-1][l]+1;
                            min =minTopLeft[k-1][l];
                        }
                        if(l!=0 && minTopLeft[k][l-1]<min)
                        {
                            minTopLeft[k][l]=minTopLeft[k][l-1]+1;
                            min =minTopLeft[k][l-1];
                        }
                        if(k!=0 && l!=0 && minTopLeft[k-1][l-1]<min)
                        {
                            minTopLeft[k][l]=minTopLeft[k-1][l-1]+1;
                            min =minTopLeft[k-1][l-1];
                        }
                        if(k==0 || l==0)
                            minTopLeft[k][l]=1;
                    }
                    
                    
                }
            }
            for (int k = toLetterFactor-1; k>=0; k--) {
                for (int l = toLetterFactor-1; l>=0; l--) {
                    if(binaryImage[k][l]==0)
                        minBottomRight[k][l]=0;
                    else
                    {
                        int min = INT_MAX;
                        if(k!=toLetterFactor-1 && minBottomRight[k+1][l]<min)
                        {
                            minBottomRight[k][l]=minBottomRight[k+1][l]+1;
                            min =minBottomRight[k+1][l];
                        }
                        if(l!=toLetterFactor-1 && minBottomRight[k][l+1]<min)
                        {
                            minBottomRight[k][l]=minBottomRight[k][l+1]+1;
                            min =minBottomRight[k][l+1];
                        }
                        if(k!=toLetterFactor-1 && l!=toLetterFactor-1 && minTopLeft[k+1][l+1]<min)
                        {
                            minBottomRight[k][l]=minBottomRight[k+1][l+1]+1;
                            min =minBottomRight[k+1][l+1];
                        }
                        if(k==toLetterFactor-1 || l==toLetterFactor-1)
                            minBottomRight[k][l]=1;
                    }
                    
                    
                }
            }
            cout<<"Distance map\n";
            for (int k = 0; k<toLetterFactor; k++) {
                for (int l = 0; l<toLetterFactor; l++) {
                    distanceMap[k][l]=minBottomRight[k][l]>minTopLeft[k][l]?minTopLeft[k][l]:minBottomRight[k][l];
                    cout<<distanceMap[k][l]<<" ";
                }
                cout<<"\n";
            }
            
            //apply multiplication to all and find min sum.
            //int row = 15;
            int row = toLetterFactor;
            //int col = 15;
            int col = toLetterFactor;
            Point textOrg(0,(12*toLetterFactor)/15);
            char replacementCharacter=' ';
            int minDistance=INT_MAX;
            
            for (char k=32; k<126; k++) {
                Mat letterImage(row,col,CV_8UC1, Scalar(255));
                string s="";
                s+=k;
                putText(letterImage, s, textOrg, FONT_HERSHEY_SCRIPT_SIMPLEX, (.5*toLetterFactor)/15.0, Scalar::all(0),1.5,8,false);
                cout<<k;
                int distanceValue = 0;
                for (int k = 0; k<toLetterFactor; k++) {
                    for (int l = 0; l<toLetterFactor; l++) {
                        Scalar pixel = img.at<uchar>(k,l);
                        uchar intensity = pixel.val[0];
                        distanceValue+=distanceMap[k][l]*(255-(int)intensity);
                    }
                }
                if(distanceValue>minDistance)
                {
                    minDistance=distanceValue;
                    replacementCharacter=k;
                }
                //namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
                //imshow( "Display window", letterImage);
            }
            cout<<"replacement character is->"<<replacementCharacter<<"<-\n";
            asciiArray[i%toLetterFactor][j%toLetterFactor]=replacementCharacter;
            
        }
    }
    
}



void printAsciiImage(Mat &img, char**asciiArray, int toLetterFactor, int toImageFactor)
{
    //int r = 1;
    int r = img.rows/toLetterFactor;
    //int c = 256;
    int c = img.cols/toLetterFactor;
    int row = toImageFactor*r+10;
    int col = toImageFactor*c;
    Mat asciiImg(row,col,CV_8UC1, Scalar(255));
    Mat asciiImageSmall;
    stringToMat(asciiImg, asciiArray,r,c,toImageFactor);
    resize(asciiImg, asciiImageSmall, Size(), .7, .7, INTER_LINEAR);
    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", asciiImageSmall );
    namedWindow( "Display window2", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window2", img );
    waitKey(0);
}


//void printAsciiImage(Mat &img, char**asciiArray)
//{
//    char t = ' ';
//    int r = 1;
//    //int r = img.rows;
//    int c = 1;
//    //int c = img.cols;
//    int row = 15*r;
//    int col = 15*c;
//    Mat asciiImg(row,col,CV_8UC1, Scalar(255));
//    Mat asciiImageSmall;
//        char *temp = &t;
//        char **as = &temp;
//    stringToMat(asciiImg, as,r,c);
//    //resize(asciiImg, asciiImageSmall, Size(), 1, 1, INTER_LINEAR);
//        string name = "Display window";
//    namedWindow( name+t, WINDOW_AUTOSIZE );// Create a window for display.
//    imshow( name+t, asciiImg );
////    namedWindow( "Display window2", WINDOW_AUTOSIZE );// Create a window for display.
////    imshow( "Display window2", img );
//   {char t = ']';
//    int r = 1;
//    //int r = img.rows;
//    int c = 1;
//    //int c = img.cols;
//    int row = 15*r;
//    int col = 15*c;
//    Mat asciiImg(row,col,CV_8UC1, Scalar(255));
//    Mat asciiImageSmall;
//    char *temp = &t;
//    char **as = &temp;
//    stringToMat(asciiImg, as,r,c);
//    //resize(asciiImg, asciiImageSmall, Size(), 1, 1, INTER_LINEAR);
//    string name = "Display window";
//    namedWindow( name+t, WINDOW_AUTOSIZE );// Create a window for display.
//    imshow( name+t, asciiImg );
//    //    namedWindow( "Display window2", WINDOW_AUTOSIZE );// Create a window for display.
//    //    imshow( "Display window2", img );
//    waitKey(0);
//    }
//}

#endif
