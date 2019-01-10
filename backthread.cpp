#include "backthread.h"
#include <QFile>

BackThread::BackThread(QObject *parent) : QObject(parent){
}

IFace* BackThread::execGetIface(){
    IFace *iface = new IFace();
    QString cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli -f TYPE,DEVICE,STATE device > /tmp/kylin-nm-iface";
    system(cmd.toUtf8().data());

    QFile file("/tmp/kylin-nm-iface");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
    }
    QString txt = file.readAll();
    QStringList txtList = txt.split("\n");
    file.close();

    for(int i = 1; i < txtList.size(); i ++){
        QString line = txtList.at(i);
        if(line != ""){
            int index1 = line.indexOf(" ");
            QString type = line.left(index1);
            QString lastStr = line.mid(index1).trimmed();
            int index2 = lastStr.indexOf(" ");
            QString iname = lastStr.left(index2);
            QString istateStr = lastStr.mid(index2).trimmed();

            if(type == "ethernet"){
                iface->lname = iname;

                if(istateStr == "unmanaged"){
                    iface->lstate = 2;
                }
                if(istateStr == "disconnected"){
                    iface->lstate = 1;
                }
                if(istateStr == "connected"){
                    iface->lstate = 0;
                }
            }
            if(type == "wifi"){
                iface->wname = iname;

                if(istateStr == "unmanaged" || istateStr == "unavailable"){
                    iface->wstate = 2;
                }
                if(istateStr == "disconnected"){
                    iface->wstate = 1;
                }
                if(istateStr == "connected"){
                    iface->wstate = 0;
                }
            }
        }
    }

    return iface;
}

void BackThread::execEnNet(){
    system("nmcli networking on;sleep 5");
    emit enNetDone();
    emit btFinish();
}

void BackThread::execDisNet(){
    system("nmcli networking off;sleep 3");
    emit disNetDone();
    emit btFinish();
}

void BackThread::execEnWifi(){
    system("nmcli radio wifi on;sleep 5");
    emit enWifiDone();
    emit btFinish();
}

void BackThread::execDisWifi(){
    system("nmcli radio wifi off;sleep 3");
    emit disWifiDone();
    emit btFinish();
}

void BackThread::execConnLan(QString connName){
    QString cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli connection up '" + connName + "' > /tmp/kylin-nm-btoutput";
    system(cmd.toUtf8().data());

    QFile file("/tmp/kylin-nm-btoutput");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
    }
    QString line = file.readLine();
    file.close();

    if(line.indexOf("successfully") != -1){
        emit connDone(0);
    }else{
        emit connDone(1);
    }

    emit btFinish();
}

void BackThread::execConnWifi(QString connName){
    QString cmd = "/usr/share/kylin-nm/shell/connup.sh '" + connName + "'";
    system(cmd.toUtf8().data());

    QFile file("/tmp/kylin-nm-btoutput_");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
    }
    QString line = file.readLine();
    file.close();

    if(line.indexOf("successfully") != -1){
        emit connDone(0);
    }else if(line.indexOf("unknown") != -1){
        emit connDone(2);
    }else{
        emit connDone(1);
    }

    emit btFinish();
}

void BackThread::execConnWifiPWD(QString connName, QString password){
    qDebug()<<"once";
    QString cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli device wifi connect '" + connName + "' password '" + password + "' > /tmp/kylin-nm-btoutput";
    system(cmd.toUtf8().data());

    QFile file("/tmp/kylin-nm-btoutput");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
    }
    QString line = file.readLine();
    file.close();

    if(line.indexOf("successfully") != -1){
        emit connDone(0);
    }else{
        emit connDone(1);
    }

    emit btFinish();
}

QString BackThread::getConnProp(QString connName){
    QString cmd = "nmcli connection show '" + connName + "' > /tmp/kylin-nm-connprop";
    system(cmd.toUtf8().data());

    QFile file("/tmp/kylin-nm-connprop");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
    }

    QString txt = file.readAll();
    QStringList txtLine = txt.split("\n");
    file.close();

    QString rtn = "";
    foreach (QString line, txtLine) {
        if(line.startsWith("ipv4.method:")){
            QString v4method = line.mid(12).trimmed();
            rtn += "method:" + v4method + "|";
        }
        if(line.startsWith("ipv4.addresses:")){
            QString value = line.mid(15).trimmed();
            if(value == "--"){
                rtn += "addr:|mask:|";
            }else{
                QString addr = value.split("/").at(0);
                QString mask = value.trimmed().split("/").at(1);
                rtn += "addr:" + addr + "|";
                rtn += "mask:" + mask + "|";
            }
        }
        if(line.startsWith("ipv4.gateway:")){
            QString value = line.mid(13).trimmed();
            if(value == "--"){
                rtn += "gateway:|";
            }else{
                rtn += "gateway:" + value + "|";
            }
        }
        if(line.startsWith("ipv4.dns:")){
            QString value = line.mid(9).trimmed();
            if(value == "--"){
                rtn += "dns:|";
            }else{
                rtn += "dns:" + value + "|";
            }
        }
    }

    return rtn.left(rtn.length() - 1);
}

bool BackThread::execChkWifiExist(QString connName){
    QString cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli connection show '" + connName + "' > /tmp/kylin-nm-chkwifiexist";
    system(cmd.toUtf8().data());

    QFile file("/tmp/kylin-nm-chkwifiexist");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
    }
    QString line = file.readLine();
    file.close();

    if(line.length() < 2 || line.indexOf("Error:") != -1){
        return false;
    }else{
        return true;
    }
}

QString BackThread::execChkLanWidth(QString ethName){
    QString cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';ethtool '" + ethName + "' | grep Speed > /tmp/kylin-nm-bandwidth";
    system(cmd.toUtf8().data());

    QFile file("/tmp/kylin-nm-bandwidth");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
    }
    QString line = file.readLine();
    file.close();

    QStringList params = line.split(":");
    if(params.size() < 2){
        return "";
    }

    QString rtn = params.at(1);
    return rtn.trimmed();

}