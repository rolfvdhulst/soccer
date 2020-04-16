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

    auto replayMenu = menu->addMenu(tr("&Replay"));
    auto action = replayMenu->addAction(tr("&Open"));
    connect(action,SIGNAL(triggered()),mainControls->getReplayWidget(),SLOT(openFile()));
    auto action2 = replayMenu->addAction(tr("Save backlog"));
    connect(action2,SIGNAL(triggered()),mainControls,SLOT(saveBacklog()));
    sideBarLayout->addWidget(mainControls);
    sideBarLayout->addWidget(gameStateVisualizer);

    //Connect replay information to relevant widgets
    ReplayWidget * replayWidget = mainControls->getReplayWidget();
    connect(replayWidget,SIGNAL(gotLogFrame(const proto::FrameLog&)),visualizer,SLOT(updateSingleFrame(const proto::FrameLog&)));
    connect(replayWidget,SIGNAL(gotLogFrame(const proto::FrameLog&)),gameStateVisualizer,SLOT(updateFrame(const proto::FrameLog&)));
    connect(replayWidget,SIGNAL(gotLogFrame(const proto::FrameLog&)),mainControls,SLOT(visualizeFrame(const proto::FrameLog&)));
    connect(replayWidget,&ReplayWidget::replayActive,gameStateVisualizer->getGameEventsWidget(),&GameEventsWidget::setReplay);

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

    if(API::instance()->hasCompletedFirstTick() && !mainControls->getReplayWidget()->isRunning()){
        std::vector<proto::FrameLog> frameLogs = API::instance()->getData();
        for(const auto& frame : frameLogs){
            //Technically these only need to be the last frame
            mainControls->updateFrame(frame);
            gameStateVisualizer->updateFrame(frame);
        }
        visualizer->updateFrames(frameLogs);
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
