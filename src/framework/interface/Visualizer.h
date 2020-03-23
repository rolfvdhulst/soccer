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
    private:
        void updateAll();
        void updateWorld(const proto::World & world);
        void updateRobot(const proto::WorldRobot &robot, QMap<uint, Robot> &robots, const proto::RobotInfo &info,
                const QColor &color);
        QTimer * updateTimer;
        QGraphicsScene *scene;
        QMap<uint,Robot> blueBots;
        QMap<uint,Robot> yellowBots;


        QRectF fieldRect; //The window of the scene (e.g. what are the boundaries of what we visualize
        void refitView();


};

#endif //SOCCER_VISUALIZER_H
