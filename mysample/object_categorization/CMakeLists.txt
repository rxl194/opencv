# Minimum required CMake version
cmake_minimum_required(VERSION 2.8)

# Find the Boost installation, specifically the components 'system' and 'filesystem'
if (WIN32)
  if (MSVC)
     set(BOOST_ROOT "/opt/boost_1_55_VS08")
     set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib")
  else()
    set(BOOST_ROOT "${OpenCV_SOURCE_DIR}/../boost_1_55_0")
#    set(BOOST_INCLUDEDIR "${BOOST_ROOT}/include")
    set(BOOST_LIBRARYDIR "${BOOST_ROOT}/mingw/lib")
  endif()
endif()
find_package(Boost COMPONENTS system filesystem REQUIRED)

SET(OPENCV_CPP_SAMPLES_REQUIRED_DEPS opencv_core opencv_flann opencv_imgproc
    opencv_highgui opencv_ml opencv_video opencv_objdetect opencv_photo opencv_nonfree
    opencv_features2d opencv_calib3d opencv_legacy opencv_contrib opencv_stitching opencv_videostab)

ocv_check_dependencies(${OPENCV_CPP_SAMPLES_REQUIRED_DEPS})

# Project name
project(object_categorization)

ocv_include_directories("${OpenCV_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/include" ${Boost_INCLUDE_DIRS})#for opencv.hpp
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


# ${PROJECT_SOURCE_DIR} is the name of the root directory of the project
# TO_NATIVE_PATH converts the path ${PROJECT_SOURCE_DIR}/data/ to a full path and the file() command stores it in DATA_FOLDER
file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}/data/" DATA_FOLDER)
# set TRAIN_FOLDER to DATA_FOLDER/train_images - this is where we will put our templates for constructing the vocabulary
set(TRAIN_FOLDER "${DATA_FOLDER}train_images/")
# set TEMPLATE_FOLDER to DATA_FOLDER/templates - this is where we will put our traininfg images, in folders organized by category
set(TEMPLATE_FOLDER "${DATA_FOLDER}templates/")

# set the configuration input file to ${PROJECT_SOURCE_DIR}/Config.h.in and the includable header file holding configuration information to ${PROJECT_SOURCE_DIR}/include/Config.h
configure_file("${PROJECT_SOURCE_DIR}/Config.h.in" "${PROJECT_SOURCE_DIR}/include/Config.h")

# Other directories where header files for linked libraries can be found
include_directories(${OpenCV_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/include" ${Boost_INCLUDE_DIRS})

# executable produced as a result of compilation
#add_executable(code8-5 src/List8_5.cpp)
# libraries to be linked with this executable - OpenCV and Boost (system and filesystem components)
#target_link_libraries(code8-5  ${OPENCV_LINKER_LIBS} ${OPENCV_CPP_SAMPLES_REQUIRED_DEPS} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})

# ---------------------------------------------
#      Define executable targets
# ---------------------------------------------
MACRO(OPENCV_DEFINE_CPP_EXAMPLE name srcs)

  set(sample_kind poobj)
  set(sample_KIND POOBJ)
  set(sample_subfolder "PoObj")

  set(the_target "${sample_kind}_${name}")
  add_executable(${the_target} src/${srcs})
  target_link_libraries(${the_target} ${OPENCV_LINKER_LIBS} ${OPENCV_CPP_SAMPLES_REQUIRED_DEPS} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})

  if(HAVE_opencv_gpu)
    target_link_libraries(${the_target} opencv_gpu)
  endif()

  if(HAVE_opencv_ocl)
    target_link_libraries(${the_target} opencv_ocl)
  endif()

  set_target_properties(${the_target} PROPERTIES
    OUTPUT_NAME "${sample_kind}-${name}"
    PROJECT_LABEL "(${sample_KIND}) ${name}")

    if(ENABLE_SOLUTION_FOLDERS)
      set_target_properties(${the_target} PROPERTIES FOLDER "PracticalOpenCV/${sample_subfolder}")
    endif()

  if(WIN32)
    if (MSVC AND NOT BUILD_SHARED_LIBS)
      set_target_properties(${the_target} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:atlthunk.lib /NODEFAULTLIB:atlsd.lib /DEBUG")
    endif()
    install(TARGETS ${the_target}
            RUNTIME DESTINATION "${OPENCV_SAMPLES_BIN_INSTALL_PATH}/${sample_subfolder}" COMPONENT samples)
  endif()
ENDMACRO()

set(SRC_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/src")
file(GLOB_RECURSE cpp_samples RELATIVE ${SRC_FOLDER} *.cpp)

foreach(sample_filename ${cpp_samples})
  get_filename_component(sample ${sample_filename} NAME_WE)
  OPENCV_DEFINE_CPP_EXAMPLE(${sample}  ${sample_filename})
#  OPENCV_DEFINE_CPP_EXAMPLE("List8_5"  "List8_5.cpp")
endforeach()
