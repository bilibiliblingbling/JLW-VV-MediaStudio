#ifndef COMMAND_COMBOBOX_H
#define COMMAND_COMBOBOX_H

#include <QComboBox>

#include "core_global.h"

namespace Core {

class Command;
class CORE_EXPORT CommandComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit CommandComboBox(QWidget *parent = 0);

protected:
    bool event(QEvent *e);

private:
    virtual const Command *command(const QString &text) const = 0;
};

} // namespace Core

#endif // COMMAND_COMBOBOX_H
