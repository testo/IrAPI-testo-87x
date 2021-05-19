#***************************************************************************
#* Copyright: Testo AG, 79849 Lenzkirch, Postfach 1140
#***************************************************************************
#**@file FindOpenCV.cmake
#  @brief<b>Description: </b> find script that searches for the OPENCV library
#                             set 'OPENCV_LIBRARY_SEARCH_DIRS'
#                             to find opencv in a custom library search path
#
#  <br> Initially written by: 1000len-scm
#  <br> $Author:$
#  <br> $Date:$
#  <br> $HeadURL:$
#  <br> $Revision:$
#
#***************************************************************************

IF(YOCTO)
  unset(OpenCV_Config CACHE)
  unset(OpenCV_Config_Version CACHE)
  unset(OpenCV_CONFIG_PATH CACHE)
	SET(OPENCV_LIBRARY_SEARCH_DIRS ${YOCTO_LIB_PATH} CACHE PATH "" FORCE)
  MESSAGE("yocto environment searching opencv in ${OPENCV_LIBRARY_SEARCH_DIRS}")
ENDIF()

find_file(OpenCV_Config NAMES  "OpenCVConfig.cmake" PATHS
  "${SYSROOT_FOR_BUILD}/share/OpenCV"
  "${SYSROOT_FOR_BUILD}/lib/cmake/opencv4"
  "${OPENCV_LIBRARY_SEARCH_DIRS}"
  "${OPENCV_LIBRARY_SEARCH_DIRS}/share/OpenCV"
  "${OPENCV_LIBRARY_SEARCH_DIRS}/usr/share/OpenCV"
  "${OPENCV_LIBRARY_SEARCH_DIRS}/lib/cmake/opencv4"
  "${TESTOLIB_THIRD_PARTY_LIBS_DIR}/opencv/"
  "${TESTOLIB_THIRD_PARTY_LIBS_DIR}/opencv/share/OpenCV"
  "${TESTOLIB_THIRD_PARTY_LIBS_DIR}/opencv/sdk/native/jni"
  "${TESTOLIB_THIRD_PARTY_LIBS_DIR}/opencv/lib/cmake/opencv4"
  NO_SYSTEM_ENVIRONMENT_PATH
)

find_file(OpenCV_Config_Version NAMES  "OpenCVConfig-version.cmake" PATHS
  "${SYSROOT_FOR_BUILD}/share/OpenCV"
  "${SYSROOT_FOR_BUILD}/lib/cmake/opencv4"
  "${OPENCV_LIBRARY_SEARCH_DIRS}"
  "${OPENCV_LIBRARY_SEARCH_DIRS}/share/OpenCV"
  "${OPENCV_LIBRARY_SEARCH_DIRS}/usr/share/OpenCV"
  "${OPENCV_LIBRARY_SEARCH_DIRS}/lib/cmake/opencv4"
  "${TESTOLIB_THIRD_PARTY_LIBS_DIR}/opencv/"
  "${TESTOLIB_THIRD_PARTY_LIBS_DIR}/opencv/share/OpenCV"
  "${TESTOLIB_THIRD_PARTY_LIBS_DIR}/opencv/sdk/native/jni"
  "${TESTOLIB_THIRD_PARTY_LIBS_DIR}/opencv/lib/cmake/opencv4"
  NO_SYSTEM_ENVIRONMENT_PATH
)

include("${OpenCV_Config}")
include("${OpenCV_Config_Version}")

  MESSAGE("OpenCV_CONFIG_PATH :  ${OpenCV_CONFIG_PATH}")

if(MSVC AND ENABLE_PROFILING)
  set(_IMPORT_PREFIX ${OpenCV_CONFIG_PATH})
  string(SUBSTRING ${OpenCV_VERSION} 0 1 OPENCV_MAJOR_VERSION)
  message("OpenCV major version: " ${OPENCV_MAJOR_VERSION})
  if(OPENCV_MAJOR_VERSION GREATER 2)
    message("==> include " ${CMAKE_CURRENT_LIST_DIR}/OpenCVModules-relwithdebinfo_300.cmake)
    include("${CMAKE_CURRENT_LIST_DIR}/OpenCVModules-relwithdebinfo_300.cmake")
  else()
    message("==> include " ${CMAKE_CURRENT_LIST_DIR}/OpenCVModules-relwithdebinfo.cmake)
    include("${CMAKE_CURRENT_LIST_DIR}/OpenCVModules-relwithdebinfo.cmake")
  endif()
endif()

set(OPENCV_INCLUDE_DIRS  ${OpenCV_INCLUDE_DIRS})
set(OPENCV_LIB_DIR       ${OpenCV_LIB_DIR})
set(OPENCV_LIBRARIES     ${OpenCV_LIBS})

if(UNIX)
  set(OPENCV_LIB_DIR    "${OpenCV_INSTALL_PATH}/lib")
  set(OPENCV_BIN_DIR    "${OpenCV_INSTALL_PATH}/lib")
  set(OPENCV_BINARY_DIR "${OpenCV_INSTALL_PATH}/lib")

  if(ANDROID)
    if(MOBILEAPPS_TARGET_ARCHITECTURE)
      set(OPENCV_LIB_DIR    "${OpenCV_INSTALL_PATH}/sdk/native/libs/${MOBILEAPPS_TARGET_ARCHITECTURE}/")
      set(OPENCV_BIN_DIR    "${OpenCV_INSTALL_PATH}/sdk/native/libs/${MOBILEAPPS_TARGET_ARCHITECTURE}/")
      set(OPENCV_BINARY_DIR "${OpenCV_INSTALL_PATH}/sdk/native/libs/${MOBILEAPPS_TARGET_ARCHITECTURE}/")
    else()
      set(OPENCV_LIB_DIR    "${OpenCV_INSTALL_PATH}/sdk/native/libs/${ANDROID_ABI}")
      set(OPENCV_BIN_DIR    "${OpenCV_INSTALL_PATH}/sdk/native/libs/${ANDROID_ABI}")
      set(OPENCV_BINARY_DIR "${OpenCV_INSTALL_PATH}/sdk/native/libs/${ANDROID_ABI}")
    endif()
  endif()
else(WIN32)
  if(_OpenCV_LIB_PATH)
    string(REPLACE "\\" "/" OPENCV_BIN_DIR    ${_OpenCV_LIB_PATH})
    string(REPLACE "\\" "/" OPENCV_BINARY_DIR ${_OpenCV_LIB_PATH})
  else()
    set( OPENCV_BIN_DIR    "${OpenCV_LIB_DIR}/../bin" )
    set( OPENCV_BINARY_DIR "${OpenCV_LIB_DIR}/../bin" )
  endif()
endif()


include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OpenCV OpenCV_VERSION OPENCV_INCLUDE_DIRS OPENCV_LIB_DIR OPENCV_LIBRARIES)

if(MOBILE_BUILD)
  link_directories("${OPENCV_LIB_DIR}")
endif()

include_directories(SYSTEM "${OPENCV_INCLUDE_DIRS}")

set(OpenCv_FOUND ON)

# create info message block
get_filename_component(THIS_CMAKE_PATH "${CMAKE_CURRENT_LIST_FILE}" PATH)
include("${CMAKE_CURRENT_LIST_DIR}/TestoLibFunctions.cmake")
createFindInfo("OpenCV" "${OpenCV_VERSION}" "${OPENCV_INCLUDE_DIRS}" "${OPENCV_LIB_DIR}" "${OPENCV_BIN_DIR}" "${OPENCV_LIBRARIES}")

mark_as_advanced(OpenCV_Config)
