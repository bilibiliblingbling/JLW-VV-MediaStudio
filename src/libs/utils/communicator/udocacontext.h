#ifndef UTILS_COMM_UDOCA_CONTEXT_H
#define UTILS_COMM_UDOCA_CONTEXT_H

#include <QObject>

#include <UDOCA/Protocol.h>

#include "utils/utils_global.h"

namespace Utils {

class MEDIASTUDIO_UTILS_EXPORT UDOCAContext : public QObject, public UDOCA::IContext
{
    Q_OBJECT

public:
    UDOCAContext(QObject *parent = 0);
    ~UDOCAContext();

signals:
    void responseReceived(const UDOCA::Request &req, const UDOCA::Response &resp);
};

} // namespace Utils

#endif // UTILS_COMM_UDOCA_CONTEXT_H
