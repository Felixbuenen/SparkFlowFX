#pragma once

#include <chrono>
#include <memory>

#include <QOpenGLWidget>

#include "rendering/camera.h"

namespace sfx::simulation {
    class Simulator;
} // namespace sfx::simulation

namespace sfx::rendering {
    class ViewportRenderer;
} // namespace sfx::rendering

class ViewportWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit ViewportWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~ViewportWidget() override;

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

private:
    void advanceSimulation();

    std::unique_ptr<sfx::simulation::Simulator> m_sim;
    std::unique_ptr<sfx::rendering::ViewportRenderer> m_vpRenderer;
    std::chrono::steady_clock::time_point m_lastTick;
    std::chrono::nanoseconds m_accumulator{};

    sfx::rendering::Camera m_camera;
};