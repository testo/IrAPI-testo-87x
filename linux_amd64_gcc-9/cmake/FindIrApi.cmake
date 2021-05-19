
#***************************************************************************
#* Copyright: Testo AG, 79849 Lenzkirch, Postfach 1140
#***************************************************************************
#**@file FindIRAPI.cmake
#  @brief<b>Description: </b> find script that searches for ridgid interface
#
#  <br> Initially written by: 1000len-kuj
#  <br> $Author:$
#  <br> $Date:$
#  <br> $HeadURL:$
#  <br> $Revision:$
#
#***************************************************************************


include("${CMAKE_CURRENT_LIST_DIR}/TestoLibFunctions.cmake")

find_file(IRAPI_CONFIG NAMES  "IrApi.cmake" PATHS
  "${IRAPI_LIBRARY_SEARCH_DIRS}"
  NO_SYSTEM_ENVIRONMENT_PATH
)
include("${IRAPI_CONFIG}")
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(IrApi DEFAULT_MSG IRAPI_LIBRARIES IRAPI_INCLUDE_DIR)

mark_as_advanced(IRAPI_CONFIG)

# create info message block
get_filename_component(THIS_CMAKE_PATH "${CMAKE_CURRENT_LIST_FILE}" PATH)

createFindInfo("IrApi" "${IRAPI_VERSION}" "${IRAPI_INCLUDE_DIR}" "${IRAPI_LIBRARY_DIR}" "${IRAPI_BINARY_DIR}" "${IRAPI_BOTH_LIBRARIES}")


