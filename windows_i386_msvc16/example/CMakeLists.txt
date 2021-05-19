cmake_minimum_required (VERSION 2.6)
project (IrApiExample)

SET(BUILD_SHARED_LIBS ON CACHE BOOL "")
# Set the location of the cmake modules
set (CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../cmake")

#Add the relative paths of IrApi and OpenCv to the library search directories
set (IRAPI_LIBRARY_SEARCH_DIRS "${CMAKE_CURRENT_LIST_DIR}/..")
set (OPENCV_LIBRARY_SEARCH_DIRS "${CMAKE_CURRENT_LIST_DIR}/../thirdparty/opencv")

# Find and configure OpenCv and IrApi
find_package(OpenCv)
find_package(IrApi)

# set binary dir for debugging (MSVC config)
set(PROPERTY_EXTERN_LIB_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/../bin" )

# add bmt file example target and link it to irapi and opencv
add_executable(example_bmt example_bmt.cpp)
target_link_libraries(example_bmt ${OPENCV_LIBRARIES} ${IRAPI_LIBRARIES})

if(WIN32)
  configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/../cmake/VSEnv.vcxproj.user.in"
    "${CMAKE_CURRENT_BINARY_DIR}/example_bmt.vcxproj.user")
endif()

# add bmt file example target and link it to irapi and opencv
add_executable(example_cam example_cam.cpp)
target_link_libraries(example_cam ${OPENCV_LIBRARIES} ${IRAPI_LIBRARIES})

if(WIN32)
  configure_file(
    "${CMAKE_CURRENT_LIST_DIR}/../cmake/VSEnv.vcxproj.user.in"
    "${CMAKE_CURRENT_BINARY_DIR}/example_cam.vcxproj.user")
endif()