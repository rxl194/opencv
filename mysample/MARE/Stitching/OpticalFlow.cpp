

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/stitching/stitcher.hpp>  
#include "Config.h"

#ifdef _DEBUG    
#pragma comment(lib, "opencv_core246d.lib")     
#pragma comment(lib, "opencv_imgproc246d.lib")   //MAT processing    
#pragma comment(lib, "opencv_highgui246d.lib")    
#pragma comment(lib, "opencv_stitching246d.lib");  

#else    
#pragma comment(lib, "opencv_core246.lib")    
#pragma comment(lib, "opencv_imgproc246.lib")    
#pragma comment(lib, "opencv_highgui246.lib")    
#pragma comment(lib, "opencv_stitching246.lib");  
#endif    

using namespace cv;
using namespace std;


#define MAX_COUNT 250     
#define DELAY_T 20     
#define PI 3.1415     

int main ()
{

  //////////////////////////////////////////////////////////////////////////     
  //image class           
  IplImage* image = 0;

  //T, T-1 image     
  IplImage* current_Img = 0;
  IplImage* Old_Img = 0;

  //Optical Image     
  IplImage * imgA = 0;
  IplImage * imgB = 0;


  //Video Load     
  CvCapture * capture = cvCreateFileCapture ("1.avi"); //cvCaptureFromCAM(0); //cvCreateFileCapture("1.avi");     

  //Window     
  cvNamedWindow ("Origin");
  //////////////////////////////////////////////////////////////////////////     


  //////////////////////////////////////////////////////////////////////////      
  //Optical Flow Variables      
  IplImage * eig_image = 0;
  IplImage * tmp_image = 0;
  int corner_count = MAX_COUNT;
  CvPoint2D32f* cornersA = new CvPoint2D32f[ MAX_COUNT ];
  CvPoint2D32f * cornersB = new CvPoint2D32f[ MAX_COUNT ];

  CvSize img_sz;
  int win_size = 20;

  IplImage* pyrA = 0;
  IplImage* pyrB = 0;

  char features_found[ MAX_COUNT ];
  float feature_errors[ MAX_COUNT ];
  //////////////////////////////////////////////////////////////////////////     


  //////////////////////////////////////////////////////////////////////////     
  //Variables for time different video     
  int one_zero = 0;
  int t_delay = 0;



  //Routine Start     
  while (1) {


    //capture a frame form cam        
    if (cvGrabFrame (capture) == 0)
      break;

    //Image Create     
    if (Old_Img == 0) {
      image = cvRetrieveFrame (capture);
      current_Img = cvCreateImage (cvSize (image->width, image->height), image->depth, image->nChannels);
      Old_Img = cvCreateImage (cvSize (image->width, image->height), image->depth, image->nChannels);



    }



    if (one_zero == 0) {
      if (eig_image == 0) {
        eig_image = cvCreateImage (cvSize (image->width, image->height), image->depth, image->nChannels);
        tmp_image = cvCreateImage (cvSize (image->width, image->height), image->depth, image->nChannels);
      }

      //copy to image class     
      memcpy (Old_Img->imageData, current_Img->imageData, sizeof (char)*image->imageSize);
      image = cvRetrieveFrame (capture);
      memcpy (current_Img->imageData, image->imageData, sizeof (char)*image->imageSize);

      //////////////////////////////////////////////////////////////////////////     
      //Create image for Optical flow     
      if (imgA == 0) {
        imgA = cvCreateImage (cvSize (image->width, image->height), IPL_DEPTH_8U, 1);
        imgB = cvCreateImage (cvSize (image->width, image->height), IPL_DEPTH_8U, 1);
      }

      //RGB to Gray for Optical Flow     
      cvCvtColor (current_Img, imgA, CV_BGR2GRAY);
      cvCvtColor (Old_Img, imgB, CV_BGR2GRAY);

      //     
      cvGoodFeaturesToTrack (imgA, eig_image, tmp_image, cornersA, &corner_count, 0.01, 5.0, 0, 3, 0, 0.04);
      cvFindCornerSubPix (imgA, cornersA, corner_count, cvSize (win_size, win_size), cvSize (-1, -1), cvTermCriteria (CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03));


      CvSize pyr_sz = cvSize (imgA->width + 8, imgB->height / 3);
      if (pyrA == 0) {
        pyrA = cvCreateImage (pyr_sz, IPL_DEPTH_32F, 1);
        pyrB = cvCreateImage (pyr_sz, IPL_DEPTH_32F, 1);
      }

      cvCalcOpticalFlowPyrLK (imgA, imgB, pyrA, pyrB, cornersA, cornersB, corner_count, cvSize (win_size, win_size), 5, features_found, feature_errors, cvTermCriteria (CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.3), 0);

      /////////////////////////////////////////////////////////////////////////        

      for (int i = 0; i < corner_count; ++i) {

        if (features_found[i] == 0 || feature_errors[i] > MAX_COUNT)
          continue;



        //////////////////////////////////////////////////////////////////////////         
        //Vector Length     
        float fVecLength = sqrt ((float) ((cornersA[i].x - cornersB[i].x)*(cornersA[i].x - cornersB[i].x)+(cornersA[i].y - cornersB[i].y)*(cornersA[i].y - cornersB[i].y)));
        //Vector Angle     
        float fVecSetha = fabs (atan2 ((float) (cornersB[i].y - cornersA[i].y), (float) (cornersB[i].x - cornersA[i].x)) * 180 / PI);

        cvLine (image, cvPoint (cornersA[i].x, cornersA[i].y), cvPoint (cornersB[i].x, cornersA[i].y), CV_RGB (0, 255, 0), 2);

        printf ("[%d] - Sheta:%lf, Length:%lf\n", i, fVecSetha, fVecLength);
      }


      //////////////////////////////////////////////////////////////////////////         

    }
    cvShowImage ("Origin", image);

    //////////////////////////////////////////////////////////////////////////     

    //time delay     
    one_zero++;
    if ((one_zero % DELAY_T) == 0) {
      one_zero = 0;
    }

    //break        
    if (cvWaitKey (10) >= 0)
      break;
  }

  //release capture point        
  cvReleaseCapture (&capture);
  //close the window        
  cvDestroyWindow ("Origin");

  cvReleaseImage (&Old_Img);
  //////////////////////////////////////////////////////////////////////////     
  cvReleaseImage (&imgA);
  cvReleaseImage (&imgB);
  cvReleaseImage (&eig_image);
  cvReleaseImage (&tmp_image);
  delete cornersA;
  delete cornersB;
  cvReleaseImage (&pyrA);
  cvReleaseImage (&pyrB);


  //////////////////////////////////////////////////////////////////////////     
} 