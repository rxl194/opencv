
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

Mat img;
vector<vector<Point> > contours;
vector<Vec4i> heirarchy;

const string contoursWinTitle = "Contours";

int levels = 0;

static void on_trackbar (int, void *) 
{
  if (contours.empty()) return;

  Mat img_show = img.clone();

  drawContours (img_show, contours, -1, Scalar(0, 0, 255), 3, 8,
		heirarchy, levels);
  imshow(contoursWinTitle, img_show);
}

int main (int argc, char** argv)
{
  if (argc != 2) {
    printf ("Usage: %s <Image_file_name>\n", argv[0]);
    return -1;
  }

  img = imread(argv[1]);

  Mat img_b;
  cvtColor (img, img_b, CV_RGB2GRAY);

  Mat edges;
  Canny (img_b, edges, 50, 100);

  findContours (edges, contours, heirarchy, CV_RETR_TREE, 
		CV_CHAIN_APPROX_NONE);

  namedWindow (contoursWinTitle);
  createTrackbar ("levels", contoursWinTitle, &levels, 15, on_trackbar);

  on_trackbar(0,0);

  while (char(waitKey(1)) != 'q') {}
  return 0;
}

