#pragma once

#ifdef EMF2PDF_STATIC
  #define EMF2PDF_EXPORTS
#else
  #if (defined WIN32 || defined _WIN32 || defined WINCE || defined __CYGWIN__) && defined EMF2PDF_API_EXPORTS
    #define EMF2PDF_EXPORTS __declspec(dllexport)
  #elif defined __GNUC__ && __GNUC__ >= 4
    #define EMF2PDF_EXPORTS __attribute__ ((visibility ("default")))
  #else
    #define EMF2PDF_EXPORTS
  #endif
#endif

#define EMF2PDF_VERSION_MAJOR    0
#define EMF2PDF_VERSION_MINOR    9
#define EMF2PDF_VERSION_REVISION 34
#define EMF2PDF_VERSION_STATUS   ""

#define EMF2PDF_STR_EXP(__A)     #__A
#define EMF2PDF_STR(__A)         EMF2PDF_STR_EXP(__A)

#define EMF2PDF_VERSION          EMF2PDF_STR(EMF2PDF_VERSION_MAJOR) "." EMF2PDF_STR(EMF2PDF_VERSION_MINOR) "." EMF2PDF_STR(EMF2PDF_VERSION_REVISION) EMF2PDF_VERSION_STATUS
