#pragma once

#ifdef PROJECT_ALPHA_DLL
  #define PROJECT_ALPHA_API __declspec(dllexport) 
#else
  #define PROJECT_ALPHA_API __declspec(dllimport) 
#endif
