#ifndef __IRCAM2020_CONFIG_H__
# define __IRCAM2020_CONFIG_H__

#if defined(__WIN32__) || defined(__WIN32)
# if !defined(WIN32)
#  define WIN32 1
# endif
#endif

/* debug define */
#ifdef _DEBUG
# ifndef DEBUG
#  define DEBUG
# endif
#endif

#define CONF_VAL_FPGA_IRENGINE_T871_REG_BITORDERING  0

/* version information defines */
#define IRCAM2020_VERSION_MAJOR 
#define IRCAM2020_VERSION_MINOR 
#define IRCAM2020_VERSION_PATCH 
#define IRCAM2020_VERSION_BUILD 


#define BUILD_SHARED_LIBS
/* #undef USE_QT */
#define UNIX
/* #undef MSVC */
/* #undef MSVC10 */
/* #undef MSVC11 */
/* #undef MSVC12 */
/* #undef MSVC60 */
/* #undef MSVC70 */
/* #undef MSVC71 */
/* #undef MSVC80 */
/* #undef MSVC90 */
#define GNU_GCC

#define CMAKE_MAKE_PROGRAM /usr/bin/make

#define IRCAM2020_VERSION          "1.2.0"
/* #undef IRCAM2020_BUILD_T87X */
/* #undef IRCAM2020_BUILD_T88X */
/* #undef IRCAM2020_BUILD_TESTS */
/* #undef IRCAM2020_BUILD_TOOLS */
/* #undef IRCAM2020_BUILD_EXAMPLES */
/* #undef IRCAM2020_GENERATE_MAP_FILES */
/* #undef ENABLE_QML_DEBUGGING */
/* #undef CONF_EN_TARGET */
/* #undef CONF_EN_TARGET_VISCAM */
/* #undef CONF_EN_TARGET_IRCAM */
/* #undef CONF_EN_TARGET_LENS */
/* #undef ENABLE_PIC */
/* #undef ENABLE_GCOV */
/* #undef ENABLE_GPROF */
/* #undef IRCAM2020_BUILD_CAMERA_CORE */
/* #undef IRCAM2020_BUILD_CAMERA_GUI */
/* #undef IRCAM2020_BUILD_USB_CONNECT_DAEMON */
#define IRCAM2020_BUILD_IRAPI 
#define IRCAM2020_BUILD_IRAPI_EXAMPLE 
/* #undef IRCAM2020_BUILD_RIDGID */

#ifndef CONF_EN_CXX11_REGEX
#define CONF_EN_CXX11_REGEX
#endif //CONF_EN_CXX11_REGEX
#ifndef CONF_EN_CXX11
#define CONF_EN_CXX11
#endif //CONF_EN_CXX11
#ifndef CONF_EN_CXX11_FUTURE
#define CONF_EN_CXX11_FUTURE
#endif //CONF_EN_CXX11_FUTURE
#ifndef CONF_EN_CXX11_TYPED_ENUM
#define CONF_EN_CXX11_TYPED_ENUM
#endif //CONF_EN_CXX11_TYPED_ENUM
#ifndef CONF_EN_CXX11_DECLTYPE
#define CONF_EN_CXX11_DECLTYPE
#endif //CONF_EN_CXX11_DECLTYPE
#ifndef CONF_EN_CXX11_VARIADIC_TEMPLATE
#define CONF_EN_CXX11_VARIADIC_TEMPLATE
#endif //CONF_EN_CXX11_VARIADIC_TEMPLATE


#define IRCAM2020_RESOURCE_DIR "../user"
#define IRCAM2020_IMAGE_DIR "../images"
#define IRCAM2020_CORE_LOG_FILE "core.log"
#define IRCAM2020_GUI_LOG_FILE "gui.log"

#ifdef __cplusplus
# ifdef WIN32
#  include <cstdint>
# else
#  include <stdint.h>
# endif
#else // not defined __cplusplus
# include <stdint.h>
#endif // __cplusplus

/* disable dll-linkage warnings that have no effect at all */
#if defined MSVC
# pragma warning(disable:4100) // unreferenced formal parameter
# pragma warning(disable:4127)
# pragma warning(disable:4251)
# pragma warning(disable:4996)
# pragma warning(disable:4275)
# pragma warning(disable:4512) // assignment operator could not be generated (testo::Variant needs this!)
# pragma warning(disable:4718) // QT header warning: recursive call has no side effects, deleting
# if defined MSVC10
#  pragma warning(disable:4102) // lambda functions with gtest
# endif
#endif



#endif /* __IRCAM2020_CONFIG_H__ */
