//
// Created by rolf on 05-04-20.
//


#include "MainSettingsWidget.h"
#include "MainTeamSettingsWidget.h"
#include <QtWidgets/QLineEdit>
#include <protoUtils/Flip.h>
#include "ReplayWidget.h"

MainSettingsWidget::MainSettingsWidget(QWidget* parent) : QWidget(parent){

    mainLayout = new QVBoxLayout();
    settingsBox = new QGroupBox("Main settings");
    mainSettings = new QVBoxLayout();

    replayWidget = new ReplayWidget(this);
    //This ensures that when the replay is active relevant settings are greyed out and not set.
    connect(replayWidget, &ReplayWidget::replayActive, this, &MainSettingsWidget::setPlayReplay);
    mainLayout->addWidget(replayWidget);

    usageMode = new QComboBox();
    usageMode->addItems({"One team","Two teams","One team simulator","Two teams simulator"});
    usageMode->setStyleSheet("QComboBox{background-color: darkslategray;};");
    usageMode->setEnabled(true);
    connect(usageMode,SIGNAL(currentIndexChanged(int)), this, SLOT(updateMode(int)));
    mainSettings->addWidget(usageMode);


    checkBoxLayout = new QHBoxLayout();

    refereeCheckBox = new QCheckBox("Listen to referee");
    refereeCheckBox->setChecked(true);
    checkBoxLayout->addWidget(refereeCheckBox);

    connect(refereeCheckBox, &QCheckBox::clicked, this, &MainSettingsWidget::setListenToReferee);

    matlabLoggingCheckbox = new QCheckBox("Log vision filter (Matlab)");
    matlabLoggingCheckbox->setChecked(false);
    checkBoxLayout->addWidget(matlabLoggingCheckbox);
    connect(matlabLoggingCheckbox, &QCheckBox::clicked, this, &MainSettingsWidget::setMatlabLoggingOn);

    loggingCheckBox = new QCheckBox("Log data");
    loggingCheckBox->setChecked(false);
    checkBoxLayout->addWidget(loggingCheckBox);
    connect(loggingCheckBox, &QCheckBox::clicked, this, &MainSettingsWidget::setLoggingOn);


    mainSettings->addLayout(checkBoxLayout);

    networkLayout = new QHBoxLayout();
    networkText = new QLabel("grSim address and port");
    networkLayout->addWidget(networkText);
    IPText = new QLineEdit();
    IPText->setText("127.0.0.1");
    networkLayout->addWidget(IPText);

    portBox = new QSpinBox();
    portBox->setRange(0, 999999);
    portBox->setValue(10004);
    networkLayout->addWidget(portBox);
    mainSettings->addLayout(networkLayout);

    leftTeamWidget = new MainTeamSettingsWidget(true,this);
    rightTeamWidget = new MainTeamSettingsWidget(false,this);

    teamSettingsLayout = new QHBoxLayout();
    teamSettingsLayout->addWidget(leftTeamWidget);
    teamSettingsLayout->addWidget(rightTeamWidget);

    settingsBox->setLayout(mainSettings);
    mainLayout->addWidget(settingsBox);
    mainLayout->addLayout(teamSettingsLayout);
    setLayout(mainLayout);

    setDisabledColor(refereeCheckBox);
    setDisabledColor(loggingCheckBox);
    setDisabledColor(leftTeamWidget);
    setDisabledColor(rightTeamWidget);
    setDisabledColor(matlabLoggingCheckbox);

    leftTeamWidget->setEnabled(false);
    rightTeamWidget->setEnabled(false);
    rightTeamWidget->hide();

}
void MainSettingsWidget::setListenToReferee(bool listen) {
    listenToReferee = listen;
    leftTeamWidget->setEnabled(!listen);
    rightTeamWidget->setEnabled(!listen);
}
void MainSettingsWidget::setLoggingOn(bool listen) {
    loggingOn = listen;
}
MainSettingsWidget::~MainSettingsWidget() {

}
proto::Settings MainSettingsWidget::getSettings(){
    messageCounter ++; //we increment the counter by one everytime we send a command
    proto::Settings settings;
    settings.set_messagecounter(messageCounter);
    settings.set_mode((proto::Settings_usageMode)usageMode->currentIndex());//NOTE THE CAST!
    settings.set_listentoreferee(listenToReferee);
    settings.set_loggingon(loggingOn);
    settings.set_playingreplay(playReplay);
    settings.set_commandaddress(IPText->text().toStdString());
    settings.set_commandport(portBox->text().toUInt());
    settings.mutable_firstteam()->CopyFrom(leftTeamWidget->getTeamSettings());
    if(rightTeamWidget->isVisible()){
        settings.mutable_secondteam()->CopyFrom(rightTeamWidget->getTeamSettings());
    }
    if(saveBacklogNextTick){
        settings.set_savebacklog(true);
        saveBacklogNextTick = false;
    }
    settings.set_logvisionmatlab(matlabLoggingOn);
    return settings;
}
void MainSettingsWidget::updateMode(int index) {
    auto newMode = (proto::Settings_usageMode) index; //MAKE SURE THAT THE ORDER IS THE SAME
    showUsageMode(newMode);
    if(!playReplay){
        mode = newMode;
    }
}
void MainSettingsWidget::showUsageMode(const proto::Settings_usageMode &newMode) const {
    if(newMode == proto::Settings_usageMode_TWO_TEAMS || newMode == proto::Settings_usageMode_SIMULATION_TWO_TEAMS){
        rightTeamWidget->show();
    }else if (newMode == proto::Settings_usageMode_ONE_TEAM || newMode == proto::Settings_usageMode_SIMULATION_ONE_TEAM){
        rightTeamWidget->hide();
    }
}
void MainSettingsWidget::setDisabledColor(QWidget* widget) {
    widget->setStyleSheet(QString::fromUtf8("QWidget:disabled"
                                              "{ color: gray }"));
}
void MainSettingsWidget::updateFrame(const proto::FrameLog &frame) {
    if(listenToReferee && frame.has_gamestate()){
        leftTeamWidget->setFromGameState(frame.gamestate());
        proto::GameState flipped = frame.gamestate();
        flip(flipped);
        rightTeamWidget->setFromGameState(flipped);
    }
}
void MainSettingsWidget::setPlayReplay(bool play) {
    playReplay = play;
    if(playReplay){
        // we are in replay mode
        //Listen to old settings for information
        //We disable all the options the user can normally change
        refereeCheckBox->setEnabled(false);
        loggingCheckBox->setEnabled(false);
        //We explicitly set logging to false under replay as we don't want to record data of replays (this can mess up some parts)
        loggingCheckBox->setChecked(false);
        loggingOn = false;
        leftTeamWidget->setEnabled(false);
        leftTeamWidget->setReplay(true);
        rightTeamWidget->setEnabled(false);
        rightTeamWidget->setReplay(true);
        usageMode->setEnabled(false);
        usageMode->setStyleSheet("QComboBox{background-color: darkslategray; color:gray};");

    } else{
        refereeCheckBox->setEnabled(true);
        refereeCheckBox->setChecked(listenToReferee);
        loggingCheckBox->setEnabled(true);
        leftTeamWidget->setEnabled(!listenToReferee);
        leftTeamWidget->setReplay(false);
        rightTeamWidget->setEnabled(!listenToReferee);
        rightTeamWidget->setReplay(false);
        usageMode->setEnabled(true);
        usageMode->setCurrentIndex(mode);
        usageMode->setStyleSheet("QComboBox{background-color: darkslategray;};");
    }
}
ReplayWidget* MainSettingsWidget::getReplayWidget() {
    return replayWidget;
}
void MainSettingsWidget::visualizeFrame(const proto::FrameLog &frame){
    refereeCheckBox->setChecked(frame.replaysettings().listentoreferee());
    usageMode->setCurrentIndex(frame.replaysettings().mode());
    leftTeamWidget->visualizeFromGameState(frame.gamestate());
    proto::GameState flipped = frame.gamestate();
    flip(flipped);
    rightTeamWidget->visualizeFromGameState(flipped);//TODO: fix
}
void MainSettingsWidget::saveBacklog() {
    saveBacklogNextTick = true;
}

void MainSettingsWidget::setMatlabLoggingOn(bool logging) {
    matlabLoggingOn = logging;
}
