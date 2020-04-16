#ifndef OUTPUT_GLOBAL_H
#define OUTPUT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(OUTPUT_LIBRARY)
#  define OUTPUT_EXPORT Q_DECL_EXPORT
#else
#  define OUTPUT_EXPORT Q_DECL_IMPORT
#endif

#endif // OUTPUT_GLOBAL_H
