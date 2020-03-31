//
// Created by rolf on 22-03-20.
//

#ifndef SOCCER_MAINWINDOW_H
#define SOCCER_MAINWINDOW_H
#include <QMainWindow>
#include <QHBoxLayout>

class Visualizer;
class GameStateVisualizer;
class MainWindow : public QMainWindow{
        Q_OBJECT
    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow() override;
    private:
        static void configureCheckableMenuItem(const QString& title, QMenu *menu,
                const QObject *receiver, const char *method, bool defaultState);
        QWidget *mainWidget;
        QHBoxLayout *horizontalLayout;
        QVBoxLayout *mainLayout;
        QVBoxLayout *sideBarLayout;
        QWidget * sideBarWidget;
        Visualizer *visualizer;
        GameStateVisualizer *gameStateVisualizer;
};

#endif //SOCCER_MAINWINDOW_H
