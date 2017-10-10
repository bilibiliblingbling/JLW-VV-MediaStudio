#ifndef PLACE_HOLDER_H
#define PLACE_HOLDER_H

#include <QSplitter>
#include <QSplitterHandle>
#include <QVBoxLayout>

#include "core_global.h"
#include "modemanager.h"

namespace Core {

template<class T>
class CORE_EXPORT PlaceHolder : public QWidget
{
    friend T;   // C++11

public:
    explicit PlaceHolder(IMode *mode, QWidget *parent = 0)
        : QWidget(parent)
        , m_mode(mode)
    {
        setLayout(new QVBoxLayout());
        layout()->setMargin(0);

        connect(ModeManager::instance(), &ModeManager::currentModeChanged,
                this, &PlaceHolder::currentModeChanged);
    }

    ~PlaceHolder()
    {
        if (m_current == this) {
            T::instance()->setParent(0);
            T::instance()->hide();
        }
    }

public:
    static PlaceHolder *current()
    {
        return m_current;
    }

public:
    void applyStoredSize(int size)
    {
        if (size) {
            QSplitter *splitter = qobject_cast<QSplitter *>(parentWidget());
            if (splitter) {
                // A splitter we need to resize the splitter sizes
                QList<int> sizes = splitter->sizes();
                int index = splitter->indexOf(this);
                int diff = size - sizes.at(index);
                int adjust = (sizes.count() > 1)? (diff / (sizes.count() - 1)): 0;
                for (int i = 0; i < sizes.count(); ++i) {
                    if (i != index)
                        sizes[i] += adjust;
                }
                sizes[index]= size;
                splitter->setSizes(sizes);
            } else {
                QSize s = QWidget::size();

                if (T::instance()->orientation() == Qt::Horizontal)
                    s.setWidth(size);

                if (T::instance()->orientation() == Qt::Vertical)
                    s.setHeight(size);

                resize(s);
            }
        }
    }

protected:
    QSize sizeHint() const
    {
        return T::instance()->sizeHint();
    }

private:
    void currentModeChanged(IMode *mode)
    {
        auto widget = T::instance();

        if (m_current == this) {
            m_current = 0;
            widget->setParent(0);
            widget->hide();
            widget->placeHolderChanged(m_current);
        }

        if (m_mode == mode) {
            m_current = this;

#if 1
            layout()->addWidget(widget);
#else
            qobject_cast<QVBoxLayout *>(layout())->addWidget(widget, 1, Qt::AlignTop);
#endif
            widget->show();

            int width = widget->storedSize();
            applyStoredSize(width);
            setVisible(widget->isShown());
            widget->placeHolderChanged(m_current);
        }
    }

private:
    IMode *m_mode;
    static PlaceHolder *m_current;
};

template<class T>
PlaceHolder<T> *PlaceHolder<T>::m_current = 0;

} // namespace Core

#endif // PLACE_HOLDER_H
