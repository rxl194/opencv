
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

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
    scharrWinTitle ("Scharr edges");
  namedWindow (origWinTitle);
  namedWindow (scharrWinTitle);

  GaussianBlur (image, image_blurred, Size(3, 3), 0, 0);
  cvtColor (image_blurred, image_gray, CV_RGB2GRAY);

  Mat grad_x, grad_y;
  Scharr (image_gray, grad_x, CV_32F, 1, 0);
  Scharr (image_gray, grad_y, CV_32F, 0, 1);

  pow (grad_x, 2, grad_x);
  pow (grad_y, 2, grad_y);

  Mat grad = grad_x + grad_y;
  sqrt (grad, grad);

  Mat edges;
  grad.convertTo (edges, CV_8U);

  imshow (origWinTitle, image);
  imshow (scharrWinTitle, edges);

  while (char(waitKey(1) != 'q')) {}
  return 0;
}
