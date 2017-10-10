#ifndef KV_MS_UTILS_IP_VALIDATOR_H
#define KV_MS_UTILS_IP_VALIDATOR_H

#include <QValidator>

#include "utils_global.h"

namespace Utils {

class MEDIASTUDIO_UTILS_EXPORT IPv4Validator : public QValidator
{
    Q_OBJECT

public:
    explicit IPv4Validator(QObject *parent = 0);

    void fixup(QString &ip) const;

    State validate(QString &input, int &pos) const;
};

} // namespace Utils

#endif // KV_MS_UTILS_IP_VALIDATOR_H
