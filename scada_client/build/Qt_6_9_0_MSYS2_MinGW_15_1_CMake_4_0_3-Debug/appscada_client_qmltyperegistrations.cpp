/****************************************************************************
** Generated QML type registration code
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtQml/qqml.h>
#include <QtQml/qqmlmoduleregistration.h>



#if !defined(QT_STATIC)
#define Q_QMLTYPE_EXPORT Q_DECL_EXPORT
#else
#define Q_QMLTYPE_EXPORT
#endif
Q_QMLTYPE_EXPORT void qml_register_types_scada_client()
{
    QT_WARNING_PUSH QT_WARNING_DISABLE_DEPRECATED
    QT_WARNING_POP
    qmlRegisterModule("scada_client", 1, 0);
}

static const QQmlModuleRegistration scadaclientRegistration("scada_client", qml_register_types_scada_client);
