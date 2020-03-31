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
#include <QtWidgets/QGraphicsEllipseItem>


Visualizer::Visualizer(QWidget* parent)
        :QGraphicsView(parent),
        field{FieldState(DefaultField::DivisionA)}
        {//the field constructor above must be the same default constructor as in your ai to make sure they have the same field!
    scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);

    //set anti-aliasing
    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::HighQualityAntialiasing, true);

    updateTimer = new QTimer(this);
    connect(updateTimer,&QTimer::timeout,this,&Visualizer::updateAll);
    updateTimer->start(20); //50hz
    createBall();
    createPlacementMarker();
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

    std::vector<proto::SSL_WrapperPacket> frames = API::instance()->getFramesAndClear();
    updateDetections(frames);

    if(API::instance()->newGeometry()){
        proto::SSL_GeometryData data = API::instance()->readGeometryData();
        field = FieldState(data.field());
        double margin = field.getBoundaryWidth() + 0.1; //boundary width + an extra small margin
        double halfLength = 0.5*field.getFieldLength();
        double halfWidth = 0.5*field.getFieldWidth();
        fieldRect= QRectF(-(halfLength+margin),-(halfWidth+margin), 2*(halfLength+margin),
                2*(halfWidth+margin));
        for (const auto & cam : data.calib()){
            cameras.addCamera(Camera(cam));
        }
    }

    //todo what to do on initialization?
    auto gamestate=API::instance()->getGameState();
    if (gamestate.has_designated_position()){
        placementMarker->setPos(gamestate.designated_position().x(),-gamestate.designated_position().y()); //QT has mirrored y-axis
        placementMarker->show();
    }else{
        placementMarker->hide();
    }
    weAreBlue = gamestate.ourcolor() != proto::Team::YELLOW;
    refitView();
}
Visualizer::~Visualizer() {
    delete updateTimer;
    delete scene;
    delete ball;
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
    double robotAngle = toDegrees(-robot.angle()); //inverted y-axis
    visBot.robot->setPos(robot.pos().x(), -robot.pos().y()); //QT has inverted y-axis
    visBot.robot->setRotation(robotAngle);
    visBot.id->setPos(robot.pos().x(), -robot.pos().y()); //QT has inverted y-axis
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
    if (world.has_ball()){
        ball->setPos(world.ball().pos().x(),-world.ball().pos().y());
        ball->show();
    }else{
        ball->hide();
    }
}
void Visualizer::refitView() {
    setSceneRect(fieldRect);
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
void Visualizer::drawDetectionFrames(QPainter* painter, const std::vector<proto::SSL_DetectionFrame>& frames) {
    for(const auto& frame : frames){
        drawDetectionFrame(painter,frame);
    }
}
void Visualizer::drawDetectionFrame(QPainter* painter, const proto::SSL_DetectionFrame &frame) {
    proto::RobotInfo info;
    info.set_radius(0.09);
    info.set_height(0.15);
    info.set_frontwidth(0.12);
    info.set_dribblerangle(asinf(0.5 * info.frontwidth() / info.radius())); //TODO: fix
    painter->save(); //save so we can restore the state later.
    for(const auto& blueBot : frame.robots_blue()){
        drawDetectionRobot(painter,blueBot,info,Qt::green);
    }
    for(const auto& yellowBot : frame.robots_yellow()){
        drawDetectionRobot(painter,yellowBot,info,Qt::red);
    }
    for(const auto& detBall : frame.balls()){
        drawDetectionBall(painter,detBall);
    }
    painter->restore();
}
void Visualizer::drawDetectionBall(QPainter* painter, const proto::SSL_DetectionBall &detBall) {
    const float radius=0.02133; //TODO: fix
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::cyan);
    painter->setOpacity(0.8);
    painter->drawEllipse(QPointF(detBall.x()/1000.0,-detBall.y()/1000.0),radius,radius);
}
void Visualizer::drawDetectionRobot(QPainter* painter, const proto::SSL_DetectionRobot &bot,
        const proto::RobotInfo &info, const QColor &color) {
    const float radius= info.radius();
    const float startAngle=toDegrees(info.dribblerangle()); // should already be in degrees
    const float endAngle= 360.0-toDegrees(info.dribblerangle());
    const float sweepLength=endAngle-startAngle;
    QRectF rect(-radius, -radius, radius*2,radius*2);
    QPointF botPos(bot.x()/1000.0,-bot.y()/1000.0);//QT has an inverted y-axis
    rect.translate(botPos);
    float trueAngle=startAngle+ toDegrees(bot.orientation());
    QPainterPath path;
    path.arcMoveTo(rect,trueAngle);
    path.arcTo(rect,trueAngle,sweepLength);
    path.closeSubpath();

    QPen pen;
    pen.setWidthF(0.005);
    pen.setColor(Qt::white);
    painter->setPen(pen);
    painter->setBrush(color);
    painter->setOpacity(0.3);
    painter->drawPath(path);

    QFont f("Helvetica");
    f.setPointSizeF(radius);
    painter->setFont(f);
    painter->setPen(Qt::black);
    painter->setOpacity(1.0);
    painter->drawText(botPos+QPointF(-radius*0.4,radius*0.4),QString::number(bot.robot_id()));
}
void Visualizer::drawBackground(QPainter* painter, const QRectF &rect) {
    painter->save();
    if(redrawField){
        drawField(painter);
        redrawField = true; //TODO: figure out why this is so problematic
    }
    painter->restore();
}
void Visualizer::drawForeground(QPainter* painter, const QRectF &rect) {
    if(showDetections){
        drawDetectionFrames(painter,usedDetectionFrames);
    }
}
void Visualizer::updateDetections(const std::vector<proto::SSL_WrapperPacket>& packets) {
    usedDetectionFrames.clear();
    for (const auto& packet : packets){
        if (packet.has_detection()){
            usedDetectionFrames.push_back(packet.detection());
        }
    }
}
void Visualizer::drawField(QPainter* painter) {
    QPen pen;
    pen.setColor(Qt::white);
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::MiterJoin);
    pen.setWidthF(field.getLineThickness());
    painter->setPen(pen);

    for (const LineSegment& line : {field.getLeftLine(),
                                    field.getTopLine(),
                                    field.getRightLine(),
                                    field.getBottomLine(),
                                    field.getHalfLine(),
                                    field.getCenterLine(),
                                    field.getLeftPenaltyLine(),
                                    field.getBottomLeftPenaltyStretch(),
                                    field.getTopLeftPenaltyStretch(),
                                    field.getRightPenaltyLine(),
                                    field.getBottomRightPenaltyStretch(),
                                    field.getTopRightPenaltyStretch()
    }){
        QLineF graphicLine(line.start.x, -line.start.y, line.end.x, -line.end.y);//QT mirrors y-axis
        painter->drawLine(graphicLine);
    }
    for (const Circle& circle : {field.getCenterCircle()}){
        double radius = circle.radius-field.getLineThickness();//Center circle has already larger radius to account for lineWidth
        painter->drawEllipse(QPointF(circle.center.x,-circle.center.y),radius,radius);
    }
    pen.setWidthF(field.getLineThickness()*0.3);
    painter->setPen(pen);
    const Rectangle& rect=field.getMarginField();
    QRectF drawnRect(QPointF(rect.minX(),-rect.minY()),QPointF(rect.maxX(),-rect.maxY())); //QT mirrors y-axis
    painter->drawRect(drawnRect);
    drawGoal(painter,true);
    drawGoal(painter,false);
}
void Visualizer::createBall() {
    const float ballRadius = 0.021333f;
    const float attentionRadius = field.getFieldLength()*0.015f+ballRadius;
    ball = new Ball();
    ball->actual = new QGraphicsEllipseItem();
    ball->actual->setPen(Qt::NoPen);
    ball->actual->setBrush(QColor(255,140,0));
    ball->actual->setZValue(100.0);//TODO: collect and organise
    ball->actual->setRect(QRectF(-ballRadius,-ballRadius,ballRadius*2,ballRadius*2));
    ball->attentionCircle = new QGraphicsEllipseItem();
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidthF(0.01);
    ball->attentionCircle->setPen(pen);
    ball->attentionCircle->setBrush(Qt::NoBrush);
    ball->attentionCircle->setOpacity(0.5);
    ball->attentionCircle->setZValue(40.0);//TODO: collect and organise
    ball->attentionCircle->setRect(QRectF(-attentionRadius,-attentionRadius,attentionRadius*2,attentionRadius*2));
    ball->noBallWarning = new QGraphicsSimpleTextItem("NO BALL");
    ball->noBallWarning->setFont(QFont("ubuntu",1));
    pen.setColor(Qt::red);
    ball->noBallWarning->setPen(pen);
    ball->hide();
    scene->addItem(ball->actual);
    scene->addItem(ball->attentionCircle);
    scene->addItem(ball->noBallWarning);
}
void Visualizer::resizeEvent(QResizeEvent* event) {
    refitView();
}
void Visualizer::setShowDetections(bool show) {this->showDetections = show; }
void Visualizer::createPlacementMarker() {
    placementMarker = new PlacementMarker();
    const float radius= 2*0.021333; //2x ball radius cross
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidthF(0.01);
    placementMarker->line1 = new QGraphicsLineItem(-radius,-radius,radius,radius);
    placementMarker->line2 = new QGraphicsLineItem(-radius,radius,radius,-radius);
    placementMarker->line1->setPen(pen);
    placementMarker->line2->setPen(pen);
    const float goodRange = 0.15;
    placementMarker->goodRadius = new QGraphicsEllipseItem();
    QPen pen2;
    pen2.setColor(Qt::darkGreen);
    pen2.setWidthF(0.01);
    placementMarker->goodRadius->setPen(pen2);
    placementMarker->goodRadius->setBrush(Qt::NoBrush);
    placementMarker->goodRadius->setOpacity(0.8);
    placementMarker->goodRadius->setZValue(40.0);//TODO: collect and organise
    placementMarker->goodRadius->setRect(QRectF(-goodRange,-goodRange,goodRange*2,goodRange*2));

    placementMarker->hide();

    scene->addItem(placementMarker->line1);
    scene->addItem(placementMarker->line2);
    scene->addItem(placementMarker->goodRadius);
}
void Visualizer::drawGoal(QPainter* painter, bool isLeft) {
    bool colorIsYellow = isLeft ^ weAreBlue;
    auto color = colorIsYellow ?  QColor(255, 255, 0, 255) : QColor(80, 80, 255, 255);
    QPen pen;
    pen.setColor(color);
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::MiterJoin);
    QPainterPath path;
    double side = isLeft ? -1.0 : 1.0;
    double d = field.getGoalWallThickness()* 0.5;
    double l = field.getFieldLength() * 0.5;
    double w = field.getGoalWidth()* 0.5 + d; //it's drawn using symmetry so we don't have to mirror the y-axis (it does not matter)
    pen.setWidthF(2*d);
    painter->setPen(pen);
    path.moveTo(side * l, w);
    path.lineTo(side * (l + field.getGoalDepth() + d), w);
    path.lineTo(side * (l + field.getGoalDepth() + d), -w);
    path.lineTo(side * l, -w);
    painter->drawPath(path);
}
void Visualizer::Ball::show() {
    actual->show();
    attentionCircle->show();
    noBallWarning->hide();
}
void Visualizer::Ball::hide() {
    actual->hide();
    attentionCircle->hide();
    noBallWarning->show();
}
void Visualizer::Ball::setPos(qreal x, qreal y) {
    actual->setPos(x,y);
    attentionCircle->setPos(x,y);
}
void Visualizer::PlacementMarker::setPos(qreal x, qreal y) {
    line1->setPos(x,y);
    line2->setPos(x,y);
    goodRadius->setPos(x,y);
}
void Visualizer::PlacementMarker::hide() {
    line1->hide();
    line2->hide();
    goodRadius->hide();
}
void Visualizer::PlacementMarker::show() {
    line1->show();
    line2->show();
    goodRadius->show();
}
