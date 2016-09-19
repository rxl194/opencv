
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

  for (directory_iterator i (argv[1]), end_iter; i != end_iter; i++) {
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
  stitcher.stitch (images, panorama);

  namedWindow ("Panorama", CV_WINDOW_NORMAL);
  imshow ("Panorama", panorama);

  while (char(waitKey (1)) != 'q') {
  }

  return 0;

}