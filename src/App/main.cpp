#include "mainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>
#include <QCommandLineParser>
#include <QDebug>

#include "log.h"

int main(int argc, char *argv[])
{
    // Request a 4.6 core context, must run before QApplication is created
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setVersion(4, 6);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    const QCommandLineOption logOption("log", "Log level: off, error, warning, info, debug or trace.", "level", "info");
    parser.addOption(logOption);
    parser.process(app);

    const auto logLevel = sfx::log::parseLevel(parser.value(logOption).toStdString());
    if (!logLevel) {
        qCritical().noquote() << "Invalid log level:" << parser.value(logOption);
        return 1;
    }
    sfx::log::setLevel(*logLevel);

    if (*logLevel > sfx::log::kCompiledLevel) {
        SFX_LOG_WARNING("log level '{}' exceeds the compiled ceiling '{}'",
            sfx::log::toString(*logLevel), sfx::log::toString(sfx::log::kCompiledLevel));
    }


    MainWindow w;
    w.show();
    return QApplication::exec();
}