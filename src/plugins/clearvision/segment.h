#ifndef CLEARVISION_SEGMENT_H
#define CLEARVISION_SEGMENT_H

#include <UDOCA/Language.h>

#include <projectexplorer/segment.h>

namespace ClearVision {
namespace Internal {

class Segment : public ProjectExplorer::Segment
{
    Q_OBJECT

public:
    Segment(Core::Id mediaSourceId, const UDOCA::Object &obj);
    Segment(const Segment &that);

    static Segment *create(Core::Id srcId, const UDOCA::Object &obj);

public:
    QString mimeType() const;
    UDOCA::String serverId() const;

protected:
    Segment *clone() const Q_DECL_OVERRIDE;

private:
    QString m_cameraName;
    UDOCA::String m_serverId;
};

} // namespace Internal
} // namespace ClearVision

#endif // CLEARVISION_SEGMENT_H
