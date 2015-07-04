
#include <opencv2/opencv.hpp>
#include <opencv2/stitching/stitcher.hpp>
#include <opencv2/stitching/warpers.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>

using namespace std;
using namespace cv;

static void on_mouse (int event, int x, int y, int, void* _p)
{
  Point2f *p = (Point2f *) _p;
  if (event == CV_EVENT_LBUTTONUP) {
    p->x = x;
    p->y = y;
  }
}

class perspective_transformer {
private:
  Mat im, im_transformed, im_perspective_transformed, im_show, im_transformed_show;
  vector<Point2f> points, points_transformed;
  Mat M;
  Point2f get_click (string, Mat);
public:
  perspective_transformer (const string& img);
  void estimate_perspective ();
  void show_diff ();
};

perspective_transformer::perspective_transformer (const string& img)
{
  im = imread (img);
  im_transformed = imread ("transformed.jpg");
}

Point2f perspective_transformer::get_click (string window_name, Mat _im)
{
  Point2f p (-1, -1);
  setMouseCallback (window_name, on_mouse, (void *) &p);
  while (p.x == -1 && p.y == -1) {
    imshow (window_name, _im);
    waitKey (20);
  }
  return p;
}

void perspective_transformer::estimate_perspective ()
{
  const string oriWinTitle ("Original");
  const string transformedWinTitle ("Transformed");

  imshow (oriWinTitle, im);
  imshow (transformedWinTitle, im_transformed);

  cout << "To estimate the Perspective transform between the original and transformed images you will have to click on 8 matching pairs of points" << endl;

  im_show = im.clone ();
  im_transformed_show = im_transformed.clone ();
  Point2f p;
  for (int i = 0; i < 8; i++) {
    cout << "Click on a distinguished point in the ORIGINAL image" << endl;
    p = get_click (oriWinTitle, im_show);
    cout << p << endl;
    points.push_back (p);
    circle (im_show, p, 2, Scalar (0, 0, 255), -1);
    imshow (oriWinTitle, im_show);

    cout << "Click on a distinguished point in the TRANSFORMED image" << endl;
    p = get_click (transformedWinTitle, im_transformed_show);
    points_transformed.push_back (p);
    circle (im_transformed_show, p, 2, Scalar (0, 0, 255), -1);
    imshow (transformedWinTitle, im_transformed_show);
  }

  // Estimate perspective transform
  M = findHomography (points, points_transformed, CV_RANSAC, 2);
  cout << "Estimated Perspective transform = " << M << endl;

  // Apply estimated perspective transform
  warpPerspective (im, im_perspective_transformed, M, im.size ());
  imshow ("Estimated Perspective transform", im_perspective_transformed);
}

void perspective_transformer::show_diff ()
{
  imshow ("Difference", im_transformed - im_perspective_transformed);
}

int main (int argc, char** argv)
{
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " <Image_file_name>" << endl;
    return -1;
  }
  perspective_transformer a (argv[1]);
  a.estimate_perspective ();
  cout << "Press 'd' to show difference, 'q' to end" << endl;
  if (char(waitKey (-1)) == 'd') {
    a.show_diff ();
    cout << "Press 'q' to end" << endl;
    if (char(waitKey (-1)) == 'q')
      return 0;
  }
  else
    return 0;
}