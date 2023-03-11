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
 * a interface for standardizing data sourcing functions
 */

using namespace hmdf;

using DTDataFrame = StdDataFrame<DateTime>;

class DataSource {
    struct DataForm {
        long long openTime;
        long long closeTime;
        int open;
        int close;
    };


};