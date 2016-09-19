
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main (int argc, char** argv)
{
  if (argc != 2) {
    printf ("Usage: %s <Image_file_name>\n", argv[0]);
    return -1;
  }

  Mat image = imread (argv[1], CV_LOAD_IMAGE_GRAYSCALE), img_filtered;
  if (!image.data) {
    printf ("No image data\n");
    return -1;
  }

  float vertical_fk[5][5] = {{0,0,0,0,0}, {0,0,0,0,0}, {-1,-2,6,-2,-1},
			     {0,0,0,0,0}, {0,0,0,0,0}};
  float horizontal_fk[5][5] = {{0,0,-1,0,0}, {0,0,-2,0,0}, {0,0,6,0,0},
			       {0,0,-2,0,0}, {0,0,-1,0,0}};

  Mat filter_kernel = Mat (5, 5, CV_32FC1, horizontal_fk);

  filter2D (image, img_filtered, -1, filter_kernel);

  const string winTitle ("Display Image"), filteredTitle ("Filtered Image");
  namedWindow (winTitle, CV_WINDOW_AUTOSIZE);
  namedWindow (filteredTitle, CV_WINDOW_AUTOSIZE);
  imshow (winTitle, image);
  imshow (filteredTitle, img_filtered);

  waitKey(0);
  return 0;
}
