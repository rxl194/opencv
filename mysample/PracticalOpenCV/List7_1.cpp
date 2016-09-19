
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int hs_slider = 0, low_slider = 30, high_slider = 100;
int low_h = 30, low_s = 30, high_h = 100, high_s = 100;

const char *lowThresWinTitle = "Low threshold";
const char *highThresWinTitle = "High threshold";
const char *segmentWinTitle = "Segmentation";

static void on_hs_trackbar (int, void*)
{
  switch (hs_slider) {
  case 0:
    setTrackbarPos (lowThresWinTitle, segmentWinTitle, low_h);
    setTrackbarPos (highThresWinTitle, segmentWinTitle, high_h);
    break;
  case 1:
    setTrackbarPos (lowThresWinTitle, segmentWinTitle, low_s);
    setTrackbarPos (highThresWinTitle, segmentWinTitle, high_s);
    break;
  }
}

static void on_low_thresh_trackbar (int, void*)
{
  switch (hs_slider) {
  case 0:
    low_h = min (high_slider - 1, low_slider);
    setTrackbarPos (lowThresWinTitle, segmentWinTitle, low_h);
    break;
  case 1:
    low_s = min (high_slider - 1, low_slider);
    setTrackbarPos (lowThresWinTitle, segmentWinTitle, low_s);
    break;
  }
}

static void on_high_thresh_trackbar (int, void *)
{
  switch (hs_slider) {
  case 0:
    high_h = max (low_slider + 1, high_slider);
    setTrackbarPos (highThresWinTitle, segmentWinTitle, high_h);
    break;
  case 1:
    high_s = max (low_slider + 1, high_slider);
    setTrackbarPos (highThresWinTitle, segmentWinTitle, high_s);
    break;
  }
}

int main ()
{
  VideoCapture cap (0);
  if (!cap.isOpened ()) {
    cout << "Capture could not be opened successfully" << endl;
    return -1;
  }

  const char *videoWinTitle = "Video";
  namedWindow (videoWinTitle);
  namedWindow (segmentWinTitle);

  createTrackbar ("0. H\n1. S", segmentWinTitle, &hs_slider, 1,
                  on_hs_trackbar);
  createTrackbar ("Low threshold", segmentWinTitle, &low_slider, 255,
                  on_low_thresh_trackbar);
  createTrackbar ("High threshold", segmentWinTitle, &high_slider, 255,
                  on_high_thresh_trackbar);

  while (char(waitKey (1)) != 'q' && cap.isOpened ()) {
    Mat frame, frame_thresholded, frame_hsv;
    cap >> frame;
    cvtColor (frame, frame_hsv, CV_BGR2HSV);
    if (frame.empty ()) {
      cout << "Video over" << endl;
      break;
    }
    int from_to[] = {0, 0, 1, 1};
    Mat hs (frame.size (), CV_8UC2);
    mixChannels (&frame_hsv, 1, &hs, 1, from_to, 2);

    inRange (hs, Scalar (low_h, low_s), Scalar (high_h, high_s), frame_thresholded);

    Mat str_el = getStructuringElement (MORPH_ELLIPSE, Size (7, 7));
    morphologyEx (frame_thresholded, frame_thresholded, MORPH_OPEN, str_el);
    morphologyEx (frame_thresholded, frame_thresholded, MORPH_CLOSE, str_el);

    imshow (videoWinTitle, frame);
    imshow (segmentWinTitle, frame_thresholded);
  }

  return 0;
}
