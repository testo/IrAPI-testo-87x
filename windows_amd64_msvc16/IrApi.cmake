# This file defines some variables which cen be used to build other projects
# using the precompiled thermography interface

set(IRAPI_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(IRAPI_SOURCE_DIR "${IRAPI_REAL_SOURCE_DIR}")
set(IRAPI_BUILD_DIR "${IRAPI_REAL_BINARY_DIR}")
set(IRAPI_NAME "${IRAPI_NAME}")

################################  GET VERSION  ###############################
file(READ "${CMAKE_CURRENT_LIST_DIR}/version" IRAPI_VERSION)
string(REPLACE "." ";" IRAPI_VERSIONLIST ${IRAPI_VERSION})
list(GET IRAPI_VERSIONLIST 0 IRAPI_VERSION_MAJOR)
list(GET IRAPI_VERSIONLIST 1 IRAPI_VERSION_MINOR)
list(GET IRAPI_VERSIONLIST 2 IRAPI_VERSION_PATCH)

set(DEBUG_POSTFIX "_${IRAPI_VERSION_MAJOR}_${IRAPI_VERSION_MINOR}d")
set(RELEASE_POSTFIX "_${IRAPI_VERSION_MAJOR}_${IRAPI_VERSION_MINOR}")

################################  DEFINE POSTFIX ###############################
if(ANDROID AND BUILD_SHARED_LIBS)
  set(DEBUG_POSTFIX ${DEBUG_POSTFIX}.so)
  set(RELEASE_POSTFIX ${RELEASE_POSTFIX}.so)
endif()

if(WIN32)
  # set file ending later use for dll and lib
  set(DEBUG_POSTFIX ${DEBUG_POSTFIX})
  set(RELEASE_POSTFIX ${RELEASE_POSTFIX})
endif()

if(NOT BUILD_SHARED_LIBS AND NOT WIN32)
  set(DEBUG_POSTFIX ${DEBUG_POSTFIX}.a)
  set(RELEASE_POSTFIX ${RELEASE_POSTFIX}.a)
endif()

# DIRECTORIES
set(IRAPI_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include" CACHE PATH "Path where IRAPI_ includes are located")
set(IRAPI_LIBRARY_DIR "${CMAKE_CURRENT_LIST_DIR}/lib"     CACHE PATH "Path where IRAPI_ libraries are located")
if(WIN32)
 set(IRAPI_BINARY_DIR  "${CMAKE_CURRENT_LIST_DIR}/bin"     CACHE PATH "Path where IRAPI_ binaries are located")
endif()

##############################  SET LIBRARIES  ####################################

if(WIN32)
  set(IRAPI_BINARIES_DEBUG debug     irapi${DEBUG_POSTFIX}.dll)
  set(IRAPI_BINARIES_RELEASE optimized irapi${RELEASE_POSTFIX}.dll)
  set(IRAPI_BINARIES ${IRAPI_BINARIES_DEBUG} ${IRAPI_BINARIES_RELEASE})

  set(IRAPI_LIBRARIES_DEBUG   debug   irapi${DEBUG_POSTFIX}.lib)
  set(IRAPI_LIBRARIES_RELEASE optimized irapi${RELEASE_POSTFIX}.lib)
  set(IRAPI_LIBRARIES ${IRAPI_LIBRARIES_DEBUG} ${IRAPI_LIBRARIES_RELEASE})
  set(IRAPI_BOTH_LIBRARIES ${IRAPI_LIBRARIES})
else()
  set(IRAPI_LIBRARIES_DEBUG   debug     irapi${DEBUG_POSTFIX})
  set(IRAPI_LIBRARIES_RELEASE optimized irapi${RELEASE_POSTFIX})
  set(IRAPI_LIBRARIES ${IRAPI_LIBRARIES_DEBUG} ${IRAPI_LIBRARIES_RELEASE})
  set(IRAPI_BOTH_LIBRARIES ${IRAPI_LIBRARIES})
endif()

##################################  SET DIR  #######################################
include_directories(${IRAPI_INCLUDE_DIR})
link_directories(${IRAPI_LIBRARY_DIR})
#for now third party are put into bin? - LINUX
#link_directories(${IRAPI_BINARY_DIR})

mark_as_advanced(IRAPI_INCLUDE_DIR)
mark_as_advanced(IRAPI_LIBRARY_DIR)

if(WIN32)
mark_as_advanced(IRAPI_BINARY_DIR)
endif()
