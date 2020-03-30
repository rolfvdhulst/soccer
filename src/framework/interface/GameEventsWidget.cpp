//
// Created by rolf on 30-03-20.
//

#include <QtCore/QDateTime>
#include "GameEventsWidget.h"
GameEventsWidget::GameEventsWidget(QWidget* parent)
        :QPlainTextEdit(parent) {
    setReadOnly(true);
    setMaximumBlockCount(1000);
    setLineWrapMode(LineWrapMode::WidgetWidth);
}
void GameEventsWidget::addNewEvents(std::vector<proto::GameEvent> events,const GameState& state) {
    for(const auto& protoEvent : events){
        GameEvent event(protoEvent);
        long msecs = state.timeStamp.asIntegerMilliSeconds();
        QDateTime dateTime =QDateTime::fromMSecsSinceEpoch(msecs);

        double seconds =(msecs % 60000)/1000.0;
        QString zero = seconds <10 ? "0" :"";
        QString toPrint="["+dateTime.toString(Qt::DateFormat::DefaultLocaleShortDate)+":"+zero+QString::number(seconds)+
                "] "+QString(QString::fromStdString(event.toString()));

        appendPlainText(toPrint);
    }
}
