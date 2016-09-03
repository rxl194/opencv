
/***** 
**  win-mingw or ubuntu
*****/


 1. Go to the "Cmake/" folder and search for the file named "OpenCVCompilerOptions.cmake"
    #add_extra_compiler_option(-Werror=non-virtual-dtor) // add comments

    // ocv3.1.0F
 2. cmake -DWITH_IPP=OFF -DWITH_DSHOW=OFF -DWITH_VFW=OFF

    // Compile error: need to add (#ifdef MINGW)

/***** 
**  ubuntu:
*****/
sudo usermod -a -G groupName userName
https://help.ubuntu.com/community/OpenCV
 
// cmake -D CMAKE_BUILD_TYPE=DEBUG -D CMAKE_INSTALL_PREFIX=/opt/ocv3.0.0 -D WITH_TBB=ON \
//  -D BUILD_NEW_PYTHON_SUPPORT=ON -D WITH_V4L=ON -D INSTALL_C_EXAMPLES=ON -D INSTALL_PYTHON_EXAMPLES=ON \
//  -D BUILD_EXAMPLES=ON -D WITH_QT=ON -D WITH_OPENGL=ON ..

 $ mkdir -p build
 $ cd build
 $ csh ../csh_cmake_ubuntu
 $ make -j4
 $ make; sudo make install; sudo ldconfig
 $ sudo cp /opt/ocv3.1.0/lib/python2.7/dist-packages/cv2.so /usr/local/lib/python2.7/dist-packages/
 $ cp /opt/ocv3.1.0/lib/python2.7/dist-packages/cv2.so /home/ystd2016/miniconda2/lib/python2.7/site-packages

/*********
** ocv3.1.0 with  extra_module
** and with tesseract
*********/

$ vi CMakeCache.txt
  OPENCV_EXTRA_MODULES_PATH:PATH=/home/ystd2016/git/ocv3.1.0_contrib/modules
  Tesseract_INCLUDE_DIR:PATH=/opt/ocv/include
  Tesseract_LIBRARY:FILEPATH=/opt/ocv/lib/libtesseract.a
$ csh ../csh_cmake_ubuntu
