#include <QEvent>

#include "action/command.h"
#include "commandcombobox.h"

namespace Core {

// A combo associated with a command. Shows the command text
// and shortcut in the tooltip.

CommandComboBox::CommandComboBox(QWidget *parent)
    : QComboBox(parent)
{
}

bool CommandComboBox::event(QEvent *e)
{
    if (e->type() == QEvent::ToolTip) {
        const QString text = currentText();
        if (const Core::Command *cmd = command(text)) {
            const QString tooltip = tr("Activate %1 View").arg(text);
            setToolTip(cmd->stringWithAppendedShortcut(tooltip));
        } else {
            setToolTip(text);
        }
    }
    return QComboBox::event(e);
}

} // namespace Core
