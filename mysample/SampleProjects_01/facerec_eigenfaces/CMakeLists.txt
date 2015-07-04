
cmake_minimum_required(VERSION 2.8)
project( facerec_eigenfaces )

#find_package(OpenCV REQUIRED)
SET(OPENCV_CPP_SAMPLES_REQUIRED_DEPS opencv_core opencv_flann opencv_imgproc
    opencv_highgui opencv_ml opencv_video opencv_objdetect opencv_photo opencv_nonfree
    opencv_features2d opencv_calib3d opencv_legacy opencv_contrib opencv_stitching opencv_videostab)
	
ocv_check_dependencies(${OPENCV_CPP_SAMPLES_REQUIRED_DEPS})

ocv_include_directories("${OpenCV_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/include" )#for opencv.hpp
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

MACRO(OPENCV_DEFINE_CPP_EXAMPLE name srcs)

  set(sample_kind ps)
  set(sample_KIND PS)
  set(sample_subfolder "PS")

  set(the_target "${sample_kind}_${name}")
  add_executable(${the_target} ${srcs})
  target_link_libraries(${the_target} ${OPENCV_LINKER_LIBS} ${OPENCV_CPP_SAMPLES_REQUIRED_DEPS})
  
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

file(GLOB_RECURSE cpp_samples RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

foreach(sample_filename ${cpp_samples})
  get_filename_component(sample ${sample_filename} NAME_WE)
  OPENCV_DEFINE_CPP_EXAMPLE(${sample}  ${sample_filename})
endforeach()