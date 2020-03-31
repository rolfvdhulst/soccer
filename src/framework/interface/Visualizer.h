//
// Created by rolf on 22-03-20.
//

#ifndef SOCCER_VISUALIZER_H
#define SOCCER_VISUALIZER_H
#include <QGraphicsView>
#include <QtCore/QMap>
#include <protobuf/WorldRobot.pb.h>
#include <protobuf/RobotInfo.pb.h>
#include <protobuf/World.pb.h>
#include <protobuf/messages_robocup_ssl_detection.pb.h>
#include <protobuf/messages_robocup_ssl_wrapper.pb.h>
#include <field/FieldState.h>
#include <field/CameraMap.h>

class Visualizer : public QGraphicsView {
        Q_OBJECT

    private:
        struct Robot{
          QGraphicsPathItem *robot;
          QGraphicsSimpleTextItem *id;
          bool visible;
          void tryHide();
          void show();
        };
        struct Ball{
          QGraphicsEllipseItem *actual;
          QGraphicsEllipseItem *attentionCircle;
          QGraphicsSimpleTextItem *noBallWarning;
          void setPos(qreal x, qreal y);
          void hide();
          void show();
        };
        struct PlacementMarker{
          QGraphicsLineItem *line1;
          QGraphicsLineItem *line2;
          QGraphicsEllipseItem *goodRadius;
          void setPos(qreal x, qreal y);
          void hide();
          void show();
        };
    public:
        explicit Visualizer(QWidget *parent = nullptr);
        ~Visualizer() override;

    protected:
        void wheelEvent(QWheelEvent *event) override;
        void drawForeground(QPainter *painter, const QRectF &rect) override;
        void drawBackground(QPainter *painter, const QRectF &rect) override;
        void resizeEvent(QResizeEvent * event) override;

    public slots:
        void setShowDetections(bool showDetections);
        void setShowPlacementMarker(bool show);
    private slots:
        void updateAll();

    private:
        void updateWorld(const proto::World & world);
        void updateDetections(const std::vector<proto::SSL_WrapperPacket>& packets);
        void updateRobot(const proto::WorldRobot &robot, QMap<uint, Robot> &robots, const proto::RobotInfo &info,
                const QColor &color);
        void drawDetectionFrames(QPainter *painter, const std::vector<proto::SSL_DetectionFrame>& frames);
        void drawDetectionFrame(QPainter *painter, const proto::SSL_DetectionFrame &frame);
        void drawDetectionBall(QPainter * painter, const proto::SSL_DetectionBall& ball);
        void drawDetectionRobot(QPainter * painter, const proto::SSL_DetectionRobot& bot, const proto::RobotInfo &info, const QColor &color);
        std::vector<proto::SSL_DetectionFrame> usedDetectionFrames;

        QTimer * updateTimer;
        QGraphicsScene *scene;

        QMap<uint,Robot> blueBots;
        QMap<uint,Robot> yellowBots;
        Ball * ball;
        void createBall();

        PlacementMarker * placementMarker;
        void createPlacementMarker();

        QRectF fieldRect; //The window of the scene (e.g. what are the boundaries of what we visualize
        void refitView();

        void drawField(QPainter * painter);
        void drawGoal(QPainter * painter,bool isLeft);
        bool redrawField = true;
        bool weAreBlue = true; // in other parts of our code we are blue by default and the world is rotated as it is for us
        FieldState field;
        CameraMap cameras;

        bool showDetections = false;
        bool showPlacementMarker = true;
};

#endif //SOCCER_VISUALIZER_H
