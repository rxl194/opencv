sudo apt-get update && sudo apt-get -y upgrade
sudo apt-get -y dist-upgrade && sudo apt-get -y autoremove

# build developer tools. Some of these are probably non-pertinent
sudo apt-get install -y git-core curl zlib1g-dev build-essential \
     libssl-dev libreadline-dev libyaml-dev libsqlite3-dev \
     libxml2-dev libxslt1-dev libcurl4-openssl-dev \
     python-software-properties

# numpy is a dependency for OpenCV, so most of these other
# packages are probably optional
sudo apt-get install -y python-numpy python-scipy python-matplotlib ipython ipython-notebook python-pandas python-sympy python-nose
sudo apt-get install python-pip python-dev
## Other scientific libraries (obviously not needed for OpenCV)
sudo pip install -U scikit-learn
sudo pip install -U nltk

### opencv from source
# first, installing some utilities
sudo apt-get install -y qt-sdk unzip
sudo apt-get install libboost-all-dev

OPENCV_VER=2.4.10
curl "http://fossies.org/linux/misc/opencv-${OPENCV_VER}.zip" -o opencv-${OPENCV_VER}.zip
unzip "opencv-${OPENCV_VER}.zip" && cd "opencv-${OPENCV_VER}"
mkdir build && cd build
# build without ffmpeg
cmake -D WITH_TBB=ON -D BUILD_NEW_PYTHON_SUPPORT=ON \
      -D WITH_V4L=ON -D INSTALL_C_EXAMPLES=ON \
      -D INSTALL_PYTHON_EXAMPLES=ON -D BUILD_EXAMPLES=ON \
      -D WITH_QT=ON -D WITH_OPENGL=ON -D WITH_VTK=ON \
      -D WITH_FFMPEG=OFF -D EIGEN_INCLUDE_PATH=/opt/ocv/include \
      -D CMAKE_INSTALL_PREFIX:PATH=/opt/ocv2.4.10 ..

