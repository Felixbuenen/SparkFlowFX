#include "viewportwidget.h"

#include <algorithm>
#include <QOpenGLContext>
#include <QtLogging>

#include "renderer.h"
#include "simulation.h"

ViewportWidget::ViewportWidget(QWidget* parent, Qt::WindowFlags f)
    : QOpenGLWidget(parent, f)
    , m_sim{ std::make_unique<sparkflow::simulation::Simulator>()}
{
    connect(this, &QOpenGLWidget::frameSwapped, this, [this] { update(); });
}

ViewportWidget::~ViewportWidget() = default;

void ViewportWidget::initializeGL()
{
    // check if the actual context is equal to the requested context
    const QSurfaceFormat requested = QSurfaceFormat::defaultFormat();
    const QSurfaceFormat actual = context()->format();

    if (actual.profile() != requested.profile() || actual.version() < requested.version())
    {
        qFatal("Requested OpenGL %d.%d core, but the driver provided %d.%d",
            requested.majorVersion(), requested.minorVersion(),
            actual.majorVersion(), actual.minorVersion());
    }

    // Core's GLAD resolves its GL function pointers through Qt's context
    const bool ok = sparkflow::render::LoadGL([](const char* name) {
        return QOpenGLContext::currentContext()->getProcAddress(name);
        });

    if (!ok)
        qFatal("Failed to load OpenGL functions");

    m_lastTick = std::chrono::steady_clock::now();
}

void ViewportWidget::paintGL()
{
    advanceSimulation();

    // Qt has made the widget's context current and bound its FBO, so Render() draws into the viewport
    sparkflow::render::Render();
}

void ViewportWidget::advanceSimulation()
{
    using namespace std::chrono;

    constexpr int maxFramesPerUpdate = 5;
    const auto frameInterval = duration_cast<nanoseconds>(
        duration<double>{ 1.0 / m_sim->config().framesPerSecond });

    const auto now = steady_clock::now();
    m_accumulator = std::min<nanoseconds>(m_accumulator + (now - m_lastTick), frameInterval * maxFramesPerUpdate);
    m_lastTick = now;

    while (m_accumulator >= frameInterval)
    {
        m_sim->advanceFrame();
        m_accumulator -= frameInterval;
    }
}