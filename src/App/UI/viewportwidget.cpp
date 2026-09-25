#include "viewportwidget.h"
#include <QOpenGLContext>
#include "renderer.h"

ViewportWidget::ViewportWidget(QWidget* parent, Qt::WindowFlags f)
    : QOpenGLWidget(parent, f) { }


void ViewportWidget::initializeGL()
{
    // Core's GLAD resolves its GL function pointers through Qt's context
    const bool ok = sparkflow::render::InitializeGL([](const char* name) {
        return QOpenGLContext::currentContext()->getProcAddress(name);
        });
}

void ViewportWidget::paintGL()
{
    // Qt has made the widget's context current and bound its FBO, so Render() draws into the viewport
    sparkflow::render::Render();
}