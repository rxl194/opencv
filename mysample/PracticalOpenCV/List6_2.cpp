// Program to find smallest closed contour in image encosing a clicked point
// Author: Samarth Manoj Brahmbhatt, University of Pennsylvania

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

vector<vector<Point> > g_contours;
vector<Vec4i> g_heirarchy;
Mat img_all_contours;

// Function to make the contours closed
static void make_contours_closed(vector<vector<Point> > contours) {
    for(unsigned int i = 0; i < contours.size(); i++) {
        vector<Point> cc;
        approxPolyDP(contours[i], cc, 0.1, true);
        contours[i] = cc;
    }
}

// Function to find the index of smalledst enclosing contour in 'contours'
static int smallest_contour(Point p, vector<vector<Point> > contours, vector<Vec4i> heirarchy) {
    int idx = 0, prev_idx = -1;
    while(idx >= 0) {
        vector<Point> c = contours[idx];
        // Point polygon test
        double d = pointPolygonTest(c, p, false);
        // If point is inside the contour, move to its child... 
        if(d > 0) {
            prev_idx = idx;
            idx = heirarchy[idx][2];
        }
        //...else check the next cotour at the same level
        else idx = heirarchy[idx][0];
    }

    return prev_idx;
}

// Mouse callback
static void on_mouse(int event, int x, int y, int, void *) {
    if(event != EVENT_LBUTTONDOWN) return;
    Point p(x, y);

    int idx = smallest_contour(p, g_contours, g_heirarchy);
    
    // If function returned a valid contour index, draw it using a thick red line
    if(idx > 0) {
        vector<vector<Point> > contour_show(1, g_contours[idx]);
        Mat img_show = img_all_contours.clone();
        drawContours(img_show, contour_show, -1, Scalar(0, 0, 255), 3);
        imshow("Contours", img_show);
    }
    else
        imshow("Contours", img_all_contours);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf ("Usage: %s <Image_file_name>\n", argv[0]);
    return -1;
  }
    Mat img = imread(argv[1]);
    Mat edges;
    Canny(img, edges, 50, 100);
    findContours(edges, g_contours, g_heirarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
    // Make the contours closed
    make_contours_closed(g_contours);
    img_all_contours = img.clone();
    // Draw all contours using a thin green line
    drawContours(img_all_contours, g_contours, -1, Scalar(0, 255, 0));

    namedWindow("Contours", CV_WINDOW_AUTOSIZE);
    imshow("Contours", img_all_contours);

    setMouseCallback("Contours", on_mouse);

    while(char(waitKey(1)) != 'q') {}

    return 0;
}
