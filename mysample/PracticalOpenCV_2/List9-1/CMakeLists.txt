# Minimum required CMake version
cmake_minimum_required(VERSION 2.8)

# Project name
project(camera_calibrate)

# Find the OpenCV installation
#find_package(OpenCV REQUIRED)
SET(OPENCV_CPP_SAMPLES_REQUIRED_DEPS opencv_core opencv_flann opencv_imgproc
    opencv_highgui opencv_ml opencv_video opencv_objdetect opencv_photo opencv_nonfree
    opencv_features2d opencv_calib3d opencv_legacy opencv_contrib opencv_stitching opencv_videostab)

ocv_check_dependencies(${OPENCV_CPP_SAMPLES_REQUIRED_DEPS})

# Find the Boost installation, specifically the components 'system' and 'filesystem'
if (WIN32)
  if(MSVC)
    set(BOOST_ROOT "${OpenCV_SOURCE_DIR}/../boost_1_55_0")
  else()
    set(BOOST_ROOT "${OpenCV_SOURCE_DIR}/../boost_1_55_0")
    set(BOOST_INCLUDEDIR "${OpenCV_SOURCE_DIR}/include")
    set(BOOST_LIBRARYDIR "${BOOST_ROOT}/mingw/lib")
  endif()
endif()
find_package(Boost COMPONENTS system filesystem REQUIRED)


# Other directories where header files for linked libraries can be found
#include_directories(${OpenCV_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/include" ${Boost_INCLUDE_DIRS})
ocv_include_directories("${OpenCV_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/include" ${Boost_INCLUDE_DIRS} )#for opencv.hpp
ocv_include_modules(${OPENCV_CPP_SAMPLES_REQUIRED_DEPS})


# ${PROJECT_SOURCE_DIR} is the name of the root directory of the project
# TO_NATIVE_PATH converts the path ${PROJECT_SOURCE_DIR}/data/ to a full path and the file() command stores it in DATA_FOLDER
file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}/data/" DATA_FOLDER)
# set TRAIN_FOLDER to DATA_FOLDER/train_images - this is where we will put our templates for constructing the vocabulary
set(IMAGE_FOLDER "${DATA_FOLDER}images/")

# set the configuration input file to ${PROJECT_SOURCE_DIR}/Config.h.in and the includable header file holding configuration information to ${PROJECT_SOURCE_DIR}/include/Config.h
configure_file("${PROJECT_SOURCE_DIR}/Config.h.in" "${PROJECT_SOURCE_DIR}/include/Config.h")


# executable produced as a result of compilation
# libraries to be linked with this executable - OpenCV
#add_executable(code9-1 src/code9-1.cpp)
#target_link_libraries(code9-1  ${OPENCV_LINKER_LIBS} ${OPENCV_CPP_SAMPLES_REQUIRED_DEPS} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})

MACRO(OPENCV_DEFINE_CPP_EXAMPLE name srcs)

  set(sample_kind po)
  set(sample_KIND PO)
  set(sample_subfolder "cpp")

  set(the_target "${sample_kind}_${name}")
  add_executable(${the_target} src/${srcs})
  target_link_libraries(${the_target} ${OPENCV_LINKER_LIBS} ${OPENCV_CPP_SAMPLES_REQUIRED_DEPS} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})

  if(WIN32)
    if (MSVC AND NOT BUILD_SHARED_LIBS)
      set_target_properties(${the_target} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:atlthunk.lib /NODEFAULTLIB:atlsd.lib /DEBUG")
    endif()
    install(TARGETS ${the_target}
            RUNTIME DESTINATION "${OPENCV_SAMPLES_BIN_INSTALL_PATH}/${sample_subfolder}" COMPONENT samples)
  endif()
ENDMACRO()

file(GLOB_RECURSE cpp_samples RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/src *.cpp)

foreach(sample_filename ${cpp_samples})
  get_filename_component(sample ${sample_filename} NAME_WE)
  OPENCV_DEFINE_CPP_EXAMPLE(${sample}  ${sample_filename})
endforeach()
