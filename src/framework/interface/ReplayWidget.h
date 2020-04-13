//
// Created by rolf on 12-04-20.
//

#ifndef SOCCER_REPLAYWIDGET_H
#define SOCCER_REPLAYWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QSlider>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <logger/LogReader.h>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGroupBox>
#include <core/Time.h>
#include <QTimer>

class ReplayWidget : public QWidget {
        Q_OBJECT

    public:
        ReplayWidget(QWidget * parent = nullptr);
        ~ReplayWidget() override;

    signals:
        void gotLogFrame(const proto::FrameLog &logFrame);
    public slots:
        void openFile();

    private slots:
        void togglePlay();
        void setSpeed(int index);
        void seekFrame();
        void stepForward();
        void stepBackward();
        void updateInformation();


    private:
        void openFile(const QString &filePath);
        static QString stringFromTime(const Time& time);
        void updateTimerInfo(long long int time);
        QHBoxLayout *totalLayout;
        QGroupBox *groupBox;
        QVBoxLayout * mainLayout;

        QLabel * fileName;
        QVBoxLayout * beforeInfo;
        QLabel * currentTime;
        QLabel * currentPacket;
        QVBoxLayout * afterInfo;
        QLabel * totalTime;
        QLabel * totalPacket;

        long firstTime = 0;
        long lastTime = 0;

        QToolButton * playButton;
        QToolButton * stepBackwardButton;
        QToolButton * stepForwardButton;
        QToolBar * toolBar;

        bool isPlaying = false;
        bool fileIsOpen = false;

        QComboBox * playSpeed;
        double speedFactor = 1.0;
        long long int currentReplayTime = 0;
        int currentPacketNumber = 0;
        const int STEP_MS = 5;

        QSlider * timeSlider;
        QHBoxLayout *sliderLayout;
        QTimer * playTimer;

        LogReader logReader;
};

#endif //SOCCER_REPLAYWIDGET_H
