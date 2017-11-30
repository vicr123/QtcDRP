#pragma once

#include "qtcreatordiscordrichpresence_global.h"

#include <extensionsystem/iplugin.h>
#include <discord-rpc.h>
#include <QAction>

namespace QtCreatorDiscordRichPresence {
    namespace Internal {

        class QtCreatorDiscordRichPresencePlugin : public ExtensionSystem::IPlugin
        {
                Q_OBJECT
                Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "QtCreatorDiscordRichPresence.json")

            public:
                QtCreatorDiscordRichPresencePlugin();
                ~QtCreatorDiscordRichPresencePlugin();

                bool initialize(const QStringList &arguments, QString *errorString);
                void extensionsInitialized();
                ShutdownFlag aboutToShutdown();

            private:
                QAction* rpAction;

                void initDiscord();
        };

    } // namespace Internal
} // namespace QtCreatorDiscordRichPresence
