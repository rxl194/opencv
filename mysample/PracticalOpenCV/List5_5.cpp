
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

Mat edges;
int slider = 50;

static void on_slider (int, void*) 
{
  if (!edges.empty()) {
    Mat edges_thresholded;
    threshold (edges, edges_thresholded, slider, 255, THRESH_TOZERO);
    imshow ("Thresholded Scharr edges", edges_thresholded);
  }
}

int main (int argc, char** argv)
{
  if (argc != 2) {
    printf ("Usage: %s <Image_file_name>\n", argv[0]);
    return -1;
  }

  Mat image = imread (argv[1]), image_blurred, image_gray;
  if (!image.data) {
    printf ("No image data\n");
    return -1;
  }

  const string origWinTitle ("Original image"),
    scharrWinTitle ("Thresholded Scharr edges");
  namedWindow (origWinTitle);
  namedWindow (scharrWinTitle);

  GaussianBlur (image, image_blurred, Size(3, 3), 0, 0);
  cvtColor (image_blurred, image_gray, CV_BGR2GRAY);

  Mat grad_x, grad_y;
  Scharr (image_gray, grad_x, CV_32F, 1, 0);
  Scharr (image_gray, grad_y, CV_32F, 0, 1);

  pow (grad_x, 2, grad_x);
  pow (grad_y, 2, grad_y);

  Mat grad = grad_x + grad_y;
  sqrt (grad, grad);

  Mat edges_thresholded;
  grad.convertTo (edges, CV_8U);
  threshold (edges, edges_thresholded, slider, 255, THRESH_TOZERO);

  imshow (origWinTitle, image);
  imshow (scharrWinTitle, edges_thresholded);

  createTrackbar ("Threshold", "Thresholded Scharr edges", &slider, 255, on_slider);

  while (char(waitKey(1) != 'q')) {}
  return 0;
}
