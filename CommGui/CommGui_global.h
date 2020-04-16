#ifndef COMMGUI_GLOBAL_H
#define COMMGUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COMMGUI_LIBRARY)
#  define COMMGUI_EXPORT Q_DECL_EXPORT
#else
#  define COMMGUI_EXPORT Q_DECL_IMPORT
#endif

#endif // COMMGUI_GLOBAL_H
