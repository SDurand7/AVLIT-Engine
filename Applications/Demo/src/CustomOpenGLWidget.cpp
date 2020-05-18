#include "CustomOpenGLWidget.hpp"
#include <Applications/Demo/include/MainWindow.hpp>

#include <QOpenGLContext>

#include <Core/Base/include/Scene.hpp>


CustomOpenGLWidget::CustomOpenGLWidget(QWidget *parent) : QOpenGLWidget{parent} {}

CustomOpenGLWidget::~CustomOpenGLWidget() {}

void CustomOpenGLWidget::initializeGL() {
    connect(context(), SIGNAL(aboutToBeDestroyed()), this, SLOT(release()));
    AVLIT::Engine::createInstance();
    emit contextCreated();
}

void CustomOpenGLWidget::resizeGL(int w, int h) { AVLIT::Engine::instance()->scene()->resizeCanvas(w, h); }

void CustomOpenGLWidget::paintGL() { AVLIT::Engine::instance()->scene()->render(); }

void CustomOpenGLWidget::release() { AVLIT::Engine::deleteInstance(); }
