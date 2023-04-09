//
// Created by Mingcan Li on 3/10/23.
//

#ifndef ALGOTRADING_DATASOURCING_H
#define ALGOTRADING_DATASOURCING_H

#endif //ALGOTRADING_DATASOURCING_H

#include <DataFrame/DataFrame.h>  // Main DataFrame header
#include <DataFrame/DataFrameFinancialVisitors.h>  // Financial algorithms
#include <DataFrame/DataFrameMLVisitors.h>  // Machine-learning algorithms
#include <DataFrame/DataFrameStatsVisitors.h>  // Statistical algorithms
#include <DataFrame/Utils/DateTime.h>  // Cool and handy date-time object
#include <QObject>
#include <QtNetwork>

/**
 * an interface for standardizing data sourcing functions
 */

using namespace hmdf;
using namespace std;

using TDF = StdDataFrame<int>;

class DataSource : public QObject{
    Q_OBJECT
public:
    virtual bool setTickerSymbol(string symbol, vector<string> params) = 0;
    virtual void setSampleFreq(int min) = 0;
    virtual void setHistoryLen(int days) = 0;
    virtual bool dataReady() = 0;   //to indicate that the data set is ready to query
    virtual TDF* getDataFrame() = 0;
    virtual int getCurrentPrice() = 0;
    virtual void startLive() = 0;
public slots:
    virtual void readReply(QNetworkReply* reply) {};
};