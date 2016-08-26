
/***** 
**  win-mingw or ubuntu
*****/


 1. Go to the "Cmake/" folder and search for the file named "OpenCVCompilerOptions.cmake"
    #add_extra_compiler_option(-Werror=non-virtual-dtor) // add comments


 2. cmake -DWITH_IPP=OFF

/***** 
**  ubuntu:
*****/
sudo usermod -a -G groupName userName
https://help.ubuntu.com/community/OpenCV
 
 cmake -D CMAKE_BUILD_TYPE=DEBUG -D CMAKE_INSTALL_PREFIX=/opt/ocv-3.0.0 -D WITH_TBB=ON \
  -D BUILD_NEW_PYTHON_SUPPORT=ON -D WITH_V4L=ON -D INSTALL_C_EXAMPLES=ON -D INSTALL_PYTHON_EXAMPLES=ON \
  -D BUILD_EXAMPLES=ON -D WITH_QT=ON -D WITH_OPENGL=ON ..

 
 3. make -j4

 4. make; sudo make install; sudo ldconfig

