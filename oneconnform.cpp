#include "oneconnform.h"
#include "ui_oneconnform.h"
#include "mainwindow.h"

extern int currentActWifiSignalLv;

OneConnForm::OneConnForm(QWidget *parent, MainWindow *mainWindow, ConfForm *confForm, KSimpleNM *ksnm) :
    QWidget(parent),
    ui(new Ui::OneConnForm)
{
    ui->setupUi(this);

    ui->lbPassword->setText("输入密码:");
    ui->btnConf->setText("设置");
    ui->btnConn->setText("连接");
    ui->btnConnPWD->setText("连接");
    ui->btnDisConn->setText("断开连接");
    ui->lePassword->setEchoMode(QLineEdit::Password);

    ui->wbg->hide();
    ui->lbPassword->hide();
    ui->lePassword->hide();
    ui->btnConf->hide();
    ui->btnConn->hide();
    ui->btnDisConn->hide();
    ui->btnConnPWD->hide();

    ui->wbg->setStyleSheet("#wbg{background-color:#3593b5;}");
    ui->lbName->setStyleSheet("QLabel{font-size:13px;color:#ffffff;}");
    ui->lbPassword->setStyleSheet("QLabel{font-size:13px;color:#ffffff;}");
    ui->lbSafe->setStyleSheet("QLabel{font-size:13px;color:#aaaaaa;}");
    ui->lbConned->setStyleSheet("QLabel{font-size:13px;color:#ffffff;}");
    ui->lePassword->setStyleSheet("QLineEdit{border: 1px solid #cccccc;background-color:#ffffff;}");
    ui->btnConf->setStyleSheet("QPushButton{border:0px;background-color:rgba(0,0,0,0.4);color:white;font-size:12px;}"
                               "QPushButton:Hover{border:1px solid rgba(255,255,255,0.2);background-color:rgba(0,0,0,0.2);}"
                               "QPushButton:Pressed{background-color:rgba(0,0,0,0.6);}");
    ui->btnConn->setStyleSheet("QPushButton{border:0px;background-color:rgba(0,0,0,0.4);color:white;font-size:12px;}"
                               "QPushButton:Hover{border:1px solid rgba(255,255,255,0.2);background-color:rgba(0,0,0,0.2);}"
                               "QPushButton:Pressed{background-color:rgba(0,0,0,0.6);}");
    ui->btnConnPWD->setStyleSheet("QPushButton{border:0px;background-color:rgba(0,0,0,0.4);color:white;font-size:12px;}"
                               "QPushButton:Hover{border:1px solid rgba(255,255,255,0.2);background-color:rgba(0,0,0,0.2);}"
                               "QPushButton:Pressed{background-color:rgba(0,0,0,0.6);}");
    ui->btnDisConn->setStyleSheet("QPushButton{border:0px;background-color:rgba(0,0,0,0.4);color:white;font-size:12px;}"
                               "QPushButton:Hover{border:1px solid rgba(255,255,255,0.2);background-color:rgba(0,0,0,0.2);}"
                               "QPushButton:Pressed{background-color:rgba(0,0,0,0.6);}");

    this->mw = mainWindow;
    this->cf = confForm;
    this->ks = ksnm;

    this->isSelected = false;
    this->isActive = false;
}

OneConnForm::~OneConnForm()
{
    delete ui;
}

void OneConnForm::mouseReleaseEvent(QMouseEvent *){
    emit selectedOneWifiForm(wifiName);
}

// 是否当前连接的网络，字体设置不同
void OneConnForm::setAct(bool isAct){
    if(isAct){
        ui->lbName->setStyleSheet("QLabel{font-size:14px;color:#ffffff;}");
        ui->lbSafe->setStyleSheet("QLabel{font-size:14px;color:#aaaaaa;}");
        ui->lbConned->show();
        ui->btnConf->show();
    }else{
        ui->lbName->setStyleSheet("QLabel{font-size:12px;color:#ffffff;}");
        ui->lbSafe->setStyleSheet("QLabel{font-size:12px;color:#aaaaaa;}");
        ui->lbConned->hide();
        ui->btnConf->hide();
    }
    isActive = isAct;
}

