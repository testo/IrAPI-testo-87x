#ifndef IRCAM2020_IRAPI_CONFIG_H
# define IRCAM2020_IRAPI_CONFIG_H

#include <ircam2020_config.h>

// MODULE DEFINES


// MODULE DECLARATIONS
#if defined (WIN32) && defined(BUILD_SHARED_LIBS) 
#  if defined (irapi_EXPORTS) 
#   if !defined(IRCAM2020_IRAPI_DECL) 
#     define IRCAM2020_IRAPI_DECL __declspec(dllexport) 
#   endif 
#  endif 
#  if !defined (IRCAM2020_IRAPI_DECL) 
#     define IRCAM2020_IRAPI_DECL __declspec(dllimport) 
#  endif 
#else 
#if !defined(IRCAM2020_IRAPI_DECL) 
#  define IRCAM2020_IRAPI_DECL 
# endif 
#endif 


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

#endif
