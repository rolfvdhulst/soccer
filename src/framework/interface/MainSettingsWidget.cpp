//
// Created by rolf on 05-04-20.
//


#include "MainSettingsWidget.h"
#include "MainTeamSettingsWidget.h"
#include <QtWidgets/QLineEdit>

MainSettingsWidget::MainSettingsWidget(QWidget* parent) : QWidget(parent){

    mainLayout = new QVBoxLayout();
    settingsBox = new QGroupBox("Main settings");
    mainSettings = new QVBoxLayout();

    usageMode = new QComboBox();
    usageMode->addItems({"One team","Two teams","One team simulator","Two teams simulator","Replay"});
    usageMode->setStyleSheet("background-color: darkslategray;");
    connect(usageMode,SIGNAL(currentIndexChanged(int)), this, SLOT(updateMode(int)));
    mainSettings->addWidget(usageMode);


    checkBoxLayout = new QHBoxLayout();

    refereeCheckBox = new QCheckBox("Listen to referee");
    refereeCheckBox->setChecked(true);
    checkBoxLayout->addWidget(refereeCheckBox);

    connect(refereeCheckBox, &QCheckBox::clicked, this, &MainSettingsWidget::setListenToReferee);

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
    rightTeamWidget->hide();

}
void MainSettingsWidget::setListenToReferee(bool listen) {
    listenToReferee = listen;
}
void MainSettingsWidget::setLoggingOn(bool listen) {
    loggingOn = listen;
}
MainSettingsWidget::~MainSettingsWidget() {

}
proto::Settings MainSettingsWidget::getSettings() const {
    proto::Settings settings;

    settings.set_listentoreferee(listenToReferee);
    settings.set_loggingon(loggingOn);
    settings.set_commandaddress(IPText->text().toStdString());
    settings.set_commandport(portBox->text().toUInt());
    settings.mutable_firstteam()->CopyFrom(leftTeamWidget->getTeamSettings());
    if(rightTeamWidget->isVisible()){
        settings.mutable_secondteam()->CopyFrom(rightTeamWidget->getTeamSettings());
    }
    return settings;
}
void MainSettingsWidget::updateMode(int index) {
    auto newMode = (proto::Settings_usageMode) index; //MAKE SURE THAT THE ORDER IS THE SAME
    if(newMode == proto::Settings_usageMode_TWO_TEAMS || newMode == proto::Settings_usageMode_SIMULATION_TWO_TEAMS){
        rightTeamWidget->show();
    }else if (newMode == proto::Settings_usageMode_ONE_TEAM || newMode == proto::Settings_usageMode_SIMULATION_ONE_TEAM){
        rightTeamWidget->hide();
    }
    if(newMode == proto::Settings_usageMode_REPLAY) {
        // we are in replay mode
        //Listen to old settings for information
        //We disable all the options the user can normally change
        refereeCheckBox->setDisabled(true);
        loggingCheckBox->setDisabled(true);
        loggingCheckBox->setChecked(false);
        leftTeamWidget->setDisabled(true);
        rightTeamWidget->setDisabled(true);
    }
    if(mode == proto::Settings_usageMode_REPLAY && newMode != proto::Settings_usageMode_REPLAY){
        refereeCheckBox->setEnabled(true);
        loggingCheckBox->setChecked(loggingOn); //restore old state
        loggingCheckBox->setEnabled(true);
        leftTeamWidget->setEnabled(true);
        rightTeamWidget->setEnabled(true);
    }
    mode = newMode;
}
void MainSettingsWidget::setDisabledColor(QWidget* widget) {
    widget->setStyleSheet(QString::fromUtf8("QWidget:disabled"
                                              "{ color: gray }"));
}
