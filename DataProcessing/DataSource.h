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
/**
 * an interface for standardizing data sourcing functions
 */

using namespace hmdf;
using namespace std;

using TDF = StdDataFrame<int>;

class DataSource {
public:
    virtual bool setTickerSymbol(string Symbol) = 0;
    virtual void setTimeFrame(int sec) = 0;
    virtual void setHistoryLen(int min) = 0;
    virtual bool dataReady() = 0;
    virtual TDF getDataFrame() = 0;
    virtual int getCurrentPrice() = 0;
};