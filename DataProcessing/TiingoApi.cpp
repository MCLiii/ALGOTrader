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
        string uri = url + "/api/test?token="+token;
        QNetworkRequest request((QUrl(QString::fromStdString(uri))));
        request.setRawHeader("Content-Type", "application/json");

        manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(readReply(QNetworkReply*)));
        manager->get(request);
        int retries = 0;
        waitForData();
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

    bool setTickerSymbol(string symbol, vector<string> params) override {
        symbol = symbol;
        if(subDir == "" && params.size() < 1) {
            qDebug() << "params required";
            return false;
        } else {
            subDir = params[0];
        }
        this->symbol = symbol;

        //test connection
        //set uri and send request
        string uri = url + subDir + "prices?tickers="+symbol +"&token="+token;
        QNetworkRequest request((QUrl(QString::fromStdString(uri))));
        request.setRawHeader("Content-Type", "application/json");
        manager->get(request);

        //Handle data response
        waitForData();
        if (dr) {
            if (rep.size() > 30) {
                qDebug() << "Token name set to " + QString::fromStdString(symbol);
                return true;
            } else {
                qDebug() << "Bad token name/dir";
                return false;
            }
        } else {
            qDebug() << "Connection failed";
            return false;
        }
    }

    void setSampleFreq(int min) override {
        sampleFreq = min;
    }

    void setHistoryLen(int days) override {
        historyLen = days;
    }

    bool dataReady() override {
        return dr;
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

    void startLive() override {
        tango = new std::thread(&TiingoApi::thread, this);
        tango->detach();
    }

private:
    void thread (){
        time_t curr_t = std::time(NULL);
        //get history data before current time;
        getHistoryData(curr_t);
    }

    void getHistoryData(time_t curr_t) {
        tm* gmtm = gmtime(&curr_t);
        //get the current time
        int curr_sec = gmtm ->tm_sec;
        int curr_min = gmtm ->tm_min;
        int curr_hr = gmtm ->tm_hour;
        //use the current time to offset the time to 0:0:0 UTC current date
        time_t offset_t = curr_t - curr_sec - curr_min*60 - curr_hr*3600;
        time_t startDate = dateConversion(offset_t, historyLen);
        //query maximum days with <=5000 items at a time until the current date at 0:0:0 UTC
        int maxQueryDays = sampleFreq * 5000 / (24*60);

        for (time_t queryStartDate = startDate ; queryStartDate < offset_t ; queryStartDate+=maxQueryDays) {
            time_t queryEndDate = queryStartDate + maxQueryDays > offset_t ? offset_t : queryStartDate + maxQueryDays;
            tm* gmtStart = gmtime(&queryStartDate);
            tm* gmtEnd = gmtime(&queryEndDate);

            //Date format in YYYY-MM-DD
            string startT = to_string(gmtStart->tm_year) + "-" + to_string(gmtStart->tm_mon) + "-"+ to_string(gmtStart->tm_mday);
            string endT = to_string(gmtEnd->tm_year) + "-" + to_string(gmtEnd->tm_mon) + "-"+ to_string(gmtEnd->tm_mday);

            //set uri and send request
            string uri = url + subDir + "prices?tickers="+symbol + "&startDate="+startT+"&endDate="+endT+"&resampleFreq="+
                    to_string(sampleFreq)+"min"+"&token="+token;
            QNetworkRequest request((QUrl(QString::fromStdString(uri))));
            request.setRawHeader("Content-Type", "application/json");
            manager->get(request);
            waitForData();
            if (dr) {
                parseData();
            } else {
                qDebug() << "Connection failed";
            }
        }

    }

    //the function subtracts days from current time
    time_t dateConversion(time_t curr, int days) {
        const time_t ONE_DAY = 24*60*60;
        time_t res = curr - ONE_DAY * days;
        return res;
    }

    void parseData() {
        //TODO codes to parse data into dataframe
    }

    void waitForData() {
        int retries = 0;
        while (!dr && retries < 100) {
            QCoreApplication::processEvents();
            usleep(50000);
            retries++;
        }
    }

    atomic<bool> dr; //data ready, set on when fetched, set off when accessed
    atomic<int> historyLen, sampleFreq;
    string token, url  = "https://api.tiingo.com/", subDir = "", symbol;
    QNetworkAccessManager *manager;
    QByteArray rep;
    std::thread* tango;
};