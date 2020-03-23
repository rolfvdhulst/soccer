//
// Created by rolf on 22-03-20.
//

#include "MainWindow.h"
#include "Visualizer.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    int width = 1920;
    int height = 1080;
    setMinimumSize(width/5,height/5);

    visualizer = new Visualizer(this);
    mainLayout = new QVBoxLayout();
    horizontalLayout = new QHBoxLayout();

    visualizer->setMinimumSize(width/5,height/5);
    visualizer->setMaximumSize(width,height);

    horizontalLayout->addWidget(visualizer);
    mainLayout->addLayout(horizontalLayout);

    mainWidget = new QWidget();
    setCentralWidget(mainWidget);
    centralWidget()->setLayout(mainLayout);

    showMaximized();
}
MainWindow::~MainWindow() {
    delete visualizer;
    delete horizontalLayout;
    delete mainLayout;
    delete mainWidget;
}
