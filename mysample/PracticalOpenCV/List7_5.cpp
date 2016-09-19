
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

Mat frame_hsv, frame, mask;
MatND hist;
int conn = 4, val = 255, flags = conn + (val << 8) + CV_FLOODFILL_MASK_ONLY;

bool selected = false;

float hrange[] = {0, 179}, srange[] = {0, 255};
const float *ranges[] = {hrange, srange};

static void on_mouse (int event, int x, int y, int, void*)
{
  if (event != EVENT_LBUTTONDOWN)
    return;

  selected = true;
  Point p (x, y);
  floodFill (frame, mask, p, Scalar (255, 255, 255), 0, Scalar (10, 10, 10), Scalar (10, 10, 10), flags);
  Mat _mask = mask.rowRange (1, mask.rows - 1).colRange (1, mask.cols - 1);

  int histSize[] = {50, 50}, channels[] = {0, 1};

  calcHist (&frame_hsv, 1, channels, _mask, hist, 2, histSize, ranges);
  normalize (hist, hist, 0, 255, NORM_MINMAX, -1, Mat ());
}

int main (int argc, char** argv)
{
  const char *videoWinTitle = "Video", *backProjWinTitle = "Backprojection";

  VideoCapture cap;
  if (argc < 2)
    cap.open (0);
  else
    cap.open (argv[1]);

  if (!cap.isOpened ()) {
    cout << "Capture could not be opened successfully" << endl;
    return -1;
  }

  namedWindow (videoWinTitle);
  namedWindow (backProjWinTitle);

  setMouseCallback (videoWinTitle, on_mouse);

  while (char(waitKey (1)) != 'q' && cap.isOpened ()) {
    cap >> frame;
    if (!selected) mask.create (frame.rows + 2, frame.cols + 2, CV_8UC1);

    if (frame.empty ()) {
      cout << "Video over" << endl;
      break;
    }

    cvtColor (frame, frame_hsv, CV_BGR2HSV);

    Mat frame_backprojected = Mat::zeros (frame.size (), CV_8UC1);
    if (selected) {
      int channels[] = {0, 1};
      calcBackProject (&frame_hsv, 1, channels, hist, frame_backprojected, ranges);
    }
    imshow (videoWinTitle, frame);
    imshow (backProjWinTitle, frame_backprojected);
  }

  return 0;

}