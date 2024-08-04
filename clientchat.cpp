#include "clientchat.h"
#include "ui_clientchat.h"
clientChat::clientChat(QTcpSocket *client,QWidget *parent) :
    QWidget(parent),
    m_client(client),
    ui(new Ui::clientChat)

{
    ui->setupUi(this);
    connect(m_client,&QTcpSocket::readyRead,this,&clientChat::dataReceive);
    connect(m_client,QTcpSocket::disconnected,this,&clientChat::clientDisconnected);
}

clientChat::~clientChat()
{
    delete ui;
}
void clientChat::dataReceive()
{
    auto data=m_client->readAll();
    chatprotocol::getInstance()->loadData(data);

    ui->listWidget->addItem(chatprotocol::getInstance()->getMessage());
    switch(chatprotocol::getInstance()->type()){
    case chatprotocol::Text:
        emit name(chatprotocol::getInstance()->getName());
        break;
    case chatprotocol::SetStatus:
        emit status(chatprotocol::getInstance()->getclientStatus());
        break;
    case chatprotocol::ClientACK:
        clientName=chatprotocol::getInstance()->getClientACK();


        break;
    case chatprotocol::Message:
        emit textMessageReceive(chatprotocol::getInstance()->getMessage(),clientName);


        break;

    }



}

void clientChat::on_pushButton_clicked()
{
    m_client->write(chatprotocol::getInstance()->setMessage(ui->lineEdit->text()));
    ui->lineEdit->setText("");
}
void clientChat::clientDisconnected(){
}

