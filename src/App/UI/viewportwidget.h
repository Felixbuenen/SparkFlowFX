#pragma once

#include <QOpenGLWidget>
#include "simulation.h"

#include <chrono>
#include <memory>

namespace sparkflow::simulation { class Simulator; }

class ViewportWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    ViewportWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~ViewportWidget();

protected:
    void initializeGL() override;
    void paintGL() override;

private:
    void advanceSimulation();

    std::unique_ptr<sparkflow::simulation::Simulator> m_Sim;
    std::chrono::steady_clock::time_point m_LastTick;
    std::chrono::nanoseconds m_Accumulator{};
};
