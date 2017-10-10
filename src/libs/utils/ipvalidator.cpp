#include "ipvalidator.h"

namespace Utils {

#define IPV4_FIELD_COUNT             4
#define IPV4_FIELD_SEPARATOR        "."
#define IPV4_FIELD_DEFAULT_VALUE    "0"

// TODO: maximize, minimize support required

IPv4Validator::IPv4Validator(QObject *parent) :
    QValidator(parent)
{
}

void IPv4Validator::fixup(QString &ip) const
{
    QStringList fields = ip.split(IPV4_FIELD_SEPARATOR);
    Q_ASSERT(fields.size() <= IPV4_FIELD_COUNT);

    for (int i = 0; i < fields.size(); ++i) {
        if (fields[i].isEmpty()) {
            fields[i] = IPV4_FIELD_DEFAULT_VALUE;
        }
    }

    while (fields.size() < IPV4_FIELD_COUNT) {
        fields.push_back(IPV4_FIELD_DEFAULT_VALUE);
    }

    ip = fields[0];
    for (int i = 1; i < IPV4_FIELD_COUNT; ++i) {
        ip += IPV4_FIELD_SEPARATOR + fields[i];
    }
}

QValidator::State IPv4Validator::validate(QString &input, int &pos) const
{
    if (input.isEmpty())
        return Intermediate;

    input.replace(' ', IPV4_FIELD_SEPARATOR);

    QStringList fields = input.split(IPV4_FIELD_SEPARATOR);

    for (int i = 0; i < fields.size(); ++i) {
        if ((fields[i] != "0") && fields[i][0] == '0') {
            fields[i].remove(0, 1);
            fields.insert(i, "0");
            ++pos;
        }
    }

    if (fields.size() > IPV4_FIELD_COUNT)
        return Invalid;

    bool hasEmptyField = false;
    for (int i = 0; i < fields.size(); ++i) {
        if (fields[i].isEmpty()) {
            hasEmptyField = true;
            continue;
        }

        bool bOk = false;
        uint val = fields[i].toUInt(&bOk);

        if (!bOk)
            return Invalid;

        if (val > 255) {
            int cur_pos = pos - i;
            for (int j = 0; j <= i; ++j) {
                cur_pos -= fields[j].length();
            }

            if (cur_pos < 0) {
                fields[i].chop(0 - cur_pos);

                while (cur_pos++ < 0) {
                    val /= 10;
                }
                if (val > 255)
                    return Invalid;
            } else {
                fields[i] = QString::number(val/10);

                if (i < fields.size()-1) {
                    fields[i+1] = QString::number(val%10);
                    ++pos;
                } else if (fields.size() < IPV4_FIELD_COUNT) {
                    fields.push_back(QString::number(val%10));
                    ++pos;
                } else {
                    return Invalid;
                }
            }
        }
    }

    input = fields[0];
    for (int i = 1; i < fields.size(); ++i) {
        input += IPV4_FIELD_SEPARATOR + fields[i];
    }

    if (fields.size() < IPV4_FIELD_COUNT || hasEmptyField)
        return Intermediate;

    return Acceptable;
}

} // namespace Utils
