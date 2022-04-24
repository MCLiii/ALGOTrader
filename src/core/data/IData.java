package core.data;

import java.util.List;

/**
 * get price from market, separate implementations for different exchange
 */
public interface IData {
    public double setTickerSymbol(String symbol);

    public List<Record> getHistory(int startTime, int endTime, int sampleWidth);

    public double getCurrentPrice();
}
