//
// Created by rolf on 22-03-20.
//

#ifndef SOCCER_MAINWINDOW_H
#define SOCCER_MAINWINDOW_H
#include <QMainWindow>
#include <QHBoxLayout>

class Visualizer;
class MainWindow : public QMainWindow{
        Q_OBJECT
    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow() override;
    private:
        QWidget *mainWidget;
        QHBoxLayout *horizontalLayout;
        QVBoxLayout *mainLayout;
        Visualizer *visualizer;
};

#endif //SOCCER_MAINWINDOW_H
