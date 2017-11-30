#include "qtcreatordiscordrichpresenceplugin.h"
#include "qtcreatordiscordrichpresenceconstants.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/editormanager/ieditor.h>

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>
#include <QDebug>

namespace QtCreatorDiscordRichPresence {
    namespace Internal {

        QtCreatorDiscordRichPresencePlugin::QtCreatorDiscordRichPresencePlugin()
        {
            // Create your members
        }

        QtCreatorDiscordRichPresencePlugin::~QtCreatorDiscordRichPresencePlugin()
        {
            // Unregister objects from the plugin manager's object pool
            // Delete members
        }

        bool QtCreatorDiscordRichPresencePlugin::initialize(const QStringList &arguments, QString *errorString)
        {
            // Register objects in the plugin manager's object pool
            // Load settings
            // Add actions to menus
            // Connect to other plugins' signals
            // In the initialize function, a plugin can be sure that the plugins it
            // depends on have initialized their members.

            Q_UNUSED(arguments)
            Q_UNUSED(errorString)

            /*rpAction = new QAction(tr("Disable Rich Presence"), this);
            Core::Command *cmd = Core::ActionManager::registerAction(rpAction, Constants::ACTION_ID, Core::Context(Core::Constants::C_GLOBAL));
            connect(rpAction, &QAction::triggered, [=] {

            });

            Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);
            menu->menu()->setTitle(tr("Discord Rich Presence"));
            menu->addAction(cmd);
            Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);*/

            //Register Discord Rich Presence
            initDiscord();
            DiscordRichPresence presence;
            memset(&presence, 0, sizeof(presence));
            presence.state = "Idle";
            presence.details = "Startup Screen";
            presence.largeImageKey = "qtcreator-logo";
            presence.largeImageText = "Working with Qt Creator";
            presence.instance = 1;
            Discord_UpdatePresence(&presence);

            //Connect to file editing signals
            connect(Core::EditorManager::instance(), &Core::EditorManager::currentEditorChanged, [=](Core::IEditor *editor) {
                //qDebug() << editor->document()->filePath().fileName();

                DiscordRichPresence presence;
                memset(&presence, 0, sizeof(presence));

                if (editor->isDesignModePreferred()) {
                    presence.state = "Designing some UI";
                } else {
                    presence.state = "Editing a file";
                }

                //Depending on file extension choose an icon
                QString mimeType = editor->document()->mimeType();
                qDebug() << mimeType;

                const char *smallImageKey = "", *smallImageText;
                if (mimeType == "text/x-c++hdr") { //C++ Header
                    smallImageKey = "file-cpphdr";
                    smallImageText = "C++ Header";
                } else if (mimeType == "text/x-c++src") { //C++ Source
                    smallImageKey = "file-cppsrc";
                    smallImageText = "C++ Source";
                } else if (mimeType == "application/vnd.qt.qmakeprofile") { //QMake Profile
                    smallImageKey = "file-qtprj";
                    smallImageText = "QMake Project Profile";
                } else if (mimeType == "application/x-designer") { //Designer
                    smallImageKey = "file-qtui";
                    smallImageText = "Qt User Interface File";
                }

                if (smallImageKey != nullptr && smallImageKey[0] == '\0') {
                    presence.largeImageKey = "qtcreator-logo";
                    presence.largeImageText = "Working with Qt Creator";
                } else {
                    presence.largeImageKey = smallImageKey;
                    presence.largeImageText = smallImageText;
                    presence.smallImageKey = "qtcreator-logo-small";
                    presence.smallImageText = "Working with Qt Creator";
                }

                presence.details = editor->document()->filePath().fileName().toUtf8().constData();
                presence.instance = 1;
                Discord_UpdatePresence(&presence);
            });

            return true;
        }

        void QtCreatorDiscordRichPresencePlugin::extensionsInitialized()
        {
            // Retrieve objects from the plugin manager's object pool
            // In the extensionsInitialized function, a plugin can be sure that all
            // plugins that depend on it are completely initialized.
        }

        ExtensionSystem::IPlugin::ShutdownFlag QtCreatorDiscordRichPresencePlugin::aboutToShutdown()
        {
            // Save settings
            // Disconnect from signals that are not needed during shutdown
            // Hide UI (if you add UI that is not in the main window directly)

            Discord_Shutdown();
            return SynchronousShutdown;
        }
        void QtCreatorDiscordRichPresencePlugin::initDiscord() {
            DiscordEventHandlers handlers;
            memset(&handlers, 0, sizeof(handlers));
            handlers.ready = [] {
                qDebug() << "Discord Ready!";
            };
            handlers.errored = [](int errorCode, const char* message) {
                qDebug() << "Discord Error!";
            };
            handlers.disconnected = [](int errorCode, const char* message) {
                qDebug() << "Discord Disconnected!";
            };
            Discord_Initialize("385624695229120519", &handlers, true, nullptr);

        }

    } // namespace Internal
} // namespace QtCreatorDiscordRichPresence
