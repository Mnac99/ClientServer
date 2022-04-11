#include "serverrr.h"
#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QObject>



Serverrr::Serverrr(QWidget *parent)
    : QDialog(parent)
    ,statusLabel(new QLabel)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint );
    statusLabel-> setTextInteractionFlags(Qt::TextBrowserInteraction);
    initServer();
    fortunes << filereader();
    auto quitButton = new QPushButton(tr("Quit"));
    quitButton-> setAutoDefault(false);
    connect(quitButton,&QAbstractButton::clicked,this,&QWidget::close);
    connect(tcpServer,&QTcpServer::newConnection,this,&Serverrr::send);

    auto buttonLayout = new QHBoxLayout;
    buttonLayout-> addStretch(1);
    buttonLayout-> addWidget(quitButton);
    buttonLayout-> addStretch(1);
    QVBoxLayout * mainLayout = nullptr;
    if (QGuiApplication::styleHints()->showIsFullScreen() || QGuiApplication::styleHints()->showIsMaximized()) {
        auto outerVerticalLayout = new QVBoxLayout(this);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
        auto outerHorizontalLayout = new QHBoxLayout;
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        auto groupBox = new QGroupBox(QGuiApplication::applicationDisplayName());
        mainLayout = new QVBoxLayout(groupBox);
        outerHorizontalLayout->addWidget(groupBox);
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        outerVerticalLayout->addLayout(outerHorizontalLayout);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
    } else {
        mainLayout = new QVBoxLayout(this);
    }

    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(buttonLayout);

    setWindowTitle(QGuiApplication::applicationDisplayName());
}

void Serverrr::initServer()
{
    tcpServer = new QTcpServer(this);
    if(!tcpServer->listen())
    {
        QMessageBox::critical (this,tr("Server"),tr("unable to start the server: %1.").arg(tcpServer->errorString()));
        close();
        return;

    }
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i <ipAddressesList.size();++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address())
        {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    statusLabel->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
                            "Run Client now.")
                         .arg(ipAddress).arg(tcpServer->serverPort()));


}

void Serverrr::send()
{

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);
    out << fortunes[QRandomGenerator::global()->bounded(fortunes.size())];
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected,
            clientConnection, &QObject::deleteLater);
    clientConnection->write(block);
    clientConnection->disconnectFromHost();
}

QStringList Serverrr::filereader()
{

    QFile myFile ("C:\\Users\\user\\Desktop\\Textfile.txt");


    if(!myFile.open(QIODevice::OpenModeFlag::ReadOnly))
    {
        qCritical() << "can not open file";
        qCritical() << myFile.errorString();
        //  return ;

    }
    QTextStream stream(& myFile);


    while(!stream.atEnd())
    {

        QString line = stream.readLine();


        l.push_back(line);

    }



    return l;
}



