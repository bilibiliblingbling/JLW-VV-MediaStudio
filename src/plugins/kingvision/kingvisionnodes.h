#ifndef KINGVISION_NODES_H
#define KINGVISION_NODES_H

#include <projectexplorer/basicnodes.h>

#include "devices.h"

namespace KingVision {
namespace Internal {

class CodecNode : public ProjectExplorer::FolderNode
{
public:
    CodecNode(Codec *codec);

public:
    QString displayName() const Q_DECL_OVERRIDE;
    QString description() const;

public:
    QIcon icon() const Q_DECL_OVERRIDE;

public:
    Codec *metadata() const Q_DECL_OVERRIDE;

private:
    Codec *m_codec;
};


class CameraNode : public ProjectExplorer::FolderNode
{
public:
    CameraNode(Camera *camera);

public:
    QString displayName() const Q_DECL_OVERRIDE;
    QIcon icon() const Q_DECL_OVERRIDE;

public:
    Camera *metadata() const Q_DECL_OVERRIDE;

private:
    Camera *m_camera;
};

} // namespace Internal
} // namespace KingVision

#endif // KINGVISION_NODES_H
