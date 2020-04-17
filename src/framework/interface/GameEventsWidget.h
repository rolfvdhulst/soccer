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
        void addNewEvents(const RefereeState &state);

    public slots:
        void setReplay(bool replay);

    private:
        QString savedText;

};

#endif //SOCCER_GAMEEVENTSWIDGET_H
