#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QTimer>
#include <QTabWidget>
#include <QMainWindow>
#include<QFile>
#include<QFileDialog>
#include<QTextStream>
#include<QMessageBox>
#include "secondary.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void update();
    void start();
    void on_actionopen_triggered();

    void on_actionnew_triggered();

    void on_actionsave_as_triggered();

    void on_actionsave_triggered();

    void on_actionsave_2_triggered();

    void on_actioncompare_with_triggered();
    
private:
    Ui::MainWindow *ui;
    secondary * sec;
};
#endif // MAINWINDOW_H
