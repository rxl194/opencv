// Program to find smallest closed contour in image encosing a clicked point
// Author: Samarth Manoj Brahmbhatt, University of Pennsylvania

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

Mat img;
int shape = 0;
int thresh = 100;

static void on_trackbar (int, void*)
{
  if (shape == 1) {
    Mat img_gray;
    cvtColor (img, img_gray, CV_RGB2GRAY);
    vector<Vec3f> circles;
    HoughCircles (img_gray,circles, CV_HOUGH_GRADIENT, 1, 10, 100, thresh, 5);
    Mat img_show = img.clone();
    for (unsigned int i = 0; i < circles.size(); i++) {
      Point center (cvRound(circles[i][0]), cvRound(circles[i][1]));
      int radius = cvRound(circles[i][2]);
      circle (img_show, center, 3, Scalar(0, 0, 255), -1);
      circle (img_show, center, radius, Scalar(0, 0,255), 3, 8, 0);
    }
    imshow ("Shapes", img_show);
  } else if (shape == 0) {
    Mat edges;
    Canny (img, edges, 50, 100);
    vector<Vec2f> lines;
    HoughLines (edges, lines, 1, CV_PI/180.f, thresh);
    Mat img_show = img.clone();
    for (unsigned int i = 0; i < lines.size(); i++) {
      float rho = lines[i][0];
      float theta = lines[i][1];
      double a = cos(theta), b = sin(theta);
      double x0 = a * rho, y0 = b * rho;
      Point pt1 (cvRound (x0 + 1000 * (-b)), cvRound (y0 + 1000 * (a)));
      Point pt2 (cvRound (x0 - 1000 * (-b)), cvRound (y0 - 1000 * (a)));
      line (img_show, pt1, pt2, Scalar(0, 0, 255));
    }
    imshow ("Shapes", img_show);
  }
}

int main (int argc, char** argv)
{
  if (argc != 2) {
    printf ("Usage: %s <Image_file_name>\n", argv[0]);
    return -1;
  }

  img = imread(argv[1]);

  namedWindow ("Shapes");

  createTrackbar ("Lines/Circles", "Shapes", &shape, 1, on_trackbar);
  createTrackbar ("Acc. Thresh.", "Shapes", &thresh, 300, on_trackbar);

  on_trackbar (0, 0);

  while (char(waitKey(1)) != 'q') {}

  return 0;
}


