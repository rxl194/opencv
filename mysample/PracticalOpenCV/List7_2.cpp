
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

Mat frame_hsv, frame, mask;

int low_diff = 10, high_diff = 10, conn = 4, val = 255,
        flags = conn + (val << 8) + CV_FLOODFILL_MASK_ONLY;
double l_h = 0, h_h = 0, h_s = 0, l_s = 0;

bool selected = false;

static void on_low_diff_trackbar (int, void*) { }

static void on_high_diff_trackbar (int, void*) { }

static void on_mouse (int event, int x, int y, int, void*)
{
  if (event != EVENT_LBUTTONDOWN)
    return;
  selected = true;
  Point p (x, y);
  mask = Scalar::all (0);
  floodFill (frame, mask, p, Scalar (255, 255, 255), 0, Scalar (low_diff, low_diff, low_diff),
             Scalar (high_diff, high_diff, high_diff), flags);

  Mat channels[3];
  split (frame_hsv, channels);
  minMaxLoc (channels[0], &l_h, &h_h, NULL, NULL,
             mask.rowRange (1, mask.rows - 1).colRange (1, mask.cols - 1));
  minMaxLoc (channels[1], &l_s, &h_s, NULL, NULL,
             mask.rowRange (1, mask.rows - 1).colRange (1, mask.cols - 1));
}

int main (int argc, char** argv)
{
  VideoCapture cap;
  if (argc < 2)
    cap.open (0);
  else
    cap.open (argv[1]);

  if (!cap.isOpened ()) {
    cout << "Capture could not be opened successfully" << endl;
    return -1;
  }

  const char *videoWinTitle = "Video";
  const char *segWinTitle = "Segmentation";

  createTrackbar ("Low Diff", segWinTitle, &low_diff, 50, on_low_diff_trackbar);
  createTrackbar ("High Diff", segWinTitle, &high_diff, 50, on_high_diff_trackbar);
  setMouseCallback (videoWinTitle, on_mouse);

  while (char(waitKey (1)) != 'q' && cap.isOpened ()) {
    cap >> frame;
    if (!selected)
      mask.create (frame.rows + 2, frame.cols + 2, CV_8UC1);
    if (frame.empty ()) {
      cout << "Video over" << endl;
      break;
    }
    cvtColor (frame, frame_hsv, CV_BGR2HSV);

    int from_to[] = {0, 0, 1, 1};
    Mat hs (frame.size (), CV_8UC2);
    mixChannels (&frame_hsv, 1, &hs, 1, from_to, 2);

    Mat frame_thresholded;
    inRange (hs, Scalar (l_h, l_s), Scalar (h_h, h_s), frame_thresholded);

    Mat str_el = getStructuringElement (MORPH_RECT, Size (5, 5));
    morphologyEx (frame_thresholded, frame_thresholded, MORPH_OPEN, str_el);
    morphologyEx (frame_thresholded, frame_thresholded, MORPH_CLOSE, str_el);

    imshow (videoWinTitle, frame);
    imshow (segWinTitle, frame_thresholded);
  }

  return 0;
}