#ifndef CORE_PLUGIN_H
#define CORE_PLUGIN_H

#include <extensionsystem/iplugin.h>

namespace Core {
namespace Internal {

class EditMode;
class MainWindow;

class CorePlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.bqvision.KingVision.MediaStudio.plugin" FILE "Core.json")

public:
    CorePlugin();
    ~CorePlugin();

    // IPlugin interface
public:
    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    bool delayedInitialize();
    ShutdownFlag aboutToShutdown();
    QObject *remoteCommand(const QStringList &, const QString &, const QStringList &);

public slots:
    void fileOpenRequest(const QString &f);

private:
    void parseArguments(const QStringList &arguments);

private:
    MainWindow *m_mainWindow;
    EditMode *m_editMode;
};

} // namespace Internal
} // namespace Core

#endif // CORE_PLUGIN_H
