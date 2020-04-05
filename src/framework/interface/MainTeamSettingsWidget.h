//
// Created by rolf on 05-04-20.
//

#ifndef SOCCER_MAINTEAMSETTINGSWIDGET_H
#define SOCCER_MAINTEAMSETTINGSWIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGroupBox>
#include <protobuf/Settings.pb.h>

class MainTeamSettingsWidget : public QWidget {
    Q_OBJECT

    public:
        explicit MainTeamSettingsWidget(bool isLeft, QWidget * parent);
        ~MainTeamSettingsWidget() override;
        [[nodiscard]] proto::TeamSettings getTeamSettings() const;
    private:
        QGroupBox * groupBox;
        QVBoxLayout * boxLayout;
        QVBoxLayout * mainLayout;
        QHBoxLayout * teamLayout;
        QHBoxLayout * networkLayout;

        QPushButton * colorButton;
        void toggleOurColor();
        bool weAreBlue = true;

        QPushButton * sideButton;
        void toggleOurSide();
        bool wePlayOnPositiveHalf = false;

        QComboBox * keeperIDBox;
        void setKeeperID(const QString &id);
        unsigned int keeperID = 0;
};

#endif //SOCCER_MAINTEAMSETTINGSWIDGET_H
