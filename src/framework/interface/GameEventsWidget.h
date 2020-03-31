//
// Created by rolf on 30-03-20.
//

#ifndef SOCCER_GAMEEVENTSWIDGET_H
#define SOCCER_GAMEEVENTSWIDGET_H

#include <QtWidgets/QPlainTextEdit>
#include <referee/GameEvent.h>
#include <referee/GameState.h>

class GameEventsWidget : public QPlainTextEdit{
    public:
        explicit GameEventsWidget(QWidget * parent);
        void addNewEvents(std::vector<proto::GameEvent> events,const GameState &state);
};

#endif //SOCCER_GAMEEVENTSWIDGET_H
