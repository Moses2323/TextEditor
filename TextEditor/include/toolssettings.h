/** @addtogroup NNTools
 * @{*/

/** @file */

#ifndef TOOLSSETTINGS_H
#define TOOLSSETTINGS_H

#include <QtCore/qglobal.h>

#if defined(TOOLS_LIBRARY)
#  define TOOLS_EXPORT Q_DECL_EXPORT
#else
#  define TOOLS_EXPORT Q_DECL_IMPORT
#endif

#endif // TOOLSSETTINGS_H

/*@}*/
