
#include <stdio.h>
#include <opencv2/opencv.hpp>

int main (int argc, char** argv)
{
  if (argc != 2) {
    printf ("Usage: %s <Image_file_name>\n", argv[0]);
    return -1;
  }

  cv::Mat image= cv::imread (argv[1], 1);

  if (!image.data) {
    printf ("No image data\n");
    return -1;
  }

  const std::string winTitle ("Display Image");
  cv::namedWindow (winTitle, CV_WINDOW_AUTOSIZE);
  cv::imshow (winTitle, image);

  cv::waitKey(0);
  return 0;
}
