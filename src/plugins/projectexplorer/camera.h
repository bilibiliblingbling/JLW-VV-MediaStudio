#ifndef CAMERA_H
#define CAMERA_H

#include "metadata.h"

namespace ProjectExplorer {

class PROJECTEXPLORER_EXPORT Camera : public MetaData
{
    Q_OBJECT

public:
    Camera(QObject *parent = 0);
    virtual ~Camera();

public:
    virtual QString name() const = 0;

public:
    QString mimeType() const Q_DECL_OVERRIDE;
};

} // namespace ProjectExplorer

#endif // CAMERA_H
