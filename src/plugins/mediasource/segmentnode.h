#ifndef MEDIASOURCE_SEGMENT_NODE_H
#define MEDIASOURCE_SEGMENT_NODE_H

#include <projectexplorer/basicnodes.h>
#include <projectexplorer/segment.h>

namespace MediaSource {
namespace Internal {

namespace PE = ProjectExplorer;

class SegmentNode : public PE::Node
{
public:
    SegmentNode(PE::Segment *segment);

public:
    QString displayName() const Q_DECL_OVERRIDE;
    QIcon icon() const Q_DECL_OVERRIDE;

public:
    PE::Segment *metadata() const Q_DECL_OVERRIDE;

private:
    PE::Segment *m_segment;
};

} // namespace Internal
} // namespace MediaSource

#endif // MEDIASOURCE_SEGMENT_NODE_H
