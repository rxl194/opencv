
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

int main ()
{
  vector< Mat > vImg;
  Mat rImg;

  cout << "Image Folder " << IMAGE_FOLDER << endl;
  vImg.push_back (imread (IMAGE_FOLDER + string ("/S1.jpg")));
  vImg.push_back (imread (IMAGE_FOLDER + string("/S2.jpg")));
  vImg.push_back (imread (IMAGE_FOLDER + string("/S3.jpg")));
//  vImg.push_back (imread (IMAGE_FOLDER + string("/S4.jpg")));
  vImg.push_back (imread (IMAGE_FOLDER + string("/S5.jpg")));
  vImg.push_back (imread (IMAGE_FOLDER + string("/S6.jpg")));


  Stitcher stitcher = Stitcher::createDefault ();


  unsigned long AAtime = 0, BBtime = 0; //check processing time  
  AAtime = getTickCount (); //check processing time  

  stitcher.stitch (vImg, rImg);

  BBtime = getTickCount (); //check processing time   
  printf ("%.2lf sec \n", (BBtime - AAtime) / getTickFrequency ()); //check processing time  

  imshow ("Stitching Result", rImg);

  waitKey (0);

} 