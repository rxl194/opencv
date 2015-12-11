
 1. Go to the "Cmake/" folder and search for the file named "OpenCVCompilerOptions.cmake"
    #add_extra_compiler_option(-Werror=non-virtual-dtor) // add comments

 2. cmake -DWITH_IPP=OFF
 
 3. make -j4

 4. make; sudo make install; sudo ldconfig

