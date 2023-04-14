//
// Created by Mingcan Li on 4/7/23.
//
#include "TiingoApi.cpp"
#include "QCoreApplication"
#include "time.h"

int main(int argc, char *argv[]){
    QCoreApplication a(argc, argv);
    DataSource* ds = new TiingoApi("apikey");
    ds->setTickerSymbol("btcusd",{"tiingo/crypto/"});
    ds->setHistoryLen(300);
    ds->setSampleFreq(5);
    ds->startLive();
    return a.exec();
}