// 是否选中
void OneConnForm::setSelected(bool isSelected){
    if(isSelected){
        resize(314, 111);
        ui->wbg->show();
//        ui->btnConf->show();
        if(isActive){
            ui->btnDisConn->show();
        }else{
            ui->btnConn->show();
        }

        this->isSelected = true;

    }else{
        resize(314, 60);
        ui->wbg->hide();

        if(isActive){
            ui->btnConf->show();
        }else{
            ui->btnConf->hide();
        }

        ui->btnConn->hide();
        ui->btnDisConn->hide();

        ui->lbPassword->hide();
        ui->lePassword->hide();
        ui->btnConnPWD->hide();

        ui->lbName->show();
        ui->lbSafe->show();
        ui->lbSignal->show();
        ui->lbPoint->show();

        this->isSelected = false;
    }
}

void OneConnForm::setConnedString(QString str){
    ui->lbConned->setText(str);
}

void OneConnForm::setSafeString(QString str){
    ui->lbSafe->setText(str);
}

void OneConnForm::setShowPoint(bool flag){
    if(flag){
        ui->lbPoint->show();
    }else{
        ui->lbPoint->hide();
    }
}

void OneConnForm::slotConnWifi(){
    mw->startLoading();
    emit sigConnWifi(ui->lbName->text());
}
void OneConnForm::slotConnWifiPWD(){
    mw->startLoading();
    emit sigConnWifiPWD(ui->lbName->text(), ui->lePassword->text());
}

void OneConnForm::setName(QString name){
    ui->lbName->setText(name);
    wifiName = name;
}

void OneConnForm::setSafe(QString safe){
    if(safe == "--"){
        this->isSafe = false;
        ui->lbSafe->setText("开放");
    }else{
        this->isSafe = true;
        ui->lbSafe->setText("安全");
    }
}

void OneConnForm::setRate(QString rate){
    this->setToolTip("<span style=\"font-size:13px;border:0px;background-color:#3593b5;color:white;\">&nbsp; 速率: " + rate + " &nbsp;</span>");
    QString rateStr = rate.split(" ").at(0);
    int rateNum = rateStr.toInt();
    if(rateNum >= 180){
        ui->lbPoint->setStyleSheet("QLabel{background:url(:/res/s/pgood.png);}");
    }
    if(rateNum >= 100 && rateNum < 180){
        ui->lbPoint->setStyleSheet("QLabel{background:url(:/res/s/pnormal.png);}");
    }
    if(rateNum <100){
        ui->lbPoint->setStyleSheet("QLabel{background:url(:/res/s/pbad.png);}");
    }
}

void OneConnForm::setSignal(QString lv){
    int signal = lv.toInt();
    if(signal > 75){
        if(this->isSafe){
            ui->lbSignal->setStyleSheet("QLabel{background:url(:/res/s/wifi-full-secure.png);}");
        }else{
            ui->lbSignal->setStyleSheet("QLabel{background:url(:/res/s/wifi-full.png);}");
        }
        signalLv = 1;
    }
    if(signal > 55 && signal <= 75){
        if(this->isSafe){
            ui->lbSignal->setStyleSheet("QLabel{background:url(:/res/s/wifi-high-secure.png);}");
        }else{
            ui->lbSignal->setStyleSheet("QLabel{background:url(:/res/s/wifi-high.png);}");
        }
        signalLv = 2;
    }
    if(signal > 35 && signal <= 55){
        if(this->isSafe){
            ui->lbSignal->setStyleSheet("QLabel{background:url(:/res/s/wifi-medium-secure.png);}");
        }else{
            ui->lbSignal->setStyleSheet("QLabel{background:url(:/res/s/wifi-medium.png);}");
        }
        signalLv = 3;
    }
    if(signal > 15 && signal <= 35){
        if(this->isSafe){
            ui->lbSignal->setStyleSheet("QLabel{background:url(:/res/s/wifi-low-secure.png);}");
        }else{
            ui->lbSignal->setStyleSheet("QLabel{background:url(:/res/s/wifi-low.png);}");
        }
        signalLv = 4;
    }
    if(signal <= 15){
        if(this->isSafe){
            ui->lbSignal->setStyleSheet("QLabel{background:url(:/res/s/wifi-none-secure.png);}");
        }else{
            ui->lbSignal->setStyleSheet("QLabel{background:url(:/res/s/wifi-none.png);}");
        }
        signalLv = 4;
    }
}

