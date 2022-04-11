#ifndef SERVERRR_H
#define SERVERRR_H

#include <QDialog>
#include <QString>
#include <QList>
QT_BEGIN_NAMESPACE
class QLabel;
class QTcpServer;
QT_END_NAMESPACE

class Serverrr : public QDialog
{
    Q_OBJECT

public:
   explicit Serverrr(QWidget *parent = nullptr);

private slots:
    void send();
    QStringList filereader();
private:
    void initServer();


    QLabel *statusLabel = nullptr;
    QTcpServer * tcpServer = nullptr;
    QList<QString> fortunes;
    QStringList l;


};
#endif // SERVERRR_H
