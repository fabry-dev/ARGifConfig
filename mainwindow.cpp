#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qmessagebox.h"
#include "qdebug.h"




#define PATH_DEFAULT "/home/fred/Dropbox/Taf/Cassiopee/realAugGif/files/"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->bErase,SIGNAL(clicked(bool)),this,SLOT(eraseData()));
    connect(ui->bExport,SIGNAL(clicked(bool)),this,SLOT(exportData()));

    QStringList params = QCoreApplication::arguments();

    if(params.size()>1)
        PATH = params[1];
    else
        PATH=PATH_DEFAULT;


    ui->eraseDateFrom->setDateTime(QDateTime::currentDateTime());
    ui->eraseDateTo->setDateTime(QDateTime::currentDateTime());
    ui->exportDateFrom->setDateTime(QDateTime::currentDateTime());
    ui->exportDateTo->setDateTime(QDateTime::currentDateTime());

    initDb();
 //   getPlayers();
}



void MainWindow::initDb()
{

    const QString DRIVER("QSQLITE");
    QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER);
    db.setHostName("hostname");
    db.setDatabaseName(PATH+"ardb");
    db.setUserName("user");
    db.setPassword("password");

    if(!db.open())
        qWarning() << "ERROR: " << db.lastError();

    qDebug()<<db.tables();


    getPlayers();

}






void MainWindow::eraseData()
{
    int ret = QMessageBox::critical(this,"Erase data","Are you sure you wan to erase this data?\nThis operation CANNOT be undone.",QMessageBox::Cancel,QMessageBox::Ok);

    switch (ret) {
    case QMessageBox::Ok:
        doEraseData(ui->eraseDateFrom->dateTime().toString("yyyy-MM-dd hh:mm:ss"),ui->eraseDateTo->dateTime().toString("yyyy-MM-dd hh:mm:ss"));
        break;
    case QMessageBox::Cancel:
        qDebug()<<"abort";
        break;
    default:
        // should never be reached
        break;
    }
}


void MainWindow::doEraseData(QString date1, QString date2)
{
    qDebug()<<"erase data between "<<date1<<"and"<<date2;
    QSqlQuery query;
    query.prepare("DELETE FROM players WHERE date between :DATE1 and :DATE2");
    query.bindValue(":DATE1", QVariant(date1));
    query.bindValue(":DATE2", QVariant(date2));
    query.exec();
   //getPlayers();

}











void MainWindow::getPlayers()
{
    QSqlQuery query;
    query.prepare("SELECT id,name,email,phone,gif,date FROM players");
    query.exec();

    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        QString email = query.value(2).toString();
        QString phone = query.value(3).toString();
        QString gif = query.value(4).toString();
        QDateTime date = query.value(5).toDateTime();
        qDebug()<<id<<name<<email<<phone<<gif<<date;
    }
}





void MainWindow::exportData()
{
    QString date1 = ui->exportDateFrom->dateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString date2 = ui->exportDateTo->dateTime().toString("yyyy-MM-dd hh:mm:ss");

    QSqlQuery query;





    qDebug()<<"Export data between: "<<date1<<"and"<<date2;


    query.prepare("SELECT id,name,email,phone,gif,date FROM players WHERE date between :DATE1 and :DATE2");
    query.bindValue(":DATE1", QVariant(date1));
    query.bindValue(":DATE2", QVariant(date2));
    query.exec();


    QString filename=PATH+"data.csv";
    QFile file( filename );
    if ( file.open(QIODevice::WriteOnly) )
    {
        QTextStream stream( &file );


        stream <<"user id"<<","<<"name"<<","<<"email"<<","<<"phone number"<<","<<"gif file"<<","<<"registration date"<<endl;
        while (query.next()) {
            int id = query.value(0).toInt();
            QString name = query.value(1).toString();
            QString email = query.value(2).toString();
            QString phone = query.value(3).toString();
            QString gif = query.value(4).toString();

            QDateTime date = query.value(5).toDateTime();

             qDebug()<<id<<name<<email<<phone<<gif<<date.toString("yyyy-MM-dd hh:mm:ss");
            stream <<id<<","<<name<<","<<email<<","<<phone<<","<<gif<<","<<date.toString("yyyy-MM-dd hh:mm:ss")<<endl;
        }
    }
}





MainWindow::~MainWindow()
{
    delete ui;
}
