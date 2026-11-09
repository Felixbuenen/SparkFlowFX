#include "mainWindow.h"

#include "ui_mainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_ui{ std::make_unique<Ui::MainWindow>() }
{
    m_ui->setupUi(this);
    showMaximized();
}

MainWindow::~MainWindow() = default;