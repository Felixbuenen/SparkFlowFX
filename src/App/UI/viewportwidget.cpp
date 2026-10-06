#include "viewportwidget.h"

#include <algorithm>
#include <QOpenGLContext>

#include "renderer.h"
#include "simulation.h"

ViewportWidget::ViewportWidget(QWidget* parent, Qt::WindowFlags f)
    : QOpenGLWidget(parent, f)
    , m_Sim{ std::make_unique<sparkflow::simulation::Simulator>() }
{
    connect(this, &QOpenGLWidget::frameSwapped, this, [this] { update(); });
}

ViewportWidget::~ViewportWidget() = default;

void ViewportWidget::initializeGL()
{
    // Core's GLAD resolves its GL function pointers through Qt's context
    const bool ok = sparkflow::render::InitializeGL([](const char* name) {
        return QOpenGLContext::currentContext()->getProcAddress(name);
        });

    m_LastTick = std::chrono::steady_clock::now();
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

    constexpr int maxStepsPerFrame = 5;
    const nanoseconds stepInterval = m_Sim->config().updateInterval;
    const float dt = duration<float>(stepInterval).count();

    const auto now = steady_clock::now();
    m_Accumulator = std::min(m_Accumulator + (now - m_LastTick), stepInterval * maxStepsPerFrame);
    m_LastTick = now;

    while (m_Accumulator >= stepInterval)
    {
        m_Sim->step(dt);
        m_Accumulator -= stepInterval;
    }
}