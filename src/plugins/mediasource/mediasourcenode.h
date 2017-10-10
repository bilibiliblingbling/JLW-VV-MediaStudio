#ifndef MEDIASOURCE_MEDIASOURCE_NODE_H
#define MEDIASOURCE_MEDIASOURCE_NODE_H

#include <projectexplorer/basicnodes.h>
#include <projectexplorer/mediasource.h>

namespace MediaSource {
namespace Internal {

namespace PE = ProjectExplorer;

class MediaSourceNode : public PE::Node
{
public:
    MediaSourceNode(PE::MediaSource *source);

public:
    QString displayName() const Q_DECL_OVERRIDE;
    bool enabled() const Q_DECL_OVERRIDE;
    QIcon icon() const Q_DECL_OVERRIDE;

public:
    PE::MediaSource *metadata() const Q_DECL_OVERRIDE;

private:
    PE::MediaSource *m_mediasource;
};

} // namespace Internal
} // namespace MediaSource


#endif // MEDIASOURCE_MEDIASOURCE_NODE_H
