
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

Mat image_gray, image;
int max_corners = 20;
const string trackBarTitle1 = "Max no. of corners";
const string winTitle = "Corners";

static void display_slider_pos (const int nofCorners)
{
  if (image_gray.empty()) return;

  float quality = 0.01;
  int min_distance = 10;

  vector<Point2d> corners;

  goodFeaturesToTrack (image_gray, corners, nofCorners, quality, 
		       min_distance);

  Mat image_corners = image.clone();
  for (unsigned int i = 0; i < corners.size(); i++) {
    circle (image_corners, corners[i], 4, CV_RGB(255, 0, 0), -1);
  }
  imshow (winTitle, image_corners);
}

static void on_slider (int, void*)
{
  max_corners = max (1, max_corners);
  setTrackbarPos (trackBarTitle1, winTitle, max_corners);
  display_slider_pos( max_corners );
}

int main (int argc, char** argv)
{
  if (argc < 2) {
    printf ("Usage: %s <Image_file_name>\n", argv[0]);
    return -1;
  }

  image = imread (argv[1]);
  if (!image.data) {
    printf ("No image data\n");
    return -1;
  }

  cvtColor (image, image_gray, CV_RGB2GRAY);
  if (image_gray.empty()) {
    printf ("No image gray data\n");
    return -1;
  }

  namedWindow (winTitle);

  display_slider_pos (max_corners);

  createTrackbar (trackBarTitle1, winTitle, &max_corners, 250, on_slider);

  while (char(waitKey(1) != 'q')) {}
  return 0;
}
