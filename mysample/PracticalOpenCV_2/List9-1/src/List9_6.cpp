
#include <opencv2/opencv.hpp>
#include <opencv2/stitching/stitcher.hpp>
#include <opencv2/stitching/warpers.hpp>
#include <boost/filesystem.hpp>

using namespace std;
using namespace cv;
using namespace boost::filesystem;

int main (int argc, char** argv)
{
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " <Image_files_directory>" << endl;
    return -1;
  }

  vector<Mat> images;

  // read images
  path p(argv[1]);
  for (directory_iterator i (p), end_iter; i != end_iter; i++) {
    cout << " argv = " << argv[1] << endl;
    cout << " i->path = " << i->path() << endl;
    cout << " i->path().filename() = " << i->path().filename ().string() << endl;
    string im_name = i->path ().filename ().string ();
    string filename = string (argv[1]) + im_name;
    Mat im = imread (filename);
    if (!im.empty ())
      images.push_back (im);
  }

  cout << "Read " << images.size () << " images" << endl;
  cout << "Now making panorama..." << endl;

  Mat panorama;

  Stitcher stitcher = Stitcher::createDefault ();
  CylindricalWarper *warper = new CylindricalWarper ();
  stitcher.setWarper (warper);

  // Estimate perspective transforms between images
  Stitcher::Status status = stitcher.estimateTransform (images);
  if (status != Stitcher::OK) {
    cout << "Can't stitch images, error code = " << int(status) << endl;
    return -1;
  }

  // Make panorama
  status = stitcher.composePanorama (panorama);
  if (status != Stitcher::OK) {
    cout << "Can't stitch images, error code = " << int(status) << endl;
    return -1;
  }

  namedWindow ("Panorama", CV_WINDOW_NORMAL);
  imshow ("Panorama", panorama);

  while (char(waitKey (1)) != 'q') {
  }

  return 0;
}