#ifndef CPPLIB_GLOBAL_H
#define CPPLIB_GLOBAL_H
#include <QtCore/qglobal.h>
#if defined(CPPLIB)
    #ifdef CPPLIBSTATIC
        #define CPPLIB_EXPORT// para compilar o base estaticamente
    #else
        #  define CPPLIB_EXPORT Q_DECL_EXPORT
    #endif
#else
    #ifdef CPPLIBSTATIC
        #  define CPPLIB_EXPORT // qdo um app utiliza o base compilado estaticamente
    #else
        #  define CPPLIB_EXPORT Q_DECL_IMPORT
    #endif
#endif

#endif