void OneConnForm::on_btnConf_clicked()
{
    QPoint pos = QCursor::pos();
    QRect primaryGeometry;
    for (QScreen *screen : qApp->screens()) {
        if (screen->geometry().contains(pos)) {
            primaryGeometry = screen->geometry();
        }
    }

    if (primaryGeometry.isEmpty()) {
        primaryGeometry = qApp->primaryScreen()->geometry();
    }

    BackThread *bt = new BackThread();
    QString connProp = bt->getConnProp(ui->lbName->text());
    QStringList propList = connProp.split("|");
    QString v4method, addr, mask, gateway, dns;
    foreach (QString line, propList) {
        if(line.startsWith("method:")){
            v4method = line.split(":").at(1);
        }
        if(line.startsWith("addr:")){
            addr = line.split(":").at(1);
        }
        if(line.startsWith("mask:")){
            mask = line.split(":").at(1);
        }
        if(line.startsWith("gateway:")){
            gateway= line.split(":").at(1);
        }
        if(line.startsWith("dns:")){
            dns = line.split(":").at(1);
        }
    }
//    qDebug()<<v4method<<addr<<mask<<gateway<<dns;

    cf->setProp(ui->lbName->text(), v4method, addr, mask, gateway, dns, this->isActive);

    cf->move(primaryGeometry.width() / 2 - cf->width() / 2, primaryGeometry.height() / 2 - cf->height() / 2);
    cf->show();
    cf->raise();
}

void OneConnForm::on_btnDisConn_clicked()
{
    kylin_network_set_con_down(ui->lbName->text().toUtf8().data());

    disconnect(this, SIGNAL(selectedOneWifiForm(QString)), mw, SLOT(oneWifiFormSelected(QString)));

    emit disconnActiveWifi();
}

void OneConnForm::on_btnConn_clicked()
{
    QThread *t = new QThread();
    BackThread *bt = new BackThread();
    bt->moveToThread(t);
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    connect(t, SIGNAL(started()), this, SLOT(slotConnWifi()));
    connect(this, SIGNAL(sigConnWifi(QString)), bt, SLOT(execConnWifi(QString)));
    connect(bt, SIGNAL(connDone(int)), mw, SLOT(connDone(int)));
    connect(bt, SIGNAL(connDone(int)), this, SLOT(slotConnDone(int)));
    connect(bt, SIGNAL(btFinish()), t, SLOT(quit()));
    t->start();
}

void OneConnForm::on_btnConnPWD_clicked()
{
    QThread *t = new QThread();
    BackThread *bt = new BackThread();
    bt->moveToThread(t);
    connect(t, SIGNAL(finished()), t, SLOT(deleteLater()));
    connect(t, SIGNAL(started()), this, SLOT(slotConnWifiPWD()));
    connect(this, SIGNAL(sigConnWifiPWD(QString, QString)), bt, SLOT(execConnWifiPWD(QString, QString)));
    connect(bt, SIGNAL(connDone(int)), mw, SLOT(connDone(int)));
    connect(bt, SIGNAL(connDone(int)), this, SLOT(slotConnDone(int)));
    connect(bt, SIGNAL(btFinish()), t, SLOT(quit()));
    t->start();
}

// Wifi连接结果，0成功 1失败 2没有配置文件
void OneConnForm::slotConnDone(int connFlag){
    qDebug()<<"slot conn done: "<<connFlag;
    // 无此wifi配置，需要输入密码创建配置文件尝试连接
    if(connFlag == 2){
        ui->lbPassword->show();
        ui->lePassword->show();
        ui->btnConnPWD->show();

        ui->lbName->hide();
        ui->lbSafe->hide();
        ui->lbSignal->hide();
        ui->lbPoint->hide();
        ui->btnConf->hide();
        ui->btnConn->hide();
        ui->btnDisConn->hide();
    }
    // 使用配置文件连接失败，需要删除该配置文件
    if(connFlag == 1){
        QString cmd = "export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli connection delete '" + ui->lbName->text() + "';notify-send '连接 Wifi 失败...' -t 3800";
        system(cmd.toUtf8().data());
    }

    // 设置全局变量，当前连接Wifi的信号强度
    currentActWifiSignalLv = signalLv;

    mw->stopLoading();
}