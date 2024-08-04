#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "servermanager.h"
#include <QString>
#include "clientchat.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void getName(QString);
    void changeName(QString);
    void setClientStatus(chatprotocol::Status);
private:
    Ui::MainWindow *ui;
    ServerManager *m_server;
    chatprotocol m_protocol;
    QString name;
    void setupServer();

private slots:
    void newClientConnected(QTcpSocket*);
    void clientDisconnected(QTcpSocket*);
};
#endif // MAINWINDOW_H
