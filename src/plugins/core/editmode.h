#ifndef EDIT_MODE_H
#define EDIT_MODE_H

#include "core_global.h"
#include "imode.h"

QT_BEGIN_NAMESPACE
class QSplitter;
QT_END_NAMESPACE

namespace Core {
namespace Internal {

class EditMode : public IMode
{
    Q_OBJECT

public:
    EditMode();
    ~EditMode();

private slots:
    void grapFocus(IMode *mode);

private:
    QSplitter *m_splitter;
};

} // namespace Internal
} // namespace Core


#endif // EDIT_MODE_H
