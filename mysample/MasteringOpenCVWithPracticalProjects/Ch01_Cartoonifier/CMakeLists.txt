cmake_minimum_required (VERSION 2.6) 

PROJECT(Cartoonifier)

# Requires OpenCV
#FIND_PACKAGE( OpenCV REQUIRED )

SET(OPENCV_CPP_SAMPLES_REQUIRED_DEPS opencv_core opencv_flann opencv_imgproc
    opencv_highgui opencv_ml opencv_video opencv_objdetect opencv_photo opencv_nonfree
    opencv_features2d opencv_calib3d opencv_legacy opencv_contrib opencv_stitching opencv_videostab)

ocv_check_dependencies(${OPENCV_CPP_SAMPLES_REQUIRED_DEPS})

ocv_include_directories("${OpenCV_SOURCE_DIR}/include" "${OpenCV_SOURCE_DIR}/include/opencv" "${PROJECT_SOURCE_DIR}/include")
ocv_include_modules(${OPENCV_CPP_SAMPLES_REQUIRED_DEPS})

SET(SRC
    main_desktop.cpp
    cartoon.cpp
    ImageUtils_0.7.cpp
)

ADD_EXECUTABLE( ${PROJECT_NAME} ${SRC} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME}  ${OPENCV_LINKER_LIBS} ${OPENCV_CPP_SAMPLES_REQUIRED_DEPS} )
