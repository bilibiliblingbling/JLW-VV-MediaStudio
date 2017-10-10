#ifndef KINGVISION_DEVICES_H
#define KINGVISION_DEVICES_H

#include <UDOCA/Language.h>

#include <projectexplorer/camera.h>

namespace KingVision {
namespace Internal {

class Device
{
public:
    Device(const UDOCA::Object &obj);
    virtual ~Device();

protected:
    QString safeStringProperty(const char *property) const;

protected:
    UDOCA::Object m_object;
};


class Codec : public ProjectExplorer::MetaData, public Device
{
    Q_OBJECT

public:
    Codec(const UDOCA::Object &obj);

    static Codec *create(const UDOCA::Object &obj);

public:
    QString name() const;
    QString description() const;

    QString modelId() const;
};

class Camera : public ProjectExplorer::Camera, public Device
{
    Q_OBJECT

public:
    Camera(const UDOCA::Object &obj);

    static Camera *create(const UDOCA::Object &obj);

public:
    QString name() const;

public:
    Core::Id codecId() const;

    qint32 videoPort() const;
};

} // namespace Internal
} // namespace KingVision

#endif // KINGVISION_DEVICES_H
