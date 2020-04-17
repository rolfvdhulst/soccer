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
    if(!isInReplay){
        keeperID = id.toUInt();
    }
}
MainTeamSettingsWidget::~MainTeamSettingsWidget() {

}
void MainTeamSettingsWidget::toggleOurColor() {
    weAreBlue = !weAreBlue; //invert color
    setColorVisual(weAreBlue);
}
void MainTeamSettingsWidget::toggleOurSide() {
    wePlayOnPositiveHalf = !wePlayOnPositiveHalf; //invert side
    setSideVisual(wePlayOnPositiveHalf);
}
proto::TeamSettings MainTeamSettingsWidget::getTeamSettings() const {
    proto::TeamSettings settings;
    settings.set_weareblue(weAreBlue);
    settings.set_weplayonpositivehalf(wePlayOnPositiveHalf);
    settings.set_keeperid(keeperID);
    return settings;
}
void MainTeamSettingsWidget::setReplay(bool inReplay) {
    isInReplay = inReplay;
    if(inReplay){
        //TODO: fix visuals and updates for replay mode here and in MainSettingsWidget
//        setSideVisual(replaySettings.weplayonpositivehalf());
//        setColorVisual(replaySettings.weareblue());
//        keeperIDBox->setCurrentIndex(replaySettings.defaultkeeperid());
    } else{
        //Restore initial values after replay
        setSideVisual(wePlayOnPositiveHalf);
        setColorVisual(weAreBlue);
        keeperIDBox->setCurrentIndex(keeperID);
    }
}
void MainTeamSettingsWidget::setSideVisual(bool positiveHalf) {
    if(positiveHalf){
        sideButton->setText("Playing as right ▶");
    } else{
        sideButton->setText("◀ Playing as left");
    }
}
void MainTeamSettingsWidget::setColorVisual(bool isBlue) {
    if(isBlue){
        colorButton->setStyleSheet("background-color: blue;");
        colorButton->setText("Playing as Blue");
    } else{
        colorButton->setStyleSheet("background-color: orange;");  // orange is more readable
        colorButton->setText("Playing as Yellow");
    }
}
void MainTeamSettingsWidget::setFromGameState(const proto::GameState &gameState) {
    bool gameStateWeAreBlue = gameState.settings().weareblue();// since Team can be Unknown we check this.
    // This is to prevent empty messages from changing the color
    if (gameStateWeAreBlue != weAreBlue){
        toggleOurColor();
    }
    if(gameState.settings().weplayonpositivehalf() != wePlayOnPositiveHalf){
        toggleOurSide();
    }
    if(gameState.settings().keeperid() != keeperIDBox->currentIndex()){
        keeperID = gameState.settings().keeperid();
        keeperIDBox->setCurrentIndex(gameState.settings().keeperid());
    }
}
//Crucial difference, visualizes but doesn't update default values so when you go back the widget remembers the old values
void MainTeamSettingsWidget::visualizeFromGameState(const proto::GameState &gameState){
    bool gameStateWeAreBlue = gameState.settings().weareblue();
    setColorVisual(gameStateWeAreBlue);
    setSideVisual(gameState.settings().weplayonpositivehalf());
    keeperIDBox->setCurrentIndex(gameState.settings().keeperid());
}
