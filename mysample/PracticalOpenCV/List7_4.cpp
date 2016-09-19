
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

Mat image, image_eq;
int choice = 0;
const char *imageWinTitle = "Image";

static void on_trackbar (int, void*)
{
  if (choice == 0)
    imshow (imageWinTitle, image);
  else
    imshow (imageWinTitle, image_eq);
}

int main (int argc, char** argv)
{
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " <Image_file_name>" << endl;
    return -1;
  }
  image = imread (argv[1]);
  image_eq.create (image.rows, image.cols, CV_8UC3);

  vector<Mat> channels, channels_eq;
  split (image, channels);

  for (unsigned int i = 0; i < channels.size (); i++) {
    Mat eq;
    equalizeHist (channels[i], eq);
    channels_eq.push_back (eq);
  }

  merge (channels_eq, image_eq);
  namedWindow (imageWinTitle);

  createTrackbar ("Normal/Eq.", "Image", &choice, 1, on_trackbar);
  on_trackbar (0, 0);

  while (char(waitKey (1)) != 'q') {
  }
  return 0;
}