#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qdatetime.h"
#include "QTime"
#include "qdebug.h"


#include <QtSql>
#include <QSqlQuery>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void doEraseData(QString date1,QString date2);
    void initDb();
    QString PATH;
    void getPlayers();
private slots:
    void eraseData();
    void exportData();

};

#endif // MAINWINDOW_H
