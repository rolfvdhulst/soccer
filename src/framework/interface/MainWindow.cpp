//
// Created by rolf on 22-03-20.
//

#include "MainWindow.h"
#include "Visualizer.h"
#include "GameStateVisualizer.h"
#include "MainSettingsWidget.h"

#include <QMenuBar>
#include <QtWidgets/QSplitter>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    int width = 1920;
    int height = 1080;
    setMinimumSize(width/5,height/5);

    visualizer = new Visualizer(this);
    gameStateVisualizer = new GameStateVisualizer(this);
    mainControls = new MainSettingsWidget(this);

    mainLayout = new QVBoxLayout();
    horizontalLayout = new QHBoxLayout();
    sideBarLayout = new QVBoxLayout();

    //setup top menu
    QMenuBar * menu = new QMenuBar(this);
    setMenuBar(menu);
    auto viewMenu = menu->addMenu(tr("&Visualization"));
    configureCheckableMenuItem("Show detections", viewMenu,visualizer,SLOT(setShowDetections(bool)),false);
    configureCheckableMenuItem("Show camera outlines", viewMenu,visualizer,SLOT(setShowCameraOutlines(bool)),false);
    configureCheckableMenuItem("Show placement marker", viewMenu,visualizer,SLOT(setShowPlacementMarker(bool)),true);


    sideBarLayout->addWidget(mainControls);
    sideBarLayout->addWidget(gameStateVisualizer);


    QSplitter * splitter = new QSplitter(this);
    splitter->addWidget(visualizer);
    sideBarWidget = new QWidget();
    sideBarWidget->setLayout(sideBarLayout);
    splitter->addWidget(sideBarWidget);
    splitter->setSizes({600,200});
    horizontalLayout->addWidget(splitter);
    mainLayout->addLayout(horizontalLayout);

    mainWidget = new QWidget();
    setCentralWidget(mainWidget);
    centralWidget()->setLayout(mainLayout);

    showMaximized();
}
void MainWindow::configureCheckableMenuItem(const QString& title, QMenu *menu, const QObject *receiver, const char *method, bool defaultState) {
    QAction *action = new QAction(title, menu);
    action->setCheckable(true);
    action->setChecked(defaultState);
    QObject::connect(action, SIGNAL(triggered(bool)), receiver, method);
    menu->addAction(action);
}

MainWindow::~MainWindow() {
    delete gameStateVisualizer;
    delete visualizer;
    delete horizontalLayout;
    delete mainLayout;
    delete mainWidget;
}
