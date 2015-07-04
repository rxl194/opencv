
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <boost/filesystem.hpp>

using namespace cv;
using namespace std;
using namespace boost::filesystem;

class calibrator {
private:
  string d_path;
  vector<Mat> d_images;
  Mat d_cameraMatrix, d_distCoeffs;
  bool d_show_chess_corners;
  float d_side_length;
  int d_width, d_height;
  vector<vector<Point2f> > d_image_points;
  vector<vector<Point3f> > d_object_points;

public:
  calibrator (string, float, int, int);
  void calibrate ();
  Mat get_cameraMatrix ();
  Mat get_distCoeffs ();
  void calc_image_points (bool);
};

calibrator::calibrator (string _path, float _side_length, int _width, int _height)
{
  d_side_length = _side_length;
  d_width = _width;
  d_height = _height;

  d_path = _path;
  cout << d_path << endl;

  // Read images
  for (directory_iterator i (d_path), end_iter; i != end_iter; i++) {
    string filename = d_path + i->path ().filename ().string ();
    d_images.push_back (imread (filename));
  }
}

void calibrator::calc_image_points (bool show)
{
  // Calculate the object points in the object co-ordinate system 
  // (origin at top left corner)
  vector<Point3f> ob_p;
  for (int i = 0; i < d_height; i++) {
    for (int j = 0; j < d_width; j++) {
      ob_p.push_back (Point3f (j*d_side_length, i*d_side_length, 0.f));
    }
  }

  if (show)
    namedWindow ("Chessboard corners");

  for (unsigned int i = 0; i < d_images.size (); i++) {
    Mat im = d_images[i];
    vector<Point2f> im_p;
    // find corners in the chessboard image
    bool pattern_found = findChessboardCorners (im, Size (d_width, d_height), im_p,
            CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK);
    if (pattern_found) {
      d_object_points.push_back (ob_p);
      Mat gray;
      cvtColor (im, gray, CV_BGR2GRAY);
      cornerSubPix (gray, im_p, Size (5, 5), Size (-1, -1),
              TermCriteria (CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
      d_image_points.push_back (im_p);
      if (show) {
        Mat im_show = im.clone ();
        drawChessboardCorners (im_show, Size (d_width, d_height), im_p, true);
        imshow ("Chessboard corners", im_show);
        while (char(waitKey (1)) != ' ') {
        }
      }
    }
      // if a valid pattern was not found, delete the entry from vector of images
    else
      d_images.erase (d_images.begin () + i);
  }
}

void calibrator::calibrate ()
{
  vector<Mat> rvecs, tvecs;
  float rms_error = calibrateCamera (d_object_points, d_image_points,
          d_images[0].size (), d_cameraMatrix, d_distCoeffs, rvecs, tvecs);
  cout << "RMS reprojection error " << rms_error << endl;
}

Mat calibrator::get_cameraMatrix ()
{
  return d_cameraMatrix;
}

Mat calibrator::get_distCoeffs ()
{
  return d_distCoeffs;
}

int main (int argc, char** argv)
{
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " <Image_directory> <data_directory>" << endl;
    return -1;
  }

  calibrator calib (argv[1], 25.f, 5, 4);

  calib.calc_image_points (true);
  cout << "Calibrating camera..." << endl;
  calib.calibrate ();
  // save the calibration for future use
  string filename = argv[2] + string ("cam_calib.xml");
  FileStorage fs (filename, FileStorage::WRITE);
  fs << "cameraMatrix" << calib.get_cameraMatrix ();
  fs << "distCoeffs" << calib.get_distCoeffs ();
  fs.release ();
  cout << "Saved calibration matrices to " << filename << endl;
  return 0;
}
