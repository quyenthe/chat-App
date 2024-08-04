#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWidget>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupServer();

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::setupServer(){
    m_server=new ServerManager();
    connect(m_server,&ServerManager::newClientConnected,this,&MainWindow::newClientConnected);
    connect(m_server,&ServerManager::clientDisconnected,this,&MainWindow::clientDisconnected);


}
void MainWindow::newClientConnected(QTcpSocket * client){
    auto id=client->property("id").toInt();
    ui->listWidget->addItem(QString("new client: %1").arg(id));
    auto chat=new clientChat(client);

    ui->tabWidget->addTab(chat,QString("client %1").arg(id));
    connect(chat,&clientChat::name,this,&MainWindow::getName);
    connect(chat,&clientChat::status,this,&MainWindow::setClientStatus);
    connect(chat,&clientChat::textMessageReceive,m_server,&ServerManager::onTextForOtherClients);
}
void MainWindow::clientDisconnected(QTcpSocket *client){
    auto id=client->property("id").toInt();
    ui->listWidget->addItem(QString("client disconnected id : %1").arg(id));

    int currentIndex=ui->tabWidget->currentIndex();
    ui->tabWidget->removeTab(currentIndex);

}
void MainWindow::getName(QString name){
    auto widget = qobject_cast<QWidget *>(sender());
    auto index = ui->tabWidget->indexOf(widget);
    ui->tabWidget->setTabText(index,name);


}
void MainWindow::setClientStatus(chatprotocol::Status status){
    auto widget=qobject_cast<QWidget*>(sender());
    auto index=ui->tabWidget->indexOf(widget);
    QString iconName;
    switch(status){
    case chatprotocol::Available:
        iconName=":/icons/available.png";
        break;
    case chatprotocol::Busy:
        iconName=":/icons/busy.png";
        break;
    }
    auto icon = QIcon(iconName);
    ui->tabWidget->setTabIcon(index, icon);
}
void MainWindow::changeName(QString name){

}
