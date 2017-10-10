#ifndef KINGVISION_SEGMENT_H
#define KINGVISION_SEGMENT_H

#include <UDOCA/Language.h>

#include <projectexplorer/segment.h>

namespace KingVision {
namespace Internal {

class Segment : public ProjectExplorer::Segment
{
    Q_OBJECT

public:
    Segment(Core::Id mediaSourceId, const UDOCA::Object &obj);
    Segment(const Segment &that);

    static Segment *create(Core::Id srcId, const UDOCA::Object &obj);

protected:
    Segment *clone() const Q_DECL_OVERRIDE;

private:
    bool m_hasPreview;
};

} // namespace Internal
} // namespace KingVision

#endif // KINGVISION_SEGMENT_H
