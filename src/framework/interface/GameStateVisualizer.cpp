//
// Created by rolf on 30-03-20.
//

#include <QTimer>
#include "GameStateVisualizer.h"
#include <interfaceAPI/API.h>
#include <algorithm>
#include "GameEventsWidget.h"

GameStateVisualizer::GameStateVisualizer(QWidget* parent) :
    QWidget(parent){

    setMinimumSize(192,108);
    setMaximumSize(384,720);
    totalLayout = new QVBoxLayout(this);
    teamInfoLayout = new QGridLayout();
    sharedInfoLayout = new QVBoxLayout();
    gameEventsWidget = new GameEventsWidget(this);

    totalLayout->addLayout(sharedInfoLayout);
    totalLayout->addLayout(teamInfoLayout);
    totalLayout->addWidget(gameEventsWidget);

    command = new QLabel(this);
    command->setText("Command: ");
    sharedInfoLayout->addWidget(command);

    actionTime = new QLabel(this);
    actionTime->setText("Action time left: -");
    sharedInfoLayout->addWidget(actionTime);

    gameStage = new QLabel(this);
    gameStage->setText("Stage: \nTime left: -");
    sharedInfoLayout->addWidget(gameStage);

    nextCommand = new QLabel(this);
    nextCommand->setText("Next command: -");
    sharedInfoLayout->addWidget(nextCommand);

    const int text = 0;
    const int left = 1;
    const int right = 2;
    int row = 0;

    teamNameText = new QLabel(this);
    teamNameText->setText("Team");
    teamInfoLayout->addWidget(teamNameText,row,text);
    leftTeamName = new QLabel(this);
    teamInfoLayout->addWidget(leftTeamName,row,left);
    rightTeamName = new QLabel(this);
    teamInfoLayout->addWidget(rightTeamName,row,right);
    row++;

    scoreText = new QLabel(this);
    scoreText->setText("Score");
    teamInfoLayout->addWidget(scoreText,row,text);
    leftScore = new QLabel(this);
    teamInfoLayout->addWidget(leftScore,row,left);
    rightScore = new QLabel(this);
    teamInfoLayout->addWidget(rightScore,row,right);
    row++;

    yellowCardText = new QLabel(this);
    yellowCardText->setText("Yellow cards \nTime remaining");
    teamInfoLayout->addWidget(yellowCardText,row,text);
    leftYellowCards = new QLabel(this);
    teamInfoLayout->addWidget(leftYellowCards,row,left);
    rightYellowCards = new QLabel(this);
    teamInfoLayout->addWidget(rightYellowCards,row,right);
    row++;

    foulText = new QLabel(this);
    foulText->setText("Fouls made");
    teamInfoLayout->addWidget(foulText,row,text);
    leftFouls = new QLabel(this);
    teamInfoLayout->addWidget(leftFouls,row,left);
    rightFouls = new QLabel(this);
    teamInfoLayout->addWidget(rightFouls,row,right);
    row++;

    redCardText = new QLabel(this);
    redCardText->setText("Red cards");
    teamInfoLayout->addWidget(redCardText,row,text);
    leftRedCards = new QLabel(this);
    teamInfoLayout->addWidget(leftRedCards,row,left);
    rightRedCards = new QLabel(this);
    teamInfoLayout->addWidget(rightRedCards,row,right);
    row++;

    timeOutText = new QLabel(this);
    timeOutText->setText("Timeouts left ");
    teamInfoLayout->addWidget(timeOutText,row,text);
    leftTimeouts = new QLabel(this);
    teamInfoLayout->addWidget(leftTimeouts,row,left);
    rightTimeouts = new QLabel(this);
    teamInfoLayout->addWidget(rightTimeouts,row,right);
    row++;

    goalieText = new QLabel(this);
    goalieText->setText("Goalkeeper id ");
    teamInfoLayout->addWidget(goalieText,row,text);
    leftGoalie = new QLabel(this);
    teamInfoLayout->addWidget(leftGoalie,row,left);
    rightGoalie = new QLabel(this);
    teamInfoLayout->addWidget(rightGoalie,row,right);
    row++;




}
GameStateVisualizer::~GameStateVisualizer() {
    delete leftScore;
    delete rightScore;
    //TODO: delete everything properly
    delete gameEventsWidget;
}
void GameStateVisualizer::updateFrame(const proto::FrameLog& frame) {
    if(frame.has_gamestate()){
        const GameState state(frame.gamestate());
        if(state.refState){
            updateGamestate(*state.refState,state.ourColor);
            gameEventsWidget->addNewEvents(*state.refState);
        }
    }
}
void GameStateVisualizer::updateGamestate(const RefereeState &state, Team ourColor) {

    QString gamestageText= "Stage: "+QString::fromStdString(state.stage.toString()) +"\nTime left: ";
    state.stageTimeLeft ? gamestageText+=QString::number(state.stageTimeLeft->asIntegerMilliSeconds()/1000.0) +" s" : gamestageText+="-";
    gameStage->setText(gamestageText);
    if(state.currentActionTimeRemaining){
        actionTime->setText("Action time left: "+QString::number(state.currentActionTimeRemaining->asIntegerMilliSeconds()/1000.0)+ " s");
    }else{
        actionTime->setText("Action time left: -");
    }
    command->setText("Command: "+QString::fromStdString(state.command.toString()));
    if(state.nextCommand){
        nextCommand->setText("Next command: " +QString::fromStdString(state.command.toString()));
    }else{
        nextCommand->setText("Next command: -");
    }
    //We already rotate the field so we are left, so this is easier to view.
    displayLeftTeam(state.usInfo, ourColor);
    displayRightTeam(state.themInfo, ourColor.inverse());
}
void GameStateVisualizer::displayLeftTeam(const TeamInfo &teamInfo, Team color) {
    leftTeamName->setText(QString::fromStdString(teamInfo.name));
    QString colorStr = "gray";
    if (color != Team::UNKNOWN){
        colorStr = (color == Team::BLUE ? "cyan" : "orange");
    }
    leftTeamName->setStyleSheet("QLabel { color :"+ colorStr +"; }");
    leftScore->setText(QString::number(teamInfo.score));
    QString timeOutString = QString::number(teamInfo.timeOuts)+ ", "+QString::number(teamInfo.timeOutTime.asIntegerSeconds()) + " s";
    leftTimeouts->setText(timeOutString);
    leftFouls->setText(QString::number(teamInfo.foulCounter));
    leftRedCards->setText(QString::number(teamInfo.redCards));

    QString yellowCards= QString::number(teamInfo.totalYellowCards) +", "+QString::number(teamInfo.activeYellowCards())+" active \n";
    QString remainingTimes;
    int visualizeCount = std::min((unsigned int)3,teamInfo.activeYellowCards());
    for (int i = 0; i < visualizeCount ; ++ i) {
        remainingTimes+= QString::number(teamInfo.yellowCardTimes[i].asIntegerSeconds())+" s";
        if(i != visualizeCount-1){
            remainingTimes+=", ";
        }
    }
    leftYellowCards->setText(yellowCards+remainingTimes);
    leftGoalie->setText(QString::fromStdString(teamInfo.goalkeeperID.toString()));
}
void GameStateVisualizer::displayRightTeam(const TeamInfo &teamInfo, Team color) {
    rightTeamName->setText(QString::fromStdString(teamInfo.name));
    QString colorStr = "gray";
    if (color != Team::UNKNOWN){
        colorStr = (color == Team::BLUE ? "cyan" : "orange");
    }
    rightTeamName->setStyleSheet("QLabel { color :"+ colorStr +"; }");
    rightScore->setText(QString::number(teamInfo.score));
    QString timeOutString = QString::number(teamInfo.timeOuts)+ ", "+QString::number(teamInfo.timeOutTime.asIntegerSeconds()) + " s";
    rightTimeouts->setText(timeOutString);
    rightFouls->setText(QString::number(teamInfo.foulCounter));
    rightRedCards->setText(QString::number(teamInfo.redCards));

    QString yellowCards= QString::number(teamInfo.totalYellowCards) +", "+QString::number(teamInfo.activeYellowCards())+" active \n";
    QString remainingTimes;
    int visualizeCount = std::min((unsigned int)3,teamInfo.activeYellowCards());
    for (int i = 0; i < visualizeCount ; ++ i) {
        remainingTimes+= QString::number(teamInfo.yellowCardTimes[i].asIntegerSeconds())+" s";
        if(i != visualizeCount-1){
            remainingTimes+=", ";
        }
    }
    rightYellowCards->setText(yellowCards+remainingTimes);
    rightGoalie->setText(QString::fromStdString(teamInfo.goalkeeperID.toString()));
    //TODO: remaining fields. Are not that relevant for visualization but might be nice.
}
GameEventsWidget* GameStateVisualizer::getGameEventsWidget() {
    return gameEventsWidget;
}
