#pragma once

#ifdef _MSC_VER
#   undef MATH_API

#   ifdef MATH_BUILD_DLL
#       define MATH_API __declspec(dllexport)
#   elif (defined MATH_DLL)
#       define MATH_API __declspec(dllimport)
#   else
#       define MATH_API
#   endif

#else
#   define MATH_API __attribute__ ((visibility("default")))
#endif // (defined _MSC_VER)
