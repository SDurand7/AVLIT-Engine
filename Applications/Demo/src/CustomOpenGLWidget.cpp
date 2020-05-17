#include "CustomOpenGLWidget.hpp"
#include <Applications/Demo/include/MainWindow.hpp>

#include <QOpenGLContext>

#include <Core/Base/include/Scene.hpp>


CustomOpenGLWidget::CustomOpenGLWidget(QWidget *parent) : QOpenGLWidget{parent} {}

CustomOpenGLWidget::~CustomOpenGLWidget() {}

void CustomOpenGLWidget::initializeGL() {
    connect(context(), SIGNAL(aboutToBeDestroyed()), this, SLOT(release()));
    ALIT::Engine::createInstance();
    emit contextCreated();
}

void CustomOpenGLWidget::resizeGL(int w, int h) { ALIT::Engine::instance()->scene()->resize(w, h); }

void CustomOpenGLWidget::paintGL() { ALIT::Engine::instance()->scene()->render(); }

void CustomOpenGLWidget::release() { ALIT::Engine::deleteInstance(); }
