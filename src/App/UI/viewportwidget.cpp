#include "viewportwidget.h"

#include <algorithm>
#include <QtLogging>
#include <QOpenGLContext>
#include <QSurfaceFormat>

// TODO: make a single header directory that contains the API for app
#include "rendering/viewportRenderer.h"
#include "simulation.h"

ViewportWidget::ViewportWidget(QWidget* parent, Qt::WindowFlags f)
    : QOpenGLWidget(parent, f)
    , m_sim{ std::make_unique<sparkflow::simulation::Simulator>() }
    , m_vpRenderer{ std::make_unique<sparkflow::render::ViewportRenderer>() }
{
    connect(this, &QOpenGLWidget::frameSwapped, this, [this] { update(); });
}

ViewportWidget::~ViewportWidget()
{
    makeCurrent();
    m_vpRenderer.reset();
    doneCurrent();
}

void ViewportWidget::initializeGL()
{
    // Fail fast if the driver didn't provide the requested profile and at least the requested version
    const QSurfaceFormat requested = QSurfaceFormat::defaultFormat();
    const QSurfaceFormat actual = context()->format();

    if (actual.profile() != requested.profile() || actual.version() < requested.version())
    {
        qFatal("Requested OpenGL %d.%d core, but the driver provided %d.%d",
            requested.majorVersion(), requested.minorVersion(),
            actual.majorVersion(), actual.minorVersion());
    }

    const bool ok = m_vpRenderer->Initialize([](const char* name) {
        return QOpenGLContext::currentContext()->getProcAddress(name);
        });

    if (!ok)
        qFatal("Failed to initialize the viewport renderer");

    m_lastTick = std::chrono::steady_clock::now();
}

void ViewportWidget::paintGL()
{
    advanceSimulation();

    // Qt has made the widget's context current and bound its FBO, so Render() draws into the viewport
    m_vpRenderer->Render(m_sim->particlesView());
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