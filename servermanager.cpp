#include "servermanager.h"
#include "chatprotocol.h"
ServerManager::ServerManager(int port,QObject *parent)
    : QObject{parent}
{
    setupServer(port);
}
void ServerManager::setupServer(int port){
    m_server=new QTcpServer();
    connect(m_server,&QTcpServer::newConnection,this,&ServerManager::newClientConnectionReceived);
    m_server->listen(QHostAddress::Any,port);

}
void ServerManager::newClientConnectionReceived(){
    auto client=m_server->nextPendingConnection();
    m_client<<client;
    auto id=m_client.length();
    client->setProperty("id",id);
    emit newClientConnected(client);
    connect(client,&QTcpSocket::disconnected,this,&ServerManager::onClientDisConnected);
    auto clientName=QString("client %1").arg(id);
    m_listclientName.push_back(clientName);
    m_listclient.insert(clientName,client);

    for(int i=0;i<m_client.size();i++){
        m_listclientName.remove(i,1);
        m_client[i]->write(chatprotocol::getInstance()->setClientName(m_listclientName));
        m_listclientName.insert(i,QString("client %1").arg(i+1));
    }



}
void ServerManager::onTextForOtherClients(QString message, QString receiver){
    auto msg=chatprotocol::getInstance()->getData(chatprotocol::Message,message);
    m_listclient[receiver]->write(msg);

}

void ServerManager::onClientDisConnected(){
    auto client=(QTcpSocket *)sender();
    QString clientName;
    for(int i=0;i<m_client.size();i++){
        if(m_client[i]==client){
            clientName=QString("client %1").arg(i+1);
            break;
        }
    }
    m_listclientName.removeOne(clientName);
    m_listclient.remove(clientName);
    for(int i=0;i<m_client.size();i++){
        m_client[i]->write(chatprotocol::getInstance()->setClientName(m_listclientName));
    }
    m_client.removeOne(client);
    emit clientDisconnected(client);

}

