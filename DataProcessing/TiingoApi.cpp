//
// Created by Mingcan Li on 4/7/23.
//
#include "DataSource.h"
#include <iostream>
#include <QtNetwork>
#include <unistd.h>

using namespace std;
using namespace hmdf;

class TiingoApi : public DataSource{
public:
    TiingoApi(string token) {
        this->token = token;
        string uri = url + "test?token="+token;
        QNetworkRequest request((QUrl(QString::fromStdString(uri))));
        request.setRawHeader("Content-Type", "application/json");

        manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(readReply(QNetworkReply*)));
        manager->get(request);
        int retries = 0;
        while (!dr && retries < 100) {
            QCoreApplication::processEvents();
            usleep(50000);
            retries++;
        }
        if (dr) {
            if (rep.contains("success")) {
                qDebug() << "Success";
            } else {
                qDebug() << "Failed connection to server: " + QString::fromStdString(rep.toStdString());
                ::exit(0);
            }
        } else {
            qDebug() << "Connection Failed";
        }
        dr = false;
    }

    ~TiingoApi(){

    }

    bool setTickerSymbol(string Symbol) override {
        return false;
    }

    void setTimeFrame(int sec) override {

    }

    void setHistoryLen(int min) override {

    }

    bool dataReady() override {
        return false;
    }

    TDF* getDataFrame() override {
        return nullptr;
    }

    int getCurrentPrice() override {
        return 0;
    }

    void readReply(QNetworkReply* reply) override {
        rep = reply->readAll();
        dr = true;
    }

private:
    atomic<bool> dr; //data ready, set on when fetched, set off when accessed
    atomic<int> historyLen, timeframe;
    string token, url  = "https://api.tiingo.com/api/";
    QNetworkAccessManager *manager;
    QByteArray rep;
};