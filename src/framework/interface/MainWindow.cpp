//
// Created by rolf on 22-03-20.
//

#include "MainWindow.h"
#include "Visualizer.h"
#include "GameStateVisualizer.h"
#include "MainSettingsWidget.h"

#include <QMenuBar>
#include <QtWidgets/QSplitter>
#include <interfaceAPI/API.h>
#include <interfaceAPI/SettingsAPI.h>

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
    updateTimer = new QTimer();
    connect(updateTimer,&QTimer::timeout,this,&MainWindow::updateAll);
    updateTimer->start(20);

}
void MainWindow::updateAll() {
    SettingsAPI::instance()->setSettings(mainControls->getSettings());

    if(API::instance()->hasCompletedFirstTick()){
        proto::GameState gameState = API::instance()->getGameState();
        mainControls->updateNormal(gameState);
        visualizer->updateGameState(gameState);

        proto::World worldState = API::instance()->getWorldState();
        visualizer->updateWorld(worldState);

        std::vector<proto::SSL_WrapperPacket> frames = API::instance()->getFramesAndClear();
        visualizer->updateDetections(frames);

        if(API::instance()->newGeometry()){
            proto::SSL_GeometryData data = API::instance()->readGeometryData();
            visualizer->updateGeometryData(data);
        }
    }

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
