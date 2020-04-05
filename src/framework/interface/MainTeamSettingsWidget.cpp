//
// Created by rolf on 05-04-20.
//

#include "MainTeamSettingsWidget.h"

MainTeamSettingsWidget::MainTeamSettingsWidget(bool isLeft, QWidget* parent) :QWidget(parent) {
    mainLayout = new QVBoxLayout();
    teamLayout = new QHBoxLayout();
    networkLayout = new QHBoxLayout();
    groupBox = new QGroupBox();
    isLeft ? groupBox->setTitle("First Team") : groupBox->setTitle("Second Team");
    boxLayout = new QVBoxLayout();

    colorButton = new QPushButton("Color");
    connect(colorButton,&QPushButton::clicked,this,&MainTeamSettingsWidget::toggleOurColor);
    teamLayout->addWidget(colorButton);
    toggleOurColor();
    if(isLeft){
        toggleOurColor();
    }

    keeperIDBox = new QComboBox();
    for (int i = 0; i < 16; ++ i) {
        keeperIDBox->addItem(QString::number(i));
    }
    keeperIDBox->setMaximumWidth(50);
    connect(keeperIDBox,&QComboBox::currentTextChanged,this,&MainTeamSettingsWidget::setKeeperID);
    teamLayout->addWidget(keeperIDBox);

    boxLayout->addLayout(teamLayout);

    sideButton = new QPushButton("Side");
    connect(sideButton,&QPushButton::clicked,this,&MainTeamSettingsWidget::toggleOurSide);
    boxLayout->addWidget(sideButton);
    toggleOurSide();
    if(isLeft){
        toggleOurSide();
    }
    groupBox->setLayout(boxLayout);
    mainLayout->addWidget(groupBox);
    setLayout(mainLayout);
}
void MainTeamSettingsWidget::setKeeperID(const QString& id) {
    keeperID = id.toUInt();
}
MainTeamSettingsWidget::~MainTeamSettingsWidget() {

}
void MainTeamSettingsWidget::toggleOurColor() {
    weAreBlue = !weAreBlue; //invert color
    if(weAreBlue){
        colorButton->setStyleSheet("background-color: blue;");
        colorButton->setText("Playing as Blue");
    } else{
        colorButton->setStyleSheet("background-color: orange;");  // orange is more readable
        colorButton->setText("Playing as Yellow");
    }
}
void MainTeamSettingsWidget::toggleOurSide() {
    wePlayOnPositiveHalf = !wePlayOnPositiveHalf; //invert side
    if(wePlayOnPositiveHalf){
        sideButton->setText("Playing as right ▶");
    } else{
        sideButton->setText("◀ Playing as left");
    }
}
proto::TeamSettings MainTeamSettingsWidget::getTeamSettings() const {
    proto::TeamSettings settings;
    settings.set_weareblue(weAreBlue);
    settings.set_weplayonpositivehalf(wePlayOnPositiveHalf);
    settings.set_defaultkeeperid(keeperID);
    return settings;
}
