
#include <opencv2/opencv.hpp>
#include <opencv2/stitching/stitcher.hpp>
#include <opencv2/stitching/warpers.hpp>
#include "Config.h"

using namespace cv;
using namespace std;

int main ()
{
  Mat im = imread (IMAGE_FOLDER + string ("/image.jpg")), im_transformed;
  imshow ("Original", im);

  int rotation_degrees = 30;

  Mat M = getRotationMatrix2D (Point (im.cols / 2, im.rows / 2), rotation_degrees, 1);
  cout << M << endl;

  warpAffine (im, im_transformed, M, im.size (), INTER_LINEAR);

  imshow ("Transformed", im_transformed);

  while (char(waitKey (1)) != 'q') {
  }

  return 0;

}

