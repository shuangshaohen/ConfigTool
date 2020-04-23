#ifndef TOPOPARA_GLOBAL_H
#define TOPOPARA_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TOPOPARA_LIBRARY)
#  define TOPOPARA_EXPORT Q_DECL_EXPORT
#else
#  define TOPOPARA_EXPORT Q_DECL_IMPORT
#endif

#endif // TOPOPARA_GLOBAL_H
