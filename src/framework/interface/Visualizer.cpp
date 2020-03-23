//
// Created by rolf on 22-03-20.
//

#include "Visualizer.h"
#include <QTimer>
#include <protobuf/RobotInfo.pb.h>
#include <QGraphicsItem>
#include <geometry/Angle.h>
#include <interfaceAPI/API.h>
#include <QWheelEvent>

Visualizer::Visualizer(QWidget* parent)
        :QGraphicsView(parent) {
    scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);

    //set anti-aliasing
    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::HighQualityAntialiasing, true);

    updateTimer = new QTimer(this);
    connect(updateTimer,&QTimer::timeout,this,&Visualizer::updateAll);
    updateTimer->start(20);

    double margin = 0.3 + 0.1; //boundary width + an extra small margin
    double halfLength = 6.0;
    double halfWidth = 4.5;
    fieldRect= QRectF(-(halfLength+margin),-(halfWidth+margin), 2*(halfLength+margin),
            2*(halfWidth+margin));
    setSceneRect(fieldRect);
    refitView();
}

void Visualizer::updateAll() {
    proto::World worldState = API::instance()->getWorldState();
    updateWorld(worldState);
    refitView();
}
Visualizer::~Visualizer() {
    delete updateTimer;
    delete scene;
}
void Visualizer::Robot::show() {
    visible = true;
    robot->show();
    id->show();
}
void Visualizer::Robot::tryHide() {
    if(!visible){
        robot->hide();
        id->hide();
    }
    visible = false;
}
void
Visualizer::updateRobot(const proto::WorldRobot &robot, QMap<uint, Robot> &robots, const proto::RobotInfo &info,
        const QColor &color) {
    Robot &visBot = robots[robot.id()];
    // if the bot does not exist we create it
    if (!visBot.robot) {
        visBot.robot = new QGraphicsPathItem();

        const double angle = toDegrees(info.dribblerangle());
        const double radius = info.radius();
        // create body:
        visBot.robot->setPen(Qt::NoPen);
        visBot.robot->setBrush(color);
        const QRectF rect(-radius, -radius, radius * 2.0, radius * 2.0); //box containing Robot
        QPainterPath path;
        path.arcMoveTo(rect, angle);
        path.arcTo(rect, angle, 360.0 - 2.0 * angle);
        path.closeSubpath();
        visBot.robot->setPath(path);
        // make the id appear on the robot
        QGraphicsSimpleTextItem *text = new QGraphicsSimpleTextItem(QString::number(robot.id()));
        text->setTransform(QTransform::fromTranslate(-0.7 * radius, -0.7* radius), true);
        text->setTransform(QTransform::fromScale(0.014, 0.014), true);
        text->setFont(QFont("ubuntu", 7));
        visBot.id = text;
        // make sure the object overlay correctly
        visBot.robot->setZValue(5.0); //TODO: store these somewhere centrally
        visBot.id->setZValue(6.0);
        visBot.id->setCacheMode(QGraphicsItem::DeviceCoordinateCache);// cache the id since it is always the same'
        //add the new robot to the scene
        scene->addItem(visBot.robot);
        scene->addItem(visBot.id);
    }
    double robotAngle = toDegrees(robot.angle());
    visBot.robot->setPos(robot.pos().x(), robot.pos().y());
    visBot.robot->setRotation(robotAngle);
    visBot.id->setPos(robot.pos().x(), robot.pos().y());
    visBot.show();
}
void Visualizer::updateWorld(const proto::World &world) {
    proto::RobotInfo info;
    info.set_radius(0.09);
    info.set_height(0.15);
    info.set_frontwidth(0.12);
    info.set_dribblerangle(asinf(0.5 * info.frontwidth() / info.radius()));
    for (const auto &blueBot : world.blue()){
        updateRobot(blueBot,blueBots,info,Qt::blue);
    }
    for (const auto & yellowBot : world.yellow()){
        updateRobot(yellowBot,yellowBots,info,Qt::yellow);
    }

    for (auto & robot : blueBots) {
        robot.tryHide();
    }
    for (auto & robot : yellowBots) {
        robot.tryHide();
    }
}
void Visualizer::refitView() {
    fitInView(fieldRect);
}
void Visualizer::wheelEvent(QWheelEvent *event) {
    //Todo: add minimum and maximum zoom
    double zoomConstant =1.25;
    double zoomFactor=pow(zoomConstant,event->delta()/240.0);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse); //zoom in at the point the mouse is at
    const QPointF &oldPos = mapToScene(event->pos());
    scale(zoomFactor,zoomFactor);
}