
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

Mat image, image_blurred;
int slider = 5;

static float get_sigma(const int size)
{
  return 0.3 * ((size-1) * 0.5 - 1) + 0.8;
}

static void on_trackbar (int, void*)
{
  int k_size = max(1, slider);
  k_size = (k_size % 2 == 0)? k_size + 1 : k_size;
  setTrackbarPos ("Kernel Size", "Blurred image", k_size);
  float sigma = get_sigma (k_size);
  GaussianBlur (image, image_blurred, Size (k_size, k_size), sigma);
  imshow ("Blurred image", image_blurred);
}

int main (int argc, char** argv)
{
  if (argc != 2) {
    printf ("Usage: %s <Image_file_name>\n", argv[0]);
    return -1;
  }

  image = imread (argv[1]);
  if (!image.data) {
    printf ("No image data\n");
    return -1;
  }

  const string origWinTitle ("Original image"),
    blurredWinTitle ("Blurred image");
  namedWindow (origWinTitle);
  namedWindow (blurredWinTitle);

  imshow (origWinTitle, image);
  float sigma = get_sigma (slider);
  GaussianBlur (image, image_blurred, Size(slider, slider), sigma);
  imshow (blurredWinTitle, image_blurred);

  createTrackbar ("Kernel Size", "Blurred image", &slider, 21, on_trackbar);

  while (char(waitKey(1) != 'q')) {}
  return 0;
}
