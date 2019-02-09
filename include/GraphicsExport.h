/* #ifdef COOLER_COMMON_EXPORTS
#define COOLER_COMMON_DLL_EXPORT __declspec(dllexport)
#else
#define COOLER_COMMON_DLL_EXPORT __declspec(dllimport)
#endif
 */
// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
  #define COOLER_COMMON_DLL_EXPORT __declspec(dllimport)
  #define COOLER_COMMON_DLL_EXPORT __declspec(dllexport)
  #define COOLER_COMMON_DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define COOLER_COMMON_DLL_IMPORT __attribute__ ((visibility ("default")))
    #define COOLER_COMMON_DLL_EXPORT __attribute__ ((visibility ("default")))
    #define COOLER_COMMON_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define COOLER_COMMON_DLL_IMPORT
    #define COOLER_COMMON_DLL_EXPORT
    #define COOLER_COMMON_DLL_LOCAL
  #endif
#endif