#ifndef VIEWPORTWIDGET_H
#define VIEWPORTWIDGET_H

#include <QOpenGLWidget>

class ViewportWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    ViewportWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

protected:
    void initializeGL() override;
    void paintGL() override;
};

#endif // VIEWPORTWIDGET_H
