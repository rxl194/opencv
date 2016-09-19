
# Minimum required CMake version
cmake_minimum_required(VERSION 2.8)

SET(OPENCV_CPP_SAMPLES_REQUIRED_DEPS opencv_core opencv_flann opencv_imgproc
    opencv_highgui opencv_ml opencv_video opencv_objdetect opencv_photo opencv_nonfree
    opencv_features2d opencv_calib3d opencv_legacy opencv_contrib opencv_stitching opencv_videostab)
    
ocv_check_dependencies(${OPENCV_CPP_SAMPLES_REQUIRED_DEPS})

PROJECT( damiles_OCR )

ocv_include_directories("${OpenCV_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/include")#for opencv.hpp
ocv_include_modules(${OPENCV_CPP_SAMPLES_REQUIRED_DEPS})

if(HAVE_opencv_gpu)
  ocv_include_directories("${OpenCV_SOURCE_DIR}/modules/gpu/include")
endif()

if(HAVE_opencv_ocl)
  ocv_include_directories("${OpenCV_SOURCE_DIR}/modules/ocl/include")
endif()

if(CMAKE_COMPILER_IS_GNUCXX AND NOT ENABLE_NOISY_WARNINGS)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-function")
endif()

ADD_EXECUTABLE( damiles_OCR main.cpp basicOCR.cpp preprocessing.cpp )
TARGET_LINK_LIBRARIES( damiles_OCR ${OPENCV_LINKER_LIBS} ${OPENCV_CPP_SAMPLES_REQUIRED_DEPS} )

if(HAVE_opencv_gpu)
  target_link_libraries(${the_target} opencv_gpu)
endif()

if(HAVE_opencv_ocl)
  target_link_libraries(${the_target} opencv_ocl)
endif()
