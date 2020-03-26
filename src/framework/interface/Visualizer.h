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
    public:
        explicit Visualizer(QWidget *parent = nullptr);
        ~Visualizer() override;
    protected:
        void wheelEvent(QWheelEvent *event) override;
        void drawForeground(QPainter *painter, const QRectF &rect) override;
        void drawBackground(QPainter *painter, const QRectF &rect) override;
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
        QGraphicsEllipseItem * ball;
        void createBall();


        QRectF fieldRect; //The window of the scene (e.g. what are the boundaries of what we visualize
        void refitView();

        void drawField(QPainter * painter);
        bool redrawField = true;
        FieldState field;
        CameraMap cameras;

};

#endif //SOCCER_VISUALIZER_H
